**C++ 智能指针实现**

C++ 中智能指针的实现主要依赖于下列几个技术概念：

1、析构函数，对象被销毁时会被调用的一个函数**，对于基于栈的对象而言，如果对象离开其作用域则对象会被自动销毁**，而此时析构函数也自动会被调用。

2、引用计数技术，维护一个计数器用于追踪资源(如内存)的被引用数，当资源被引用时，计数器值加1，当资源被解引用时，计算器值减1。

3、**赋值操作符重载**。

智能指针的大致实现原理就是在**析构函数中，检查所引用对象的引用计数，如果引用计数为0，则真正释放该对象内存**。

```cpp
#include <iostream>
using namespace std;

// 引用计数类
class counter {
public:
  counter() {}
  counter(int parCount) : count(parCount) {}
  void increaseCount() { count++; }
  void decreasCount() { count--; }
  int getCount() { return count; }

private:
  int count;
};

// 智能指针
template <class T> class SmartPointer {
public:
  explicit SmartPointer(T *pT) : mPtr(pT), pCounter(new counter(1)) {}
  explicit SmartPointer() : mPtr(NULL), pCounter(NULL) {}
  ~SmartPointer() // 析构函数，在引用计数为0时，释放原指针内存
  {
    if (pCounter != NULL) {
      pCounter->decreasCount();
      if (pCounter->getCount() == 0) {
        delete pCounter;
        delete mPtr;
        pCounter = NULL; // 将pCounter赋值为NULL,防止悬垂指针
        mPtr = NULL;
        cout << "delete original pointer" << endl;
      }
    }
  }

  SmartPointer(SmartPointer<T> &rh) // 拷贝构造函数，引用加1
  {
    this->mPtr = rh.mPtr;
    this->pCounter = rh.pCounter;
    this->pCounter->increaseCount();
  }

  SmartPointer<T> &operator=(SmartPointer<T> &rh) // 赋值操作符，引用加1
  {
    if (this->mPtr == rh.mPtr)
      return *this;
    this->mPtr = rh.mPtr;
    this->pCounter = rh.pCounter;
    this->pCounter->increaseCount();
    return *this;
  }
  T &operator*() // 重载*操作符
  {
    return *mPtr;
  }

  T *operator->() // 重载->操作符
  {
    return p;
  }
  T *get() { return mPtr; }

private:
  T *mPtr;
  counter *pCounter;
};

int main(int argc, _TCHAR *argv[]) {
  SmartPointer<int> sp1(new int(10));
  SmartPointer<int> sp2 = sp1;
  SmartPointer<int> sp3;
  sp3 = sp2;
  return 0;
}
```

