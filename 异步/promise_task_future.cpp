#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <random>
#include <cmath>
#include <iomanip>

// 使用 promise 和 future 的示例
void promiseFutureExample() {
    std::cout << "\n=== Promise & Future Example ===\n";
    
    // 创建 promise 和关联的 future
    std::promise<double> prom;
    std::future<double> fut = prom.get_future();
    
    // 启动工作线程
    std::thread worker([&prom] {
        std::cout << "Worker thread started\n";
        
        // 模拟复杂计算
        double result = 0;
        for (int i = 0; i < 1000000000; i++) {
            result += std::sin(i * 0.001);
        }
        
        // 设置结果到 promise
        prom.set_value(result);
        std::cout << "Worker thread completed\n";
    });
    
    // 主线程可以做其他工作...
    std::cout << "Main thread doing other work...\n";
    
    // 等待结果
    double result = fut.get();
    std::cout << "Result from worker: " << std::setprecision(10) << result << "\n";
    std::cout << "Main thread doing other work2...\n";
    
    worker.join();
}

// 使用 packaged_task 的示例
void packagedTaskExample() {
    std::cout << "\n=== Packaged Task Example ===\n";
    
    // 创建一个 packaged_task，包装一个计算函数
    std::packaged_task<double(int)> task([](int n) {
        std::cout << "Task started with n = " << n << "\n";
        
        // 模拟复杂计算
        double result = 0;
        for (int i = 0; i < n; i++) {
            result += std::sin(i * 0.001);
        }
        
        std::cout << "Task completed\n";
        return result;
    });
    
    // 获取与任务关联的 future
    std::future<double> fut = task.get_future();
    
    // 在另一个线程中执行任务
    std::thread worker(std::move(task), 1000000000);
    
    // 主线程可以做其他工作...
    std::cout << "Main thread doing other work...\n";
    
    // 等待结果
    double result = fut.get();
    std::cout << "Result from task: " << std::setprecision(10) << result << "\n";
    
    worker.join();
}

// 综合示例：使用 promise 传递异常
void exceptionHandlingExample() {
    std::cout << "\n=== Exception Handling Example ===\n";
    
    // 创建 promise 和关联的 future
    std::promise<double> prom;
    std::future<double> fut = prom.get_future();
    
    // 启动工作线程
    std::thread worker([&prom] {
        try {
            std::cout << "Worker thread started\n";
            
            // 模拟可能失败的操作
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(1, 10);
            
            if (dist(gen) > 8) { // 20% 概率失败
                throw std::runtime_error("Random failure occurred!");
            }
            
            // 正常计算
            double result = 0;
            for (int i = 0; i < 1000000000; i++) {
                result += std::sin(i * 0.001);
            }
            
            prom.set_value(result);
            std::cout << "Worker thread completed successfully\n";
        } catch (...) {
            // 捕获所有异常并通过 promise 传递
            prom.set_exception(std::current_exception());
            std::cout << "Worker thread caught an exception\n";
        }
    });
    
    try {
        // 等待结果或异常
        double result = fut.get();
        std::cout << "Result: " << std::setprecision(10) << result << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in main thread: " << e.what() << "\n";
    }
    
    worker.join();
}

int main() {
    promiseFutureExample();
    packagedTaskExample();
    exceptionHandlingExample();
    
    return 0;
}
