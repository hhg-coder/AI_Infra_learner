# 管道适合父子进程间的简单数据流通信，数据单向流动，速度较慢。
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe1[2], pipe2[2];
    pipe(pipe1); // 父写子读
    pipe(pipe2); // 子写父读

    if (fork() == 0) {
        // 子进程
        close(pipe1[1]); // 关闭父写端
        close(pipe2[0]); // 关闭父读端

        int nums[5];
        read(pipe1[0], nums, sizeof(nums)); // 读父进程发来的数据

        int sum = 0;
        for (int i = 0; i < 5; ++i) sum += nums[i];

        write(pipe2[1], &sum, sizeof(sum)); // 把结果写回父进程

        close(pipe1[0]);
        close(pipe2[1]);
    } else {
        // 父进程
        close(pipe1[0]); // 关闭子读端
        close(pipe2[1]); // 关闭子写端

        int nums[5] = {1, 2, 3, 4, 5};
        write(pipe1[1], nums, sizeof(nums)); // 发送数据到子进程

        int result = 0;
        read(pipe2[0], &result, sizeof(result)); // 读取子进程返回的结果
        printf("Sum from child: %d\n", result);

        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL); // 等待子进程结束
    }
    return 0;
}

/***

Q:为什么需要close？
管道有两个端口：读端（fd[0]）和写端（fd[1]）。

每个进程只保留自己需要的端口，关闭不用的端口。
子进程只写，不读，所以关闭 fd[0]。
父进程只读，不写，所以关闭 fd[1]。
这样做有两个好处：
防止数据混乱或死锁。
让内核正确检测到管道的“写端/读端已关闭”，从而在对端关闭时返回EOF或SIGPIPE。

Q:fork()==0代表什么？
fork() 在父进程中返回子进程的PID（大于0）。
在子进程中返回0。
所以 if (fork() == 0) 这段代码只会在子进程中执行。


Q:pipe不需要考虑同步吗?
pipe（无名管道）在父子进程通信时通常不需要额外同步机制，原因如下：

数据流同步
管道本身是FIFO（先进先出）缓冲区，读写操作是顺序的。

写端写入数据，读端读取数据，操作天然同步。
如果读端没有数据可读，read 会阻塞等待；写端写满缓冲区，write 也会阻塞。
阻塞特性

读操作在没有数据时自动阻塞，直到有数据可读。
写操作在缓冲区满时自动阻塞，直到有空间可写。
父子进程分工明确

一般一个进程负责写，一个进程负责读，互不干扰。
注意：

如果多个进程同时写同一个管道，或者多个进程同时读同一个管道，就需要额外同步（如加锁），否则数据可能混乱。
管道适合一对一通信，复杂场景建议用消息队列或共享内存配合同步机制。
总结：
pipe在典型父子进程一对一通信场景下，不需要额外同步。多对多通信时才需要考虑同步。
***/
