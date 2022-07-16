# 并发多线程-condition_variable

### 线程&进程区别

- 1.线程是程序执行的最小单位，而进程是操作系统分配资源的最小单位（线程不拥有系统资源，但可以访问隶属进程的系统资源）。
- 2.一个进程可以有一个或多个线程。
- 3.进程之间相互独立，但同一进程下的各个线程之间共享程序的内存空间（代码段，数据集，堆等）及一些进程级的资源（如打开文件和信号等），各个线程之间有独立的寄存器，栈等。
- 4.线程上下文切换比进程上下文切换快得多（切换进程涉及CPU环境的设置，切换线程只需要保存和设置一些寄存器内容，并且进程创建和撤销时，在内存空间、IO设备等分配上都会有很大的开销）。

### condition_variable

- condition_variable是利用线程间共享的变量进行同步的一种机制，是在多线程程序中用来实现**"等待–>唤醒"**逻辑常用的方法。
- 用于维护一个条件(ready，与条件变量是不同的概念)，线程可以使用条件变量来等待某个条件(ready)为真，注意并不是等待条件变量为真。
- 两个线程利用条件变量及互斥锁实现同步。**条件变量**和**互斥锁**对两个线程来说是**全局**的。
- 当条件(ready)不满足时,一个线程利用条件变量实现**等待(wait)**，同时**释放锁**。一个线程获取锁后利用该条件变量**唤醒等待的线程（notify_*）**，并让条件(ready)满足。

### 典型案例

![image-20220716194833225](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220716194833225.png)

```cpp
#include <iostream>
#include <thread>                // std::thread
#include <mutex>                 // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

std::mutex mtx; // 全局互斥锁
std::condition_variable cv; // 全局条件变量
bool ready = false; // 全局标志位

void do_print_id(int id)
{
    std::cout << "谁卡在这：" << id << '\n';
    std::unique_lock<std::mutex> lck(mtx);
    std::cout << "谁进来了：" << id << '\n';
    while (!ready)
        cv.wait(lck);
        // 某个thread先拿到lck后，因为ready为false，所以wait阻塞并释放lck，然后下个thread
        // 拿到lck进来同样wait阻塞并释放lck，此时main线程可能去go()里notify，并设ready为ture
        // 那么先进来wait这几个就好重新唤醒，并又去抢着lck.lock()；而后面还没进来的thread就会
        // 绕过ready，直接打印下面的log了。
    std::cout << "thread " << id << '\n';
} 

void go()
{
    std::cout << "main go" << '\n';
    std::unique_lock<std::mutex> lck(mtx);
    ready = true; // 设置全局标志位为true
    cv.notify_all(); // 唤醒所有线程
}

int main()
{
    std::thread threads[10];
    // spawn 10 threads
    for (int i = 0; i < 10; ++i)
        threads[i] = std::thread(do_print_id, i);
    std::cout << "10 threads ready to race...\n";
    go(); // go!
    for (auto & th:threads)
        th.join();

    return 0;
}

谁卡在这：0
谁进来了：0
谁卡在这：1
谁进来了：1
谁卡在这：5
谁进来了：5
谁卡在这：3
谁进来了：3
谁卡在这：2
谁进来了：2
谁卡在这：4
谁进来了：4
10 threads ready to race...
main go
thread 1
thread 5
thread 0
thread 4
thread 3
谁卡在这：6
谁进来了：6
thread 6
thread 2
谁卡在这：9
谁进来了：9
thread 9
谁卡在这：7
谁进来了：7
thread 7
谁卡在这：8
谁进来了：8
thread 8
```



### 参考链接

[(20条消息) C++11并发编程-条件变量(condition_variable)详解_下一页盛夏花开的博客-CSDN博客_condition_variable](https://blog.csdn.net/lv0918_qian/article/details/81745723?ops_request_misc=%7B%22request%5Fid%22%3A%22165796507816781685388799%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=165796507816781685388799&biz_id=0&spm=1018.2226.3001.4187)

[(20条消息) 条件变量(condition_variable)_持续学习，不断沉淀的博客-CSDN博客_条件变量](https://blog.csdn.net/qq_33726635/article/details/124263403?ops_request_misc=%7B%22request%5Fid%22%3A%22165796507816781685388799%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=165796507816781685388799&biz_id=0&spm=1018.2226.3001.4187)

[(20条消息) C++ std::thread初步了解_龚建波的博客-CSDN博客_std没有成员thread](https://blog.csdn.net/gongjianbo1992/article/details/99209119?ops_request_misc=%7B%22request%5Fid%22%3A%22165796968516782246476614%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=165796968516782246476614&biz_id=0&spm=1018.2226.3001.4187)

