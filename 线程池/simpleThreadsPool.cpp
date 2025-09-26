#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t threadCount) : stopFlag(false) {
        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back([this]() { this->workerLoop(); });
        }
    }

    ~ThreadPool() {
        stopFlag = true;
        cv.notify_all();
        for (auto& t : workers) {
            if (t.joinable()) t.join();
        }
    }

    // 提交任务，返回是否成功
    bool submit(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (stopFlag) return false;
            tasks.push(std::move(task));
        }
        cv.notify_one();
        return true;
    }

private:
    void workerLoop() {
        while (!stopFlag) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                /***当 stopFlag 为 true 时，线程池准备关闭，这时所有等待的线程都会被唤醒。
唤醒后，线程会判断：如果 stopFlag 为 true 或者任务队列为空，就直接 return，线程结束。
如果还有任务没处理完（即 tasks 非空），线程会继续把任务处理完再退出。
                    ***/
                cv.wait(lock, [this]() { return stopFlag || !tasks.empty(); });
                if (stopFlag && tasks.empty()) return;
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stopFlag;
};

// 示例用法
#include <iostream>
#include <chrono>

std::mutex printMutex;
int main() {
    ThreadPool pool(4); // 创建4个线程

    for (int i = 0; i < 100; ++i) {
        pool.submit([i]() {
            // std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "任务 " << i << " 在线程 " << std::this_thread::get_id() << " 执行\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // 等待任务执行
    std::cout << "主线程结束\n";
    return 0;
}
