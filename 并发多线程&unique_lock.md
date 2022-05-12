# 并发多线程&unique_lock

unique_lock 是 lock_guard 的升级加强版，它具有 lock_guard 的所有功能，同时又具有其他很多方法，使用起来更强灵活方便，能够应对更复杂的锁定需要。

- unique_lock比lock_guard灵活很多，效率上差一点，内存占用多一点。
- 创建时可以不锁定（通过指定第二个参数为std::defer_lock），而在需要时再锁定。
- 可以随时加锁解锁。
- 作用域规则同 lock_guard，析构时自动释放锁。
- 不可复制，可移动。
- 条件变量需要该类型的锁作为参数（此时必须使用unique_lock）。

#### **std::adopt_lock**（第2个参数）

- 前提：mutex需要提前lock() ，否者会报异常。
- 含义：起标记作用，表示这个互斥量已经被lock了，通知lock_guard不需要再在构造函数中调用std::mutex::lock()。
- unique_lock也可以带std::adopt_lock标记，含义相同，就是不希望再在unique_lock()的构造函数中lock这个mutex。

#### **std::try_to_lock**（第2个参数）

- 前提：mutex不能提前被lock()，否者会报异常。
- 含义：会尝试用mutex的lock()去锁定这个mutex，但如果没有锁定成功，也会立即返回，并不会阻塞在那里。
- 一般该参数都与owns_locks()搭配使用，owns_locks()方法判断是否拿到锁，如拿到返回true。

```cpp
#include<iostream>
#include<thread>
#include<string>
#include<vector>
#include<list>
#include<mutex>
 
using namespace std;
 
class A
{
public:
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
			{ 
				std::unique_lock<std::mutex> sbguard(my_mutex, std::try_to_lock); // std::try_to_lock
				if (sbguard.owns_lock())
				{
					//拿到了锁
					msgRecvQueue.push_back(i); 
					//...
					//其他处理代码
				}
				else
				{
					//没拿到锁
					cout << "inMsgRecvQueue()执行，但没拿到锁头，只能干点别的事" << i << endl;
				}
			}
		}
	}
 
	bool outMsgLULProc(int &command)
	{
		my_mutex.lock();//要先lock(),后续才能用unique_lock的std::adopt_lock参数
		std::unique_lock<std::mutex> sbguard(my_mutex, std::adopt_lock); // std::adopt_lock
 
		std::chrono::milliseconds dura(20000);
		std::this_thread::sleep_for(dura);  //休息20s
 
		if (!msgRecvQueue.empty())
		{
			//消息不为空
			int command = msgRecvQueue.front();//返回第一个元素，但不检查元素是否存在
			msgRecvQueue.pop_front();//移除第一个元素。但不返回；
			
			return true;
		}
		return false;
	}
	//把数据从消息队列取出的线程
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 10000; i++)
		{
			bool result = outMsgLULProc(command);
 
			if (result == true)
			{
				cout << "outMsgRecvQueue()执行，取出一个元素" << endl;
				//处理数据
			}
			else
			{
				//消息队列为空
				cout << "inMsgRecvQueue()执行，但目前消息队列中为空！" << i << endl;
			}
		}
		cout << "end!" << endl;
	}
 
private:
	std::list<int> msgRecvQueue;//容器（消息队列），代表玩家发送过来的命令。
	std::mutex my_mutex;//创建一个互斥量（一把锁）
};
 
int main()
{
	A myobja;
 
	std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
 
	myOutMsgObj.join();
	myInMsgObj.join();
 
	cout << "主线程执行！" << endl;
 
	return 0;
}
```

#### std::defer_lock（第2个参数）

- 前提：mutex不能提前被lock()，否者会报异常。
- 含义：表示并没有给mutex加锁，只是初始化了一个没有加锁的mutex。

#### lock() & unlock()（成员函数）

- unique_lock生命周期内，加锁和解锁。
- 如果不想在定义unique_lock对象就给互斥量加上锁，那么就要在参数表中加入std::defer_lock，等到需要加锁时再引用方法lock()，这就是lock()的用处。
- unique_lock是“智能锁”，在其作用域外就会自动解锁，那为什么还需要unlock()呢？因为有时候程序员不希望被锁的代码太多，又或者需要暂时解锁，因此引入了方法unlock()，增加了灵活性。例如：

```cpp
void foo()
{
    unique_lock<mutex> myUniLock(myMutex, std::defer_lock); // std::defer_lock
    myUniLock.lock();
    //处理一些共享代码
    myUniLock.unlock();
    //处理一些非共享代码
    myUniLock.lock();
    //处理一些共享代码
}
```

- 补充：lock()的代码段越少，执行越快，整个程序的运行效率越高。

- 锁住的代码少，叫做粒度细，执行效率高；

- 锁住的代码多，叫做粒度粗，执行效率低；

- 要学会尽量选择合适粒度的代码进行保护，粒度太细，可能漏掉共享数据的保护；粒度太粗，会影响程序的运行效率。所以，选择合适的粒度，是高级程序员的能力和实力的体现。

#### try_lock()（成员函数）

- 尝试给互斥量加锁，如果拿不到锁，返回false；如果拿到了锁，返回true，这个函数是不阻塞的。
- 注：owns_lock()是判定是否拿到了锁，返回bool；而try_lock会尝试加锁，再返回bool。

```cpp
void inMsgRecvQueue(void)
{
    for (int i = 0; i < 10000; ++i)
    {
        unique_lock<mutex> myUnique(myMutex, defer_lock); //不加锁
        if (myUnique.try_lock() == true) //返回true，表示拿到了锁
            cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
        else //如果没拿到锁，去做别的事
            cout << "inMsgRecvQueue()执行，干别的事" << i << endl;
        msgRecvQueue.push_back(i);
    }
    return;
}
```

#### release（成员函数）

- 作用：返回它所管理的mutex对象指针，并释放所有权；也就是说，unique_lock和mutex不再有任何关系。

- release()和unlock()的区别：release是智能锁不再接管mutex，剩下的事务由你管理；而unlock是智能锁的一个方法，并没有与unique_lock脱离关系。

- 这就是说，如果原来mutex对象处于加锁状态且为unique_lock接管，那么当release之后你来负责这个mutex并负责解锁。

```cpp
void inMsgRecvQueue(void)
{
    for (int i = 0; i < 10000; ++i)
    {
        unique_lock<mutex> myUnique(myMutex); //加锁，相当于把myMutex和myUnique绑定在了一起
        mutex* ptr = myUnique.release(); //release()就是解除绑定，返回它所管理的mutex对象的指针，并释放所有权，后面都要手动操作了
        //所有权由ptr接管，如果原来mutex对象处理加锁状态，就需要ptr在以后进行解锁了
        cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
        msgRecvQueue.push_back(i);
        ptr->unlock(); //需要自己手动解锁，如果没解锁，程序会终止崩溃
    }
    return;
}
```

#### unique_lock所有权的传递

**1）使用移动语义std::move**：myUnique拥有myMutex的所有权，myUnique可以把自己对myMutex的所有权转移，但不能拷贝复制。

```cpp
void inMsgRecvQueue(void)
{
    for (int i = 0; i < 10000; ++i)
    {
        unique_lock<mutex> _myUnique(myMutex);
        //unique_lock<mutex> myUnique(_myUnique); 本条语句错误
        unique_lock<mutex> myUnique(std::move(_myUnique)); //本条语句正确，myUnique获得myMutex的所有权
        cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
        msgRecvQueue.push_back(i);
    }
    return;
}
```

### 参考链接

[(19条消息) c++11中的lock_guard和unique_lock使用浅析_guotianqing的博客-CSDN博客_lock_guard和unique_lock](https://blog.csdn.net/guotianqing/article/details/104002449)

[(19条消息) C++11多线程 unique_lock详解_u012507022的博客-CSDN博客_c++ unique_lock](https://blog.csdn.net/u012507022/article/details/85909567?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)

[(19条消息) C++并发与多线程（4）——unique_lock详解_Mount256的博客-CSDN博客_c++ unique_lock](https://blog.csdn.net/baidu_39514357/article/details/113903152)

[(19条消息) 多线程共享数据的保护+互斥量概念、用法、死锁演示及解决详解(std::lock()、std::adopt_lock)_HITXJ的博客-CSDN博客_adopt_lock](https://blog.csdn.net/HITXJ/article/details/108439190?spm=1001.2101.3001.6650.1&depth_1-utm_relevant_index=2)

[(18条消息) c++11多线程编程同步——使用条件变量condition variable_guotianqing的博客-CSDN博客_c++11 多线程 条件变量](https://blog.csdn.net/guotianqing/article/details/104017649)