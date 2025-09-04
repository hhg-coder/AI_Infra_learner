#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <cstring>
#include <atomic>

// 1. 信号量实现 (基于C++11的mutex和condition_variable)
class Semaphore {
public:
    Semaphore(int count = 0) : count_(count) {}

    void signal() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return count_ > 0; });
        --count_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

// 2. 模拟网络数据结构
struct NetworkData {
    char buffer[1024]; // 假设的数据包大小
    size_t size;       // 实际数据长度
};

// 3. 固定大小的内存池
template<typename T, size_t PoolSize = 300>
class MemoryPool {
public:
    MemoryPool() {
        // 预先分配所有对象
        for (size_t i = 0; i < PoolSize; ++i) {
            free_list.push(&pool[i]);
        }
    }

    T* allocate() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (free_list.empty()) {
            return nullptr; // 池已耗尽，可根据需要扩展
        }
        T* obj = free_list.front();
        free_list.pop();
        return obj;
    }

    void deallocate(T* obj) {
        std::lock_guard<std::mutex> lock(mutex_);
        free_list.push(obj);
    }

    size_t available() const {
        return free_list.size();
    }

private:
    T pool[PoolSize];         // 预先分配的内存块
    std::queue<T*> free_list; // 空闲对象队列
    mutable std::mutex mutex_; // 保护空闲队列的互斥锁
};

// 4. 全局资源
MemoryPool<NetworkData> data_pool; // 内存池，预分配300个NetworkData
Semaphore data_available(0);       // 信号量：表示可处理的数据包数量
std::queue<NetworkData*> data_queue; // 共享数据队列
std::mutex queue_mutex;           // 保护数据队列的互斥锁
std::atomic<bool> running{true};   // 控制线程运行标志

// 5. 网络数据接收线程（模拟生产者）
void network_receiver_thread() {
    while (running) {
        // 模拟从网络接收数据...
        NetworkData* data = data_pool.allocate();
        if (data == nullptr) {
            std::cerr << "内存池耗尽！等待..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // 填充模拟数据
        std::strcpy(data->buffer, "模拟网络数据");
        data->size = strlen(data->buffer);

        // 将数据加入队列并通知处理线程
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            data_queue.push(data);
        }
        data_available.signal(); // 增加可用数据信号量

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 模拟接收间隔
    }
}

// 6. 数据处理线程（消费者）
const int CONSUMER_COUNT = 4; // 消费者线程数量

void data_processor_thread(int id) {
    while (running) {
        data_available.wait();

        NetworkData* data = nullptr;
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (!data_queue.empty()) {
                data = data_queue.front();
                data_queue.pop();
            }
        }

        if (data) {
            std::cout << "[消费者" << id << "] 处理数据: " << data->buffer << " (大小: " << data->size << ")" << std::endl;
            data_pool.deallocate(data);
        }
    }
}

int main() {
    // 启动网络接收线程和数据处理线程
    std::thread receiver(network_receiver_thread);
    std::vector<std::thread> consumers;
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumers.emplace_back(data_processor_thread, i + 1);
    }

    // 让程序运行一段时间
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 停止线程
    running = false;
    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        data_available.signal();
    }
    receiver.join();
    for (auto& t : consumers) t.join();

    std::cout << "程序结束。内存池剩余可用对象: " << data_pool.available() << std::endl;

    return 0;
}
