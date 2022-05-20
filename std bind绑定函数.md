# std::bind绑定函数

- 模板函数std::bind是一个函数适配器，接受一个可调用对象（callable object），生成一个新的可调用对象来“适应”原对象的参数列表。
- std::bind返回一个基于f的函数对象，其参数被绑定到args上。f的参数要么被绑定到值，要么被绑定到placeholders（占位符，如_1, _2, ..., _n）。
- std::bind将可调用对象与其参数一起进行绑定，绑定后的结果可以使用std::function保存。
- 需要`#include <functional>`。

### 1 std::bind绑定普通函数

```cpp
double callableFunc (double x, double y) {return x/y;}
auto NewCallable = std::bind (callableFunc, std::placeholders::_1,2); // std::bind
NewCallable (10);                  
```

- bind的第一个参数是函数名，**普通函数做实参时，会隐式转换成函数指针**。因此`std::bind(callableFunc,_1,2)`等价于std::bind (&callableFunc,_1,2)。
- `_1`表示占位符，std::placeholders::_1，`2`表示传的参数就是2。
- 第一个参数被占位符占用，表示这个参数以调用时传入的参数为准，在这里调用NewCallable时，给它传入了10，其实就想到于调用callableFunc(10,2);

### 2 std::bind绑定一个成员函数

```cpp
class Base
{
    void display_sum(int a1, int a2)
    {
        std::cout << a1 + a2 << '\n';
    }
 
    int m_data = 30;
};
int main() 
{
    Base base;
    auto newiFunc = std::bind(&Base::display_sum, &base, 100, std::placeholders::_1); // std::bind
    f(20); // cout 120
}
```

- bind绑定类成员函数时，**第一个参数表示对象的成员函数的指针**，**第二个参数表示对象的地址**。
- 必须显示的指定&Base::diplay_sum，因为编译器不会将对象的成员函数隐式转换成函数指针，**所以必须在Base::display_sum前添加&**；
- 使用对象成员函数的指针时，**必须要知道该指针属于哪个对象，因此第二个参数为对象的地址&base**；（所以有时在**类内调用会见到this**。）

### 3 完整举例

```cpp
class A
{
public:
    void fun_3(int k,int m)
    {
        cout<<k<<" "<<m<<endl;
    }
};

void fun(int x,int y,int z)
{
    cout<<x<<"  "<<y<<"  "<<z<<endl;
}

void fun_2(int &a,int &b)
{
    a++;
    b++;
    cout<<a<<"  "<<b<<endl;
}

int main(int argc, const char * argv[])
{
    auto f1 = std::bind(fun,1,2,3); //表示绑定函数 fun 的第一，二，三个参数值为： 1 2 3
    f1(); //print:1  2  3

    auto f2 = std::bind(fun, placeholders::_1,placeholders::_2,3);
    //表示绑定函数 fun 的第三个参数为 3，而fun 的第一，二个参数分别有调用 f2 的第一，二个参数指定
    f2(1,2);//print:1  2  3

    auto f3 = std::bind(fun,placeholders::_2,placeholders::_1,3);
    //表示绑定函数 fun 的第三个参数为 3，而fun 的第一，二个参数分别有调用 f3 的第二，一个参数指定
    //注意： f2  和  f3 的区别。
    f3(1,2);//print:2  1  3

    int n = 2;
    int m = 3;

    auto f4 = std::bind(fun_2, n,placeholders::_1); // 实际上就是传的n,m
    f4(m); //print:3  4

    cout<<m<<endl;//print:4  说明：m是引用传递，bind对于通过std::placeholders传递的参数是引用传递
    cout<<n<<endl;//print:2  说明：n是值传递，bind对于直接传递的参数是值传递，可通过std::ref或std::cref引用传递

    A a;
    auto f5 = std::bind(&A::fun_3, &a,placeholders::_1,placeholders::_2);
    f5(10,20);//print:10 20

    std::function<void(int,int)> fc = std::bind(&A::fun_3, &a,std::placeholders::_1,std::placeholders::_2);
    fc(10,20);//print:10 20

    return 0;
}
```

### 参考链接

[C++11中的std::bind 简单易懂_云飞扬_Dylan的博客-CSDN博客_c++ std::bind](https://blog.csdn.net/Jxianxu/article/details/107382049)

[C++11 中std::function和std::bind的用法_阳光柠檬_的博客-CSDN博客_stdfunction和stdbind用法](https://blog.csdn.net/liukang325/article/details/53668046?spm=1001.2101.3001.6650.2&depth_1-utm_relevant_index=5)