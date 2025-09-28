GDB 提供了一系列专门为多线程调试设计的强大命令，这是其最大的优势：
info threads：​​列出所有线程​​，显示每个线程的 ID 和当前正在执行的函数。这是了解程序整体线程状态的第一个命令。
thread <id>：​​切换当前调试的线程上下文​​。你可以自由地在不同线程之间跳转。
thread apply all bt：​​一次性打印所有线程的完整调用堆栈（backtrace）​​。这是诊断死锁的“杀手锏”。你一眼就能看出哪些线程在运行，哪些线程阻塞在哪个锁的等待上。
break <location> thread <id>：​​设置线程特定的断点​​。断点只在指定的线程中触发，其他线程无视它。这在 IDE 中虽然也能实现，但在 GDB 中用命令更加直接。
condition <breakpoint_num> <condition>：为断点设置复杂的触发条件，例如var == 10 && thread_id == 2。

常用命令总结表
命令	作用
run	启动程序
Ctrl+C	中断程序
info threads	查看所有线程
thread N	切换到第 N 个线程
bt	查看当前线程调用堆栈
thread apply all bt	查看所有线程调用堆栈
break ...	设置断点{break main
break 文件名:行号
break 函数名}
next / step	单步执行
continue	继续运行
print 变量	打印变量值
list	查看源码
quit	退出 GDB


gdb调试多线程：
死锁：
info threads
  1  ... 0x00007f... in __futex_abstimed_wait_common64 ...
  2  ... futex_wait ... futex_word=0x5555555591a0 <mutexB>
  3  ... futex_wait ... futex_word=0x555555559160 <mutexA>
  4  ... 0x00007f... in some_other_function ...
  5  ... 0x00007f... in sleep ...

  线程2、3 停在 futex_wait 并且堆栈显示在加锁相关代码，这两个线程就是死锁线程。
线程4、5 在其他函数或 sleep，不是死锁线程。



gdb -p <pid> 的意思是：让 GDB 附加（attach）到正在运行的进程，其中 <pid> 是目标进程的进程号（Process ID）。
作用
可以在不重启进程的情况下，用 GDB 调试和分析该进程的当前状态。
常用于线上排查死锁、卡死、崩溃等问题。
常见场景
程序已经在后台运行，想临时分析问题。
服务器上服务卡住，想用 GDB 查看线程和堆栈。
常用后续操作
进入 GDB 后，可以用：
info threads 查看所有线程
thread apply all bt 查看所有线程堆栈
其他 GDB 调试命令
