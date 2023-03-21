## C++ static变量的初始化时机

### 1. 编译时初始化

如果静态变量本身是**基本数据类型**，且初始化值是**常量**，并且是**全局变量**，那么这个初始化过程是在编译期间完成的。

```cpp
static int a = 10;
static char strArray[] = "hello! world";
```

### 2. 加载时初始化（刚运行，main函数之前）

**程序被加载时立即进行的初始化**。这个初始化发生在main函数之前。即使程序任何地方都没访问过该变量，仍然会进行初始化，因此形象地称之为"**饿汉式初始化**"。

（1）静态变量是一个基本数据类型，但是初始值**不是常量**。

```cpp
static int *p = new int[1024];

int x = 3;
int y = 4;
static int z = x + y;
```

（2）静态变量是一个**类对象**，这种情况下即使是使用常量初始化，也是加载时初始化。

```cpp
static std::string str = "Hello world !"; // string是一个类

class MyClass {
public:	
	MyClass();    
	MyClass(int a, int b);
};

static MyClass* MyClass1 = new MyClass(); // MyClass也是一个类
static MyClass MyClass2;
```

### 3. 运行时初始化

- 这个初始化发生**在变量第一次被引用**。
- 也就是说，从程序执行模型角度看，程序所在进程空间中，哪个线程先访问了这个变量，就是哪个线程来初始化这个变量。因此，相对于加载初始化来说，这种初始化是把真正的初始化动作推迟到第一次被访问时，因而形象地称为"**懒汉式初始化**"。

```cpp
int myfunc()
{     	
    static std::string msg = "hello world !";    //运行时初始化
}
```

### C++的static类成员变量为什么一定要初始化

- 其实这句话“静态成员变量是需要初始化的”是有一定问题的，应该说“静态成员变量需要定义”才是准确的，而不是初始化。
- 两者的区别在于：初始化是赋一个初始值，而定义是分配内存。
- 静态成员变量在类中仅仅是声明，没有定义，所以要在类的外面定义，实际上是给静态成员变量分配内存。
- 在类外定义（就是那个奇怪写法，某些情况不用在类外定义）的原因是，如果在类内定义，就有点每个类的实例都有一份类成员变量的意思，这样不对。

```cpp
//test.cpp 
#include <stdio.h> 
class A { 
    public: 
        static int a; //声明但未定义
 }; 
int main() { 
    printf("%d", A::a);
    return 0;
}
```

编译以上代码会出现“对‘A::a’未定义的引用”错误。这是因为静态成员变量a未定义，也就是还没有分配内存，显然是不可以访问的。

```cpp
//test.cpp 
#include <stdio.h> 
class A { 
    public: 
        static int a; //声明但未定义
 }; 
int A::a = 3; // wyh 定义了静态成员变量，同时初始化。也可以写"int A:a;"，即不给初值，同样可以通过编译
int main() { 
    printf("%d", A::a);
    return 0;
}
```

这样就对了，因为给a分配了内存，所以可以访问静态成员变量a了。



### 参考链接

[(19条消息) C++中static变量的初始化_LikeMarch的博客-CSDN博客_静态变量的初始值](https://blog.csdn.net/m0_37433111/article/details/115342523#1__5)

[(19条消息) 关于C++类的静态数据为什么一定要初始化_weixin_34375251的博客-CSDN博客](https://blog.csdn.net/weixin_34375251/article/details/92131943?spm=1001.2101.3001.6650.15&depth_1-utm_relevant_index=22)