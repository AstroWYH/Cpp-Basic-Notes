# lambda表达式

- 用于创建匿名的函数对象，用于简化编程工作。

- 通常是在**需要一个函数，但是又不想费神去命名一个函数**的场合下使用，也就是指**匿名函数。**

### 1  函数对象参数

​    标示一个Lambda表达式的开始，必须存在，不能省略。函数参数对象只能使用那些定义Lambda为止时Lambda所在作用范围内可见的局部变量（包含Lambda所在类的this）。

```
(1)[]:没有任何函数参数对象
(2)[=]:以值传递方式捕获Lambda所在范围内的所有局部变量。
(3)[&]:以引用传递方法捕获Lambda所在范围内的所有局部变量。
(4)this:函数体可以使用Lambda所在类的成员变量。
(5)[x,&y]:x以值传递捕获，y以引用形式捕获。
(6)[=,&z]:z以引用形式捕获，其他变量以值传递捕获。
(7)[&,x]:x以值传递形式捕获，其他变量以引用形式捕获。
```

### 2 函数参数

参数列表，标识重载的()操作符，没有参数时，可以省略。参数可以使用值传递和引用传递。

### 3 返回值类型

 标识函数返回值的类型，当返回值为void，或者函数体中只有一处return的地方时，这部分可以省略。

### 4 函数体

标识函数的实现，不能省略，函数体可以为空。

```cpp
auto foo = [=](int x,int y)->bool {return true;};
[&](int x,int y)->int {return x+y;} 
[](int x,int y) {return x+y;} // 隐式返回类型
[](int& x) {++x;} // 没有return语句，Lambda的函数返回类型为 'void'
[]() {++num} // 没有参数，访问全局变量num
[] {++num} // 省略参数
```

### 示例1

```cpp
bool cmp(int &a, int &b) // 比较函数cmp
{
    ruturn a > b;
}
int main()
{
    vector<int> data;
    for (int i = 0; i < 5; i++)
        data.push_back(i);
    sort(data.begin(), data.end(), cmp); // 排序，使用cmp
    for (int i = 0; i < data.size(); i++)
        cout << data[i] << endl;
    return 0;
}
```

如果想用多个比较函数，则不能再重复命名为cmp，并且麻烦，此时用lambda表达式较好。

```cpp
int main()
{
    vector<int> data;
    for (int i = 0; i < 5; i++)
        data.push_back(i);
    sort(data.begin(), data.end(), [](int &a,int &b)->bool // 用lambda表达式的形式，写cmp。其中a、b在sort内部会用到。
    {
        return a>b;
    });
    for (int i = 0; i < data.size(); i++)
        cout << data[i] << endl;
    return 0;
}
```

### 示例2

```cpp
int main()
{
    vector<int> data;
    vector<int> result;
    for (int i = 0; i < 5; i++)
        data.push_back(i);
    for_each(data.begin(), data.end(), [&result](int &elem) { // 用lambda表达式，筛选数组的偶数。其中elem在foreach内部会用到
        if (elem % 2 == 0)
        {
            retult.push_back(elem);
        }
    });
    for_each(result.begin(), result.end(), [](int &elem) { // 用lambda表达式，打印结果
        cout << elem << endl;
    });
    return 0;
}
```

### 参考链接

[(18条消息) C++ Lambda表达式详解_酷小川的博客-CSDN博客_c++ lambda表达式](https://blog.csdn.net/kksc1099054857/article/details/106816687/)