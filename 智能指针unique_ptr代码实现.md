## C++ 智能指针 unique_ptr 原理与自定义实现

**特点**：

`独享它指向的对象。也就是说，同时只有一个unique_ptr指向同一个对象，当这个unique_ptr被销毁时，指向的对象也随即被销毁`。 

这也是它和shared_ptr不一样的地方。它不需要做引用计数，也不可以被第二个人引用。只有它自己。如果想引用？没用的，编译都编译不过！

**典型用途**：

1. 作为一个类的成员变量，这个变量只在本类使用，不会被赋值给其他类，也不会作为参数传递给某个函数
2. 在一个函数作为局部变量，使用完就不用再管，函数结束，自动释放托管资源

**原理**:  

```
a) 构造时传入托管对象的指针，析构时delete对象` 
b) 禁用赋值函数`
```

```
a) 是为了要对得其智能指针的名号，即，帮你主动删除托管对象。
b) 是为了要对得起unique的名号，唯一啊唯一，那就禁用赋值，不能再让别人引用它！
```

```cpp
#include <iostream>
#include <utility>

/****
 * 智能指针unique_ptr的简单实现
 *
 * 特点：独享它指向的对象。也就是说，同时只有一个unique_ptr指向同一个对象，当这个unique_ptr被销毁时，指向的对象也随即被销毁
 *
 * 典型用途：
 * 1. 在一个函数定义一个A* ptr = new A(),
 * 结束还需要用delete，而用unique_ptr，就不需要自己调用delete
 * 2.
 * 作为一个类的变量，这个变量只在本类使用，不会被其他类调用，也不会作为参数传递给某个函数
 * */
template <typename T> class unique_ptr {
private:
  T *ptr_resource = nullptr;

public:
  // explicit构造函数是用来防止隐式转换, 即不允许写成unique_ptr<T> tempPtr = T;
  // std::move是将对象的状态或者所有权从一个对象转移到另一个对象，只是转移，没有内存的搬迁或者内存拷贝所以可以提高利用效率,改善性能.
  // move之后，raw_resource内部的资源将不能再被raw_resource使用
  explicit unique_ptr(T *raw_resource) noexcept
      : ptr_resource(std::move(raw_resource)) {}
  unique_ptr(std::nullptr_t) : ptr_resource(nullptr) {}

  unique_ptr() noexcept : ptr_resource(nullptr) {}

  // 析构时, 释放托管的对象资源
  ~unique_ptr() noexcept { delete ptr_resource; }
  // Disables the copy/ctor and copy assignment operator. We cannot have two
  // copies exist or it'll bypass the RAII concept.
  // 重要，禁止两种拷贝的赋值方式
  // 使用"=delete"修饰，表示函数被定义为deleted，也就意味着这个成员函数不能再被调用，否则就会出错。
  unique_ptr(const unique_ptr<T> &) noexcept = delete;
  unique_ptr &operator=(const unique_ptr &) noexcept = delete;

public:
  //&& 是右值引用，见https://zhuanlan.zhihu.com/p/107445960
  // 允许移动语义。虽然无法复制unique_ptr，但可以安全地移动。
  // 例子：unique_ptr<Test> tPtr3(std::move(tPtr1));
  unique_ptr(unique_ptr &&move) noexcept {
    std::cout << "construct for unique_ptr&&" << std::endl;
    move.swap(*this);
  }
  // ptr = std::move(resource)
  unique_ptr &operator=(unique_ptr &&move) noexcept {
    std::cout << "operator= for unique_ptr&&" << std::endl;
    move.swap(*this);
    return *this;
  }

  explicit operator bool() const noexcept { return this->ptr_resource; }
  // releases the ownership of the resource. The user is now responsible for
  // memory clean-up.
  T *release() noexcept { return std::exchange(ptr_resource, nullptr); }
  // returns a pointer to the resource
  T *get() const noexcept { return ptr_resource; }
  // swaps the resources
  void swap(unique_ptr<T> &resource_ptr) noexcept {
    std::swap(ptr_resource, resource_ptr.ptr_resource);
  }
  // reset就删除老的，指向新的
  void reset(T *resource_ptr) noexcept(false) {
    // ensure a invalid resource is not passed or program will be terminated
    if (resource_ptr == nullptr)
      throw std::invalid_argument(
          "An invalid pointer was passed, resources will not be swapped");

    delete ptr_resource;

    ptr_resource = nullptr;

    std::swap(ptr_resource, resource_ptr);
  }

public:
  // overloaded operators
  T *operator->() const noexcept { return this->ptr_resource; }
  T &operator*() const noexcept { return *this->ptr_resource; }
  // 额外说明noexcept
  // noexcept C++11关键字,
  // 告诉编译器，函数中不会发生异常,有利于编译器对程序做更多的优化
  // C++中的异常处理是在运行时而不是编译时检测的。为了实现运行时检测，编译器创建额外的代码，然而这会妨碍程序优化
};

#include "UniquePtr.h"
/**
 * 简单的类，将被智能指针使用
 * */
class Test {
public:
  Test() { std::cout << "Test class construct" << std::endl; }
  ~Test() { std::cout << "Test class destruct" << std::endl; }

  void printSomething() { std::cout << "Test printSomething " << std::endl; }

  void printResource() { std::cout << "Test printResource " << a << std::endl; }

  int getResource() { return a; }

private:
  int a = 10;
};

/**
 * 使用unique_ptr的类
 * */
class PUser {
public:
  PUser() {
    // 初始化pTest
    pTest.reset(new Test());
    std::cout << "PUser construct " << std::endl;
  }
  ~PUser() { std::cout << "PUser destruct" << std::endl; }

  // 可以在类的各种函数，使用pTest，
  void userTest() {
    std::cout << "userTest " << pTest->getResource() << std::endl;
  }

private:
  // 典型用法，在一个类中，作为一个类成员变量
  unique_ptr<Test> pTest;
};

/**
 * 主程序入口
 **/
int main(int argc, char *argv[]) {
  unique_ptr<Test> tPtr1(new Test());
  // 以下这两句话，//编译就不通过，因为已经定义， unique_ptr& operator = (const
  // unique_ptr&) noexcept = delete; unique_ptr<Test> tPtr2 = tPtr1;
  // unique_ptr<Test> tPtr3(tPtr1);

  // 以下两句话就允许，因为pPtr1做了控制权转移
  unique_ptr<Test> tPtr3(std::move(tPtr1));
  unique_ptr<Test> tPtr4 = std::move(tPtr3);

  // tPtr1->printResource();//这一句就崩溃，因为tPtr1非空，只不过资源完全不能用了
  tPtr1
      ->printSomething(); // 这一句不崩溃，tPtr1虽然资源不能用，但是代码段可以调用，只要代码段没有使用到资源

  PUser *pUser = new PUser();
  pUser->userTest();

  return 0;
}
```

