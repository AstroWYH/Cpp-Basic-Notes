# 并发多线程&mutex&lock_guard&unique_lock&死锁

锁用来在多线程访问同一个资源时防止数据竞险，保证数据的一致性访问。多线程本来就是为了提高效率和响应速度，但锁的使用又限制了多线程的并行执行，这会降低效率，但为了保证数据正确，不得不使用锁，它们就是这样纠缠。结合锁进行**线程间同步**的条件变量使用，请参考条件变量condition variable 。

## 1 mutex互斥量

需要`#include <mutex>`

- 保护共享数据，操作时某个线程用代码把共享数据锁住，操作数据、解锁。
- 其他想操作共享数据的线程必须等待解锁，锁定住，操作，解锁。
- 多个线程尝试用lock()成员函数来加锁这把锁头，只有一个线程能锁定成功（成功的标志是lock()函数返回）。
- 如果没锁成功，那么流程卡在lock()这里，不断尝试去锁这把锁头。
- 先lock()，操作共享数据，再unlock()，每调用一次lock()，必然应该调用一个unlock()。

```cpp
#include <list>
#include <thread>
#include <mutex>

class A
{
public:
    //把收到的消息（玩家命令）入到一个队列的线程
    void inMsgRecvQueue()
    {
        for (int i = 0;i < 100;++i)
        {
            cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
            my_mutex.lock();
            msgRecvQueue.push_back(i);
            my_mutex.unlock();
        }
    }

    bool outMsgLULProc(int& command)
    {
        my_mutex.lock();
        if (!msgRecvQueue.empty())
        {
            command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            my_mutex.unlock();
            return true;
        }
        my_mutex.unlock();
        return false; 
    }

    //把数据从消息队列中取出的线程
    void outMsgRecvQueue()
    {
        int command = 0;
        for (int i = 0;i < 100;i++)
        {
            bool result = outMsgLULProc(command);
            if (result == true)
            {
                cout << "outMsgRecvQueue()执行，取出一个元素" << command << endl;
                //可以考虑对命令(数据)进行处理
            }
            else
            {
                cout << "outMsgRecvQueue()执行，但是目前消息队列中为空" << i << endl;
            }
        }
        cout << "end" << endl;
    }

private:
    std::list<int> msgRecvQueue;    //容器,专门用于代表玩家给咱们发送过来的命令
    std::mutex my_mutex; //创建一个互斥量 
};

int main()
{
    A myobja;
    std::thread myOutnMsgObj(&A::outMsgRecvQueue,&myobja);
    std::thread myInMsgObj(&A::inMsgRecvQueue,&myobja);
    myInMsgObj.join();
    myOutnMsgObj.join();

    return 0;
}
```

## 2 lock_guard

- 为了防止大家忘记unlock()，引入了一个叫std::lock_guard()的类模板：你忘记unlock()不要紧，我替你unlock()。
- 类似智能指针：你忘记释放内存不要紧，我替你释放。
- 创建lock_guard对象时，它将尝试获取提供给它的互斥锁的所有权。当控制流离开lock_guard对象的作用域时，lock_guard析构并释放互斥量。

- lock_guard取代了mutex的lock()和unlock()。
- 创建即加锁，作用域结束自动析构并解锁，无需手工解锁。
- 不能中途解锁，必须等作用域结束才解锁。
- 不能复制。

```cpp
bool outMsgLULProc(int& command)
{
    // my_mutex.lock();
    std::lock_guard<std::mutex> sguard(my_mutex); // std::lock_guard
    if (!msgRecvQueue.empty())
    {
        command = msgRecvQueue.front();
        msgRecvQueue.pop_front();
        // my_mutex.unlock();
        return true;
    }
    // my_mutex.unlock();
    return false; 
}
```

#### **std::adopt_lock**（第2个参数）

- 前提：mutex需要提前lock() ，否者会报异常。
- 含义：起标记作用，表示这个互斥量已经被lock了，通知lock_guard不需要再在构造函数中调用std::mutex::lock()。
- unique_lock也可以带std::adopt_lock标记，含义相同，就是不希望再在unique_lock()的构造函数中lock这个mutex。

### 参考链接

[(19条消息) c++11中的lock_guard和unique_lock使用浅析_guotianqing的博客-CSDN博客_lock_guard和unique_lock](https://blog.csdn.net/guotianqing/article/details/104002449)

[(19条消息) C++11多线程 unique_lock详解_u012507022的博客-CSDN博客_c++ unique_lock](https://blog.csdn.net/u012507022/article/details/85909567?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)

[(19条消息) C++并发与多线程（4）——unique_lock详解_Mount256的博客-CSDN博客_c++ unique_lock](https://blog.csdn.net/baidu_39514357/article/details/113903152)

[(19条消息) 多线程共享数据的保护+互斥量概念、用法、死锁演示及解决详解(std::lock()、std::adopt_lock)_HITXJ的博客-CSDN博客_adopt_lock](https://blog.csdn.net/HITXJ/article/details/108439190?spm=1001.2101.3001.6650.1&depth_1-utm_relevant_index=2)

[(18条消息) c++11多线程编程同步——使用条件变量condition variable_guotianqing的博客-CSDN博客_c++11 多线程 条件变量](https://blog.csdn.net/guotianqing/article/details/104017649)