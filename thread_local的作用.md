
```cpp
#include <iostream>
#include <thread>

thread_local int threadLocalVar = 0;

void threadFunction(int id) {
    for (int i = 0; i < 10; i++) {
      threadLocalVar += id;
      std::cout << "Thread ID: " << id << " - Thread Local Variable: " << threadLocalVar << std::endl;
    }
}

int main() {
    std::thread t1(threadFunction, 1);
    std::thread t2(threadFunction, 2);

    t1.join();
    t2.join();
    
    std::cout << "Main Thread - Thread Local Variable: " << threadLocalVar << std::endl;

    return 0;
}

// output
Thread ID: 1 - Thread Local Variable: 1Thread ID: 2 - Thread Local Variable: 2

Thread ID: 2 - Thread Local Variable: 4
Thread ID: 2 - Thread Local Variable: 6
Thread ID: 2 - Thread Local Variable: 8
Thread ID: 2 - Thread Local Variable: 10
Thread ID: 2 - Thread Local Variable: 12
Thread ID: 2 - Thread Local Variable: 14
Thread ID: 2 - Thread Local Variable: 16
Thread ID: 2 - Thread Local Variable: 18
Thread ID: 2 - Thread Local Variable: 20
Thread ID: 1 - Thread Local Variable: 2
Thread ID: 1 - Thread Local Variable: 3
Thread ID: 1 - Thread Local Variable: 4
Thread ID: 1 - Thread Local Variable: 5
Thread ID: 1 - Thread Local Variable: 6
Thread ID: 1 - Thread Local Variable: 7
Thread ID: 1 - Thread Local Variable: 8
Thread ID: 1 - Thread Local Variable: 9
Thread ID: 1 - Thread Local Variable: 10
Main Thread - Thread Local Variable: 0
```
在C++中，thread_local 是一个关键字，用于声明一个线程局部存储（thread-local storage）变量。线程局部存储意味着每个线程都会有自己独立的变量实例，这些变量对于不同的线程是隔离的，每个线程可以独立地操作和访问这些变量，而不会互相影响。这在多线程编程中非常有用，因为它可以避免线程之间共享数据导致的竞争条件和同步问题。

```cpp
#include <iostream>
#include <thread>

class ThreadLocalExample {
public:
    void SetValue(int value) {
        thread_local_value = value;  // 使用 thread_local 变量模拟类成员变量
    }

    int GetValue() {
        return thread_local_value;   // 获取 thread_local 变量的值
    }

private:
    thread_local static int thread_local_value;  // 类的静态成员变量作为 thread_local 变量
};

// 静态成员变量的定义
thread_local int ThreadLocalExample::thread_local_value = 0;

int main() {
    ThreadLocalExample instance;

    // 在不同的线程中设置和获取 thread_local 变量
    std::thread thread1([&instance]() {
        instance.SetValue(42);
        std::cout << "Thread 1 - Value: " << instance.GetValue() << std::endl;
    });

    std::thread thread2([&instance]() {
        instance.SetValue(123);
        std::cout << "Thread 2 - Value: " << instance.GetValue() << std::endl;
    });

    thread1.join();
    thread2.join();
    
    std::cout << "Main Thread - Value: " << instance.GetValue() << std::endl;

    return 0;
}

Thread 2 - Value: 123
Thread 1 - Value: 42
Main Thread - Value: 0
```
在类中使用thread_local，必须同时声明为static，否则编译无法通过。
