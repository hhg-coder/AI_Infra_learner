# 消息队列支持不同进程间以消息为单位的通信，适合结构化数据，速度中等。
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

struct mymsgbuf {
    long mtype;
    int nums[5];
};

struct resultbuf {
    long mtype;
    int sum;
};

int main() {
    key_t key1 = ftok("msgqueue1", 65); // 数据消息队列
    key_t key2 = ftok("msgqueue2", 66); // 结果消息队列
    int msgid1 = msgget(key1, 0666 | IPC_CREAT);
    int msgid2 = msgget(key2, 0666 | IPC_CREAT);

    if (fork() == 0) {
        // 子进程：接收数据，计算和，返回结果
        struct mymsgbuf msg;
        msgrcv(msgid1, &msg, sizeof(msg.nums), 1, 0);

        int sum = 0;
        for (int i = 0; i < 5; ++i) sum += msg.nums[i];

        struct resultbuf res = {2, sum};
        msgsnd(msgid2, &res, sizeof(res.sum), 0);

        return 0;
    } else {
        // 父进程：发送数据，接收结果
        struct mymsgbuf msg = {1, {1, 2, 3, 4, 5}};
        msgsnd(msgid1, &msg, sizeof(msg.nums), 0);

        struct resultbuf res;
        msgrcv(msgid2, &res, sizeof(res.sum), 2, 0);
        printf("Sum from child: %d\n", res.sum);

        wait(NULL);
        msgctl(msgid1, IPC_RMID, NULL);
        msgctl(msgid2, IPC_RMID, NULL);
    }
    return 0;
}

