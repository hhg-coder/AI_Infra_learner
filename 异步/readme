std::promise<double> prom;
std::future<double> fut = prom.get_future();
std::thread worker([&prom] {
    // ...计算...
    prom.set_value(result);
});
本质：promise 是一个“承诺”，你可以在任何地方、任何时刻手动设置结果（或异常）。
适用场景：适合线程间通信，比如一个线程需要把结果/异常主动传递给另一个线程，甚至可以多次传递（用 promise<std::vector<T>> 传递批量数据）。
灵活性高：你可以在任何时刻、任何条件下设置结果或异常。

std::packaged_task<double(int)> task([](int n) {
    // ...计算...
    return result;
});
std::future<double> fut = task.get_future();
std::thread worker(std::move(task), 1000000000);
本质：packaged_task 是对一个可调用对象（函数、lambda等）的封装，自动把返回值传递给 future。
适用场景：适合任务调度/线程池，比如你有很多任务要分发给线程池执行，每个任务自动有自己的 future。
更像“任务”：你只需要定义任务逻辑，执行后结果自动传递，无需手动 set。

主要区别总结
特性	              promise/future	                       packaged_task/future
结果设置方式	    手动 set_value/set_exception	               自动（任务返回值）
适合场景	          线程通信、主动传递结果/异常	                 任务调度、线程池
代码风格	         更灵活、可控	                               更自动化、适合批量任务
关联 future 次数	  只能 get_future 一次	                       只能 get_future 一次

如果你只是想把一个任务的返回值异步传递给主线程，packaged_task 更简洁。
如果你需要更灵活的通信（比如异常、条件触发、批量数据），promise 更合适。
