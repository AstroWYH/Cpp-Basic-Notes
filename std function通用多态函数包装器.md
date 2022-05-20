# std::function通用多态函数包装器

- 类模板std::function是一种通用、多态的函数封装。
- std::function的实例可以对任何普通函数、Lambda表达式、函数指针、函数对象（仿函数）的目标实体进行存储、复制、和调用操作。
- std::function对象是对C++中现有的可调用实体的一种类型安全的包裹（我们知道像函数指针这类可调用实体，是类型不安全的）。
- 可以将std::function近似理解成函数指针。
- 需要`#include <functional>`。

### 包装普通函数

```cpp
int g_Minus(int i, int j)
{
    return i - j;
}

int main()
{
    function<int(int, int)> f = g_Minus;
    cout << f(1, 2) << endl;  // -1
    return 1;
}
```

### 包装模板函数

```cpp
template <class T>
T g_Minus(T i, T j)
{
    return i - j;
}

int main()
{
    function<int(int, int)> f = g_Minus<int>;
    cout << f(1, 2) << endl; // -1
    return 1;
}
```

### 包装lambda表达式

```cpp
auto g_Minus = [](int i, int j){ return i - j; };

int main()
{
    function<int(int, int)> f = g_Minus;
    cout << f(1, 2) << endl; // -1
    return 1;
}
```

### 包装函数对象（仿函数）

```cpp
// 非模板类型
struct Minus
{
    int operator() (int i, int j)
    {
        return i - j;
    }
};

int main()
{
    function<int(int, int)> f = Minus();
    cout << f(1, 2) << endl;                                            // -1
    return 1;
}
// 模板类型
template <class T>
struct Minus
{
    T operator() (T i, T j)
    {
        return i - j;
    }
};

int main()
{
    function<int(int, int)> f = Minus<int>();
    cout << f(1, 2) << endl;                                            // -1
    return 1;
}
```

### 包装类静态成员函数

```cpp
// 非模板类型
class Math
{
public:
    static int Minus(int i, int j)
    {
        return i - j;
    }
};

int main()
{
    function<int(int, int)> f = &Math::Minus;
    cout << f(1, 2) << endl;                                            // -1
    return 1;
}
// 模板类型
class Math
{
public:
    template <class T>
    static T Minus(T i, T j)
    {
        return i - j;
    }
};

int main()
{
    function<int(int, int)> f = &Math::Minus<int>;
    cout << f(1, 2) << endl;                                            // -1
    return 1;
}
```

### 包装类对象成员函数

```cpp
// 非模板类型
class Math
{
public:
    int Minus(int i, int j)
    {
        return i - j;
    }
};

int main()
{
    Math m;
    function<int(int, int)> f = bind(&Math::Minus, &m, placeholders::_1, placeholders::_2); // 注意类对象成员函数的形式。
    cout << f(1, 2) << endl;                                            // -1
    return 1;
}
// 模板类型
class Math
{
public:
    template <class T>
    T Minus(T i, T j)
    {
        return i - j;
    }
};

int main()
{
    Math m;
    function<int(int, int)> f = bind(&Math::Minus<int>, &m, placeholders::_1, placeholders::_2);
    cout << f(1, 2) << endl;                                            // -1
    return 1;
}
```

### 完整举例

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;

//声明一个模板
typedef std::function<int(int)> Functional;

//normal function
int TestFunc(int a)
{
    return a;
}

//lambda expression
auto lambda = [](int a)->int{return a;};

//functor仿函数
class Functor
{
public:
    int operator() (int a)
    {
        return a;
    }
};

//类的成员函数和类的静态成员函数
class CTest
{
public:
    int Func(int a)
    {
        return a;
    }
    static int SFunc(int a)
    {
        return a;
    }
};

int main(int argc, char* argv[])
{
    //封装普通函数
    Functional obj = TestFunc;
    int res = obj(0);
    cout << "normal function : " << res << endl;
 
    //封装lambda表达式
    obj = lambda;
    res = obj(1);
    cout << "lambda expression : " << res << endl;
 
    //封装仿函数
    Functor functorObj;
    obj = functorObj;
    res = obj(2);
    cout << "functor : " << res << endl;
 
    //封装类的成员函数和static成员函数
    CTest t;
    obj = std::bind(&CTest::Func, &t, std::placeholders::_1);
    res = obj(3);
    cout << "member function : " << res << endl;
 
    obj = CTest::SFunc;
    res = obj(4);
    cout << "static member function : " << res << endl;
 
    return 0;
}
```

### 参考链接

[std::function详解_在座的各位都是高手的博客-CSDN博客_std::function<>](https://blog.csdn.net/weixin_44378800/article/details/115210731)

[std::function的用法_zeqi1991的博客-CSDN博客](https://blog.csdn.net/zeqi1991/article/details/73727458?spm=1001.2101.3001.6650.1&depth_1-utm_relevant_index=2)

[C++11 中std::function和std::bind的用法_阳光柠檬_的博客-CSDN博客_stdfunction和stdbind用法](https://blog.csdn.net/liukang325/article/details/53668046?spm=1001.2101.3001.6650.2&depth_1-utm_relevant_index=5)

[C++11中的std::bind 简单易懂_云飞扬_Dylan的博客-CSDN博客_c++ std::bind](https://blog.csdn.net/Jxianxu/article/details/107382049)