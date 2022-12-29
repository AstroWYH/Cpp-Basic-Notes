# 智能指针&unique_ptr

#### unique_ptr 独享所有权

- unique_ptr对象始终是关联的原始指针的唯一所有者。
- 我们无法复制unique_ptr对象，它只能移动。
- 由于每个unique_ptr对象都是原始指针的唯一所有者，因此在其析构函数中它直接删除关联的指针，不需要任何参考计数。

#### 创建 unique_ptr 对象

```cpp
std::unique_ptr<int> ptr1;
std::unique_ptr<Task> taskPtr(new Task(22));
std::unique_ptr<Task> taskPtr = std::make_unique<Task>(34); // C++ 14 引入
std::unique_ptr<Task> taskPtr(new std::unique_ptr<Task>::element_type(23)); // 不常用
std::unique_ptr<Task> taskPtr2 = new Task(); // 编译错误，不能隐式构造，和shared_ptr一样
// 有几点需要注意：
// 第2行是完全可以理解的，因为unique_ptr有一个构造函数，其所需参数为裸指针。
// 第5行，如果等号左右两边是一样的类，那要么是拷贝构造，要么是拷贝赋值运算符，取决于写一行还是两行。
// 但如果等号左右不同，则需要先进行隐式转换，再进行拷贝（构造或赋值）。此时，如果其本身构造函数不允许隐式转换，则编译错误。
```

#### 获取被管理对象的指针

```cpp
Task *p1 = taskPtr.get();
```

#### 重置 unique_ptr 对象

```cpp
taskPtr.reset(); // delete原始关联指针，并taskPtr = nullptr置空，不需要考虑引用计数之类的。
```

#### unique_ptr 对象不可复制

```cpp
// 编译错误 : unique_ptr 不能复制
std::unique_ptr<Task> taskPtr3 = taskPtr2; // 拷贝构造函数，Compile error
// 编译错误 : unique_ptr 不能复制
taskPtr = taskPtr2; // 拷贝赋值运算符，compile error
```

#### 转移 unique_ptr 对象的所有权

![image-20220513185056558](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220513185056558.png)

```cpp
// 通过原始指针创建 taskPtr2
std::unique_ptr<Task> taskPtr2(new Task(55));
// 通过std::move转移给 taskPtr4
std::unique_ptr<Task> taskPtr4 = std::move(taskPtr2);
// 现在taskPtr2 = nullptr

// std::move() 将把 taskPtr2 转换为一个右值引用。然后调用 unique_ptr 的移动构造函数，并将关联的原始指针传输到 taskPtr4。
// 在转移完原始指针的所有权后， taskPtr2将变为空。
```

#### 可以返回unique_ptr

```cpp
unique_ptr<int> clone(int p) {
    unique_ptr<int> pInt(new int(p));
    return pInt;    // 返回unique_ptr
}

int main() {
    int p = 5;
    unique_ptr<int> ret = clone(p); // 其实和std::move转成右值后移动是一样的，函数返回的也是右值，可以调用移动构造函数。
    cout << *ret << endl;
}
```

#### 释放关联的原始指针

- 在 unique_ptr 对象上调用 release()将释放其关联的原始指针的所有权，并返回原始指针。
- **这里是释放所有权，并没有delete原始指针，reset()会delete原始指针。**

```cpp
std::unique_ptr<Task> taskPtr5(new Task(55));
// 释放关联指针的所有权
Task* ptr = taskPtr5.release(); // 返回原始指针到ptr
// 现在taskPtr5为空
if(taskPtr5 == nullptr) std::cout<<"taskPtr5 is empty"<<std::endl;
```

#### reset() & release() & get()对比

- reset()	    重置unique_ptr为空，delete其关联的指针。
- release()	返回关联指针，并将unique_ptr置空。（不delete关联指针，但取消关联指针的所有权）。
- get()	       仅仅返回关联指针。

```cpp
// unique_ptr::reset example
#include <iostream>
#include <memory>

int main () {
  std::unique_ptr<int> up;  // empty

  up.reset(new int);       // takes ownership of pointer
  *up=5;
  std::cout << *up << '\n';

  up.reset(new int);       // deletes managed object, acquires new pointer
  *up=10;
  std::cout << *up << '\n';

  up.reset();               // deletes managed object

  return 0;
}

Output:
5
10
```

#### C++新经典--unique_ptr常用操作

![image-20221017093651848](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093651848.png)

![image-20221017093719335](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093719335.png)

![image-20221017093808945](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093808945.png)

![image-20221017093833663](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093833663.png)

![image-20221017093900095](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093900095.png)

### 参考链接

[(18条消息) 【C++11】 之 std::unique_ptr 详解_Amelie_xiao的博客-CSDN博客_c++ std::unique_ptr](https://blog.csdn.net/lemonxiaoxiao/article/details/108603916?spm=1001.2101.3001.6650.2&depth_1-utm_relevant_index=5)

[(18条消息) C++ 智能指针 unique_ptr 详解与示例_码农小明的博客-CSDN博客_c++ unique_ptr](https://blog.csdn.net/shaosunrise/article/details/85158249?spm=1001.2101.3001.6650.10&depth_1-utm_relevant_index=14)