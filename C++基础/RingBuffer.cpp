// #pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

template<typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity)
        : buffer_(capacity), capacity_(capacity), head_(0), tail_(0), size_(0) {}

    // 写入数据，满则阻塞
    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        /***条件为 true：直接通过，不阻塞，继续往下执行（即 size_ < capacity_，说明缓冲区未满，可以写入）。
条件为 false：自动释放 lock，线程阻塞等待 not_full_ 被唤醒。被唤醒后会重新获得 lock 并再次判断条件，直到条件为 true 才继续执行。***/
        not_full_.wait(lock, [this] { return size_ < capacity_; });

        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
        ++size_;

        not_empty_.notify_one();
    }

    // 读取数据，空则阻塞
    T pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return size_ > 0; });

        T item = buffer_[head_];
        head_ = (head_ + 1) % capacity_;
        --size_;

        not_full_.notify_one();
        return item;
    }

    // 当前元素个数
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return size_;
    }

    // 缓冲区容量
    size_t capacity() const {
        return capacity_;
    }

private:
    std::vector<T> buffer_;
    size_t capacity_;
    size_t head_;
    size_t tail_;
    size_t size_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
};

#include <iostream>
#include <thread>
#include <vector>
#include <cassert>

void single_thread_test() {
    RingBuffer<int> buf(3);
    buf.push(1);
    buf.push(2);
    buf.push(3);
    assert(buf.size() == 3);
    assert(buf.pop() == 1);
    assert(buf.pop() == 2);
    assert(buf.pop() == 3);
    std::cout << "Single thread test passed." << std::endl;
}

void producer(RingBuffer<int>& buf, int start, int count) {
    for (int i = 0; i < count; ++i) {
        buf.push(start + i);
    }
}

void consumer(RingBuffer<int>& buf, std::vector<int>& out, int count) {
    for (int i = 0; i < count; ++i) {
        out.push_back(buf.pop());
    }
}

void multi_thread_test() {
    RingBuffer<int> buf(5);
    std::vector<int> output;
    int count = 1000;

    /***std::ref 是 C++ 标准库中的一个函数模板，用于生成对某个对象的引用包装器（std::reference_wrapper）。
它常用于需要传递引用但接口只接受值（如线程、算法等）的场景。

主要用途
保证对象以引用方式传递，而不是拷贝。
常用于 std::thread、std::bind、STL 算法等。***/
    std::thread prod(producer, std::ref(buf), 0, count);
    std::thread cons(consumer, std::ref(buf), std::ref(output), count);

    prod.join();
    cons.join();

    assert(output.size() == count);
    for (int i = 0; i < count; ++i) {
        assert(output[i] == i);
    }
    std::cout << "Multi thread test passed." << std::endl;
}

int main() {
    single_thread_test();
    multi_thread_test();
    return 0;
}
