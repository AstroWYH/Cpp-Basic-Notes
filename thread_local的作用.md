
```cpp
#include <iostream>
#include <thread>

thread_local int threadLocalVar = 0;

void threadFunction(int id) {
    threadLocalVar += id;
    std::cout << "Thread ID: " << id << " - Thread Local Variable: " << threadLocalVar << std::endl;
}

int main() {
    std::thread t1(threadFunction, 1);
    std::thread t2(threadFunction, 2);

    t1.join();
    t2.join();
    
    std::cout << "Main Thread - Thread Local Variable: " << threadLocalVar << std::endl;

    return 0;
}

Thread ID: 1 - Thread Local Variable: 1
Thread ID: 2 - Thread Local Variable: 2
Main Thread - Thread Local Variable: 0
```
在C++中，thread_local 是一个关键字，用于声明一个线程局部存储（thread-local storage）变量。线程局部存储意味着每个线程都会有自己独立的变量实例，这些变量对于不同的线程是隔离的，每个线程可以独立地操作和访问这些变量，而不会互相影响。这在多线程编程中非常有用，因为它可以避免线程之间共享数据导致的竞争条件和同步问题。
