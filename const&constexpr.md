# const&constexpr

- const（**只读**）：我承诺不改变这个值，主要用于说明接口，这样变量传递给函数就不担心变量会在函数内被修改了编译器负责确认并执行const的承诺。const只保证了运行时不直接被修改（但这个东西仍然可能是个动态变量）。

- constexpr（**常量**）：在编译时求值，主要用于说明常量，作用是允许数据置于只读内存以及提升性能。constexpr表示这玩意儿在编译期就可以算出来（前提是为了算出它所依赖的东西也是在编译期可以算出来的）。constexpr表达式是指值不会改变并且在编译过程就能得到计算结果的表达式。成员函数或变量标记为constexpr，则其也顺带被标记为const，反之则不成立。

```c++
constexpr int mf = 20;  // 20是常量表达式
constexpr int limit = mf + 1; // mf + 1是常量表达式
constexpr int sz = size(); // 之后当size是一个constexpr函数时才是一条正确的声明语句
```

### 修饰对象

- const并未区分编译期常量和运行期常量。

- constexpr限定在了编译期常量。（更严格）。


### 修饰函数

- constexpr修饰的函数，简单的来说，如果其传入的参数可以在编译时期计算出来，那么这个函数就会产生编译时期的值。但是，传入的参数如果不能在编译时期计算出来，那么constexpr修饰的函数就和普通函数一样了。不过，我们不必因此而写两个版本，所以如果函数体适用于constexpr函数的条件，可以尽量加上constexpr。
- constexpr修饰的函数，返回类型和所有形参类型都是字⾯值类型，函数体有且只有⼀条return语句。

```c++
constexpr int new() {return 42};
```

const只能用于非静态成员的函数（静态成员函数并不包含this指针，也就不存在const来修饰他了）而不是所有函数。它保证成员函数不修改任何非静态数据。

### 指针常量&常量指针

在constexpr声明中如果定义了一个指针，限定符conxtexpr仅对指针有效，与指针所指的对象无关。

```c++
const int *p = nullptr;        // p是一个指向整形常量的指针，所指向物不能变，底层const，指针常量
int* const p = nullptr;        // p本身是常量指针，自己不能变，顶层const，常量指针
constexpr int* q = nullptr;    // p本身是常量指针，自己不能变，顶层const，常量指针
```

GNU gcc编译器时，constexpr指针所指变量必须是全局变量 或 static变量(既存储在静态数据区的变量)。

### 构造函数

- const不能修饰构造函数，但字⾯值常量类的构造函数可以是constexpr。
- constexpr构造函数必须有⼀个空的函数体，即所有成员变量的初始化都放到初始化列表中。 对象调⽤的成员函数必须使⽤ constexpr 修饰。

### constexpr的好处

- 为⼀些不能修改数据提供保障，写成变量则就有被意外修改的⻛险。 
- 有些场景，编译器可以在编译期对constexpr的代码进⾏优化，提⾼效率。
- ⽐宏来说，没有额外的开销，但更安全可靠。

### mutable的用法

```cpp
class A
{
public:
	A(int a):m_a(a){}
	void matest()const;
	void macout()const
	{
		cout << m_a << endl;
	}
private:
	mutable int m_a; // 在定义时用mutable来突破这层限制
};

void A::matest() const // 正常情况下，成员函数被const修饰，则函数内不能修改成员变量
{
	m_a = 10; // 现在就可以修改了
	cout << m_a << endl;
}
```

### 参考链接

[(19条消息) 关于constexpr与const_Memaxwell的博客-CSDN博客_const和constexpr](https://blog.csdn.net/qq_22274565/article/details/78719951?spm=1001.2101.3001.6650.11&depth_1-utm_relevant_index=17)

[(9 封私信 / 50 条消息) C++ const 和 constexpr 的区别？ - 知乎 (zhihu.com)](https://www.zhihu.com/question/35614219)

[(19条消息) C++之constexpr详解_云洞的博客-CSDN博客_constexpr](https://blog.csdn.net/janeqi1987/article/details/103542802?spm=1001.2101.3001.6650.2&depth_1-utm_relevant_index=5)

[(18条消息) C++中mutable的用法_Slow-Down的博客-CSDN博客_c++ mutable](https://blog.csdn.net/qq_43509546/article/details/110386037)