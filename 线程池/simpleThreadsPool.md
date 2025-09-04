线程池嘛，主要功能就是管理调度线程，节省线程重复申请/释放所带来的损耗。我们先来聊一聊现在github和B乎上找到的一些通用实现方法。很多的思路都很简单：一般就是提供一个接口，可以把任务塞入一个queue中，交由线程池中的线程异步执行，然后等待（也可以不等待）结果返回。

(./images/ThreadsPool/image.png)

下面简要说明这个线程池的执行流程：

1. 初始化线程池
ThreadPool pool(4);
构造函数创建 4 个工作线程，每个线程都在后台运行 workerLoop()。
2. 提交任务
pool.submit([i]() { ... });i { ... });
主线程循环提交 10 个任务，每个任务是一个 lambda 表达式，打印任务编号和线程 ID。

submit 方法将任务加入队列，并唤醒一个等待的工作线程。

3. 工作线程执行任务
每个工作线程在 workerLoop() 中循环：
等待任务队列非空（或线程池停止）。
从队列取出一个任务（std::function<void()>）。
执行该任务（调用 task()）。
重复上述过程，直到线程池停止。
4. 主线程等待
主线程休眠 2 秒，等待所有任务执行完毕。
5. 线程池析构
主线程结束时，线程池析构：
设置停止标志。
唤醒所有工作线程。
等待所有线程退出（join）。
总结：
主线程提交任务，工作线程自动从队列取任务并执行，实现了任务的并发处理和线程复用。


std::queue<std::function<void()>> tasks;对于tasks，如果有多个线程push或者pull任务，如何去做同步?
对于 tasks（即 std::queue<std::function<void()>> tasks;），如果有多个线程同时 push（添加任务）或 pull（取任务），必须加锁同步，否则会导致数据竞争和未定义行为。

在你的线程池实现中，已经这样做了：

push（添加任务）时，在 submit 方法里用
std::lock_guard<std::mutex> lock(queueMutex);
保护了 tasks.push(...)。

pull（取任务）时，在 workerLoop 里用
std::unique_lock<std::mutex> lock(queueMutex);
保护了 tasks.front() 和 tasks.pop()。

总结：
只要每次访问 tasks 时都加同一个互斥锁（queueMutex），就能保证线程安全。
这就是多线程环境下对共享队列的标准做法。