/***
 * ftok：生成唯一key，标识一个IPC对象（如消息队列、共享内存等）。
msgget：创建或获取一个消息队列。
msgsnd：向消息队列发送消息。
msgrcv：从消息队列接收消息。
msgctl：控制消息队列（如删除、获取信息等）。
这些函数主要用于多进程通信，让不同进程通过消息队列交换数据。

 * Q :key1和key2分别标识了ipc对象，msgid1和msgid2是句柄吗，
 * 他们的作用是什么，如果还有一个无关的进程，在系统中启动了，
 * 并且声明了key_t key3 = ftok("msgqueue1", 65);
 * 他是不是使用的ipc对象和这里的key1是同一个对象?
 * 关键变量解释
1. key1 和 key2
key1 = ftok("msgqueue1", 65);
key2 = ftok("msgqueue2", 66);
这两个变量分别用来唯一标识两个不同的IPC对象（消息队列）。
ftok 通过文件名和项目ID生成唯一key，key相同则指向同一个IPC对象。
2. msgid1 和 msgid2
msgid1 = msgget(key1, 0666 | IPC_CREAT);
msgid2 = msgget(key2, 0666 | IPC_CREAT);
这两个变量是消息队列的句柄（ID），用于后续的消息发送、接收和删除操作。
你可以把它们理解为“打开文件”后得到的文件描述符，只不过这里是消息队列的“描述符”。
关于无关进程和key
如果另一个无关进程在系统中这样声明：
key_t key3 = ftok("msgqueue1", 65);
int msgid3 = msgget(key3, 0666);
由于ftok("msgqueue1", 65)生成的key和本代码中的key1完全一样，
所以key3和key1指向的是同一个IPC对象（同一个消息队列）。
只要权限允许，这个无关进程就可以和本代码的进程通过这个消息队列通信。

Q： 消息队列的底层原理是读写文件吗？
消息队列的底层不是普通的文件读写。

System V 消息队列（如你代码用的 msgget/msgsnd/msgrcv）是由操作系统内核管理的内存数据结构，并不直接对应磁盘上的普通文件。

消息队列的数据实际存储在内核空间的专用区域，进程通过系统调用与内核交互，完成消息的发送和接收。
ftok 虽然用文件名生成 key，但只是为了生成唯一标识，并不会真正读写这个文件。
/proc/sysvipc/msg
在 Linux 下，所有消息队列的信息可以通过 /proc/sysvipc/msg 查看，但这只是内核暴露的接口，不是实际存储。

总结：
消息队列的底层不是文件，而是内核维护的内存结构，进程通过系统调用访问，不涉及磁盘文件的读写。

Q : 在生产中，消息队列的通信可以是完全无关的两个进程吗?
在生产环境中，消息队列通信完全可以用于两个（或多个）互不相关、无亲缘关系的进程。
只要这些进程使用相同的key（通常通过ftok生成）和权限，就可以通过同一个消息队列进行数据交换，无需父子关系，也不需要先后启动顺序。

常见场景：

不同服务、不同用户、不同时间启动的进程都可以通过消息队列通信。
例如：一个日志收集进程和多个业务进程之间通过消息队列异步传递日志消息。
注意事项：

进程间要约定好key和消息结构。
权限（如0666）要设置合理，保证通信安全。
总结：
消息队列适合完全无关的进程间通信，是解耦和异步的典型IPC方式。

Q ：消息队列的同步是如何实现的？
消息队列的同步主要由操作系统内核自动完成，具体体现在以下几个方面：

原子性

每次msgsnd（发送）和msgrcv（接收）操作都是原子的，保证不会出现消息内容被拆分或混合的情况。
阻塞与唤醒

如果消息队列为空，msgrcv会阻塞等待，直到有消息到来。
如果消息队列已满，msgsnd会阻塞，直到有空间可写。
这样自动实现了生产者-消费者之间的同步。
并发安全

内核为消息队列维护锁和队列指针，多个进程同时操作同一个队列时不会出现数据竞争或混乱。
消息类型筛选

通过mtype可以有选择地接收特定类型的消息，进一步提升同步和调度的灵活性。
总结：
消息队列的同步完全由内核机制保障，用户无需手动加锁或同步，适合多进程安全通信。

Q : 如果只有一个进程给消息队列发送信息，但是有多个其它进程需要接收信息，那怎么知道哪些进程要获取对应正确的信息呢?
在 System V 消息队列中，每条消息都有一个类型字段（mtype），这就是区分和筛选消息的关键。

解决方案
不同进程接收不同类型的消息

发送消息时，设置不同的mtype（如1、2、3…）。
每个接收进程用msgrcv指定自己关心的mtype，只会收到对应类型的消息。
// 发送
msg.mtype = 2; // 发送给类型为2的进程
msgsnd(msgid, &msg, sizeof(msg.mtext), 0);

// 接收
msgrcv(msgid, &msg, sizeof(msg.mtext), 2, 0); // 只接收类型为2的消息
同一类型消息多个进程竞争

如果多个进程都用同一个mtype接收消息，则谁先调用msgrcv谁先拿到消息（类似“抢消息”）。
这种方式适合“任务分发”或“负载均衡”场景。
消息内容中自定义标识

如果消息类型不够用，可以在消息内容里加上进程ID、用户ID等自定义字段，进程收到消息后自行判断是否处理。
总结
推荐用mtype区分消息类型，每个进程只接收自己关心的类型。
如果需要更复杂的分发策略，可以在消息内容中增加标识字段，由进程自行判断。

Q : _msgflg参数是什么?
_msgflg 是 System V 消息队列相关函数（如 msgsnd、msgrcv、msgget）的参数之一，用于控制操作行为，常见取值如下：
1. msgget 的 _msgflg
控制消息队列的创建和权限。
常用值：
IPC_CREAT：如果消息队列不存在则创建。
IPC_EXCL：与 IPC_CREAT 一起用，若队列已存在则报错。
0666：权限（可读写）。
2. msgsnd 和 msgrcv 的 _msgflg
控制发送和接收的行为。
常用值：
0：默认阻塞操作。
IPC_NOWAIT：非阻塞操作，如果不能立即完成则直接返回错误。
MSG_NOERROR（仅 msgrcv）：如果消息太长则截断，不报错。
msgsnd(msgid, &msg, sizeof(msg.mtext), 0);           // 阻塞发送
msgrcv(msgid, &msg, sizeof(msg.mtext), 1, IPC_NOWAIT); // 非阻塞接收
_msgflg 控制消息队列操作的阻塞/非阻塞、权限、错误处理等行为，根据实际需求选择合适的参数组合。

 */
