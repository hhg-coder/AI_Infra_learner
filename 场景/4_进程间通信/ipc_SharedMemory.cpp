#include <sys/ipc.h>                // IPC相关函数
#include <sys/shm.h>                // 共享内存相关
#include <sys/sem.h>                // 信号量相关
#include <unistd.h>                 // fork、usleep等
#include <cstring>                  // C字符串操作
#include <cstdio>                   // printf、snprintf
#include <cstdlib>                  // exit
#include <thread>                   // std::thread
#include <mutex>                    // std::mutex, std::lock_guard
#include <condition_variable>       // std::condition_variable
#include <queue>                    // std::queue
#include <string>                   // std::string

#define SHM_SIZE 4096               // 共享内存大小
#define DATA_SIZE 128               // 单条数据最大长度

// 信号量P操作（等待/减1）
void sem_p(int semid) {
    struct sembuf sb = {0, -1, 0};  // 操作第0个信号量，-1，默认flag
    semop(semid, &sb, 1);           // 执行信号量操作
}

// 信号量V操作（释放/加1）
void sem_v(int semid) {
    struct sembuf sb = {0, 1, 0};   // 操作第0个信号量，+1，默认flag
    semop(semid, &sb, 1);           // 执行信号量操作
}

// 简单线程安全内存池
class MemPool {
public:
    // 向内存池推入数据
    void push(const std::string& data) {
        std::lock_guard<std::mutex> lock(mtx_); // 自动加锁
        queue_.push(data);                      // 入队
        cv_.notify_one();                       // 唤醒等待线程
    }
    // 从内存池弹出数据（阻塞等待）
    std::string pop() {
        std::unique_lock<std::mutex> lock(mtx_); // 加锁
        cv_.wait(lock, [this]{ return !queue_.empty(); }); // 等待队列非空
        std::string data = queue_.front();       // 取队首
        queue_.pop();                            // 出队
        return data;
    }
private:
    std::queue<std::string> queue_;              // 数据队列
    std::mutex mtx_;                             // 互斥锁
    std::condition_variable cv_;                 // 条件变量
};

// 转发线程函数，从内存池取数据并处理
void forward_thread(MemPool* pool) {
    while (true) {
        std::string data = pool->pop();          // 从内存池取数据
        printf("Forward thread: %s\n", data.c_str()); // 模拟转发
        if (data == "END") break;                // 收到END则退出
    }
}

int main() {
    key_t shmkey = ftok("shmfile", 65);          // 生成共享内存key
    key_t semkey = ftok("semfile", 66);          // 生成信号量key

    int shmid = shmget(shmkey, SHM_SIZE, 0666|IPC_CREAT); // 创建/获取共享内存
    int semid = semget(semkey, 1, 0666|IPC_CREAT);        // 创建/获取信号量
    semctl(semid, 0, SETVAL, 0);                          // 初始化信号量为0

    char* shm_buf = (char*)shmat(shmid, nullptr, 0);      // 映射共享内存到本进程

    MemPool pool;                                         // 创建内存池

    if (fork() == 0) {
        // 子进程：生产者，快速产生数据
        for (int i = 0; i < 10; ++i) {
            snprintf(shm_buf, DATA_SIZE, "Data %d", i+1); // 写数据到共享内存
            sem_v(semid);                                 // 通知接收进程
            usleep(100000);                               // 模拟快速产生
        }
        snprintf(shm_buf, DATA_SIZE, "END");              // 发送结束标志
        sem_v(semid);                                     // 通知接收进程
        shmdt(shm_buf);                                   // 解除映射
        exit(0);                                          // 退出子进程
    } else {
        // 父进程：接收进程
        std::thread t(forward_thread, &pool);             // 启动转发线程

        while (true) {
            sem_p(semid);                                 // 等待数据到来
            std::string data(shm_buf);                    // 读取共享内存数据
            pool.push(data);                              // 放入内存池
            if (data == "END") break;                     // 收到END则退出循环
        }

        t.join();                                         // 等待转发线程结束
        shmdt(shm_buf);                                   // 解除映射
        shmctl(shmid, IPC_RMID, nullptr);                 // 删除共享内存
        semctl(semid, 0, IPC_RMID);                       // 删除信号量
    }
    return 0;
}
