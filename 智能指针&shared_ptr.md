# 智能指针&shared_ptr

- 当新的 shared_ptr 对象与指针关联时，则在其构造函数中，将与此指针关联的引用计数增加1。
- 当任何 shared_ptr 对象超出作用域时，则在其析构函数中，它将关联指针的引用计数减1。如果引用计数变为0，则表示没有其他 shared_ptr 对象与此内存关联，在这种情况下，它使用delete函数删除该内存。

#### 创建 shared_ptr 对象

```cpp
std::shared_ptr<int> p1(new int());
std::shared_ptr<int> p1 = std::make_shared<int>();
std::shared_ptr<int> p1 = new int(); // 错误，因为shared_ptr内部是显示构造。
```

#### 检查 shared_ptr 对象的引用计数

```c++
p1.use_count();
```

#### 分离关联的原始指针

```c++
std::shared_ptr<int> p3 = p1; // 假如此时 p1.use_count = p3.use_count = 2。
p1.reset(); // 这种情况，p1 p1.use_count = 0（直接分离），而p3.use_count = 1，这是因为分离2个指针了，所以不共享引用计数。
// 如果没有这个p3，则p1.use_count = 0后，原始指针引用计数为0，直接会被delete。

p1.reset(new int(34)); // 这种情况，它将在内部指向新指针，p1.use_count = 1。
p1 = nullptr; // p1.use_count = 0。
```

#### shared_ptr是一个伪指针

```c++
shared_ptr充当普通指针，我们可以将*和->与 shared_ptr 对象一起使用，也可以像其他 shared_ptr 对象一样进行比较。
```

#### 完整用例

```cpp
#include <iostream>
#include  <memory> // 需要包含这个头文件

int main()
{
	// 使用 make_shared 创建空对象
	std::shared_ptr<int> p1 = std::make_shared<int>();
	*p1 = 78;
	std::cout << "p1 = " << *p1 << std::endl; // 输出78

	// 打印引用个数：1
	std::cout << "p1 Reference count = " << p1.use_count() << std::endl;

	// 第2个 shared_ptr 对象指向同一个指针
	std::shared_ptr<int> p2(p1);

	// 下面两个输出都是：2
	std::cout << "p2 Reference count = " << p2.use_count() << std::endl;
	std::cout << "p1 Reference count = " << p1.use_count() << std::endl;

	// 比较智能指针，p1 等于 p2
	if (p1 == p2) {
		std::cout << "p1 and p2 are pointing to same pointer\n";
	}

	std::cout<<"Reset p1 "<<std::endl;

	// 无参数调用reset，无关联指针，引用个数为0
	p1.reset();
	std::cout << "p1 Reference Count = " << p1.use_count() << std::endl;
	
	// 带参数调用reset，引用个数为1
	p1.reset(new int(11));
	std::cout << "p1  Reference Count = " << p1.use_count() << std::endl;

	// 把对象重置为NULL，引用计数为0
	p1 = nullptr;
	std::cout << "p1  Reference Count = " << p1.use_count() << std::endl;
	if (!p1) {
		std::cout << "p1 is NULL" << std::endl; // 输出
	}
	return 0;
}
```

### 自定义删除器 Deleter

- 当 shared_ptr 对象超出范围时，将调用其析构函数。在其析构函数中，它将引用计数减1，如果引用计数的新值为0，则删除关联的原始指针。
- 析构函数中删除内部原始指针，默认调用的是delete()函数。

#### 当 shared_ptr 对象指向数组

```c++
// 需要添加自定义删除器的使用方式
std::shared_ptr<int> p3(new int[12]);   // 不能满足要求

// 指向数组的智能指针可以使用这种形式
std::shared_ptr<int[]> p3(new int[12]);  // 正确使用方式
```

- 像这样申请的数组，应该调用delete []释放内存，而`shared_ptr<int>`析构函数中默认delete并不能满足需求。
- 可以使用`shared_ptr<int[]>`形式，或添加自定义删除器。

#### 给shared_ptr添加自定义删除器

**3种方式：Function的形式、Class的形式、Lambda表达式的形式**

```cpp
#include <memory>
using namespace std;

struct Sample
{
    Sample() {
        std::cout << "Sample\n";
    }
    ~Sample() {
        std::cout << "~Sample\n";
    }
};

void deleter(Sample * x) // Function的形式
{
    std::cout << "Function Deleter\n";
    delete[] x;
}

class Deleter // Class的形式
{
	public:
	void operator() (Sample * x) {
		std::cout<<"Class Deleter\n";
		delete[] x;
	}
};

int main()
{
    // Function的形式，作为删除器
    std::shared_ptr<Sample> p2(new Sample[3], deleter);
	std::cout << "-------------------------------------\n";
	// Class的形式，作为删除器
	std::shared_ptr<Sample> p3(new Sample[3], Deleter());
	std::cout << "-------------------------------------\n";
	// Lambda表达式的形式，作为删除器
	std::shared_ptr<Sample> p4(new Sample[3], [](Sample * x){
		std::cout<<"Lambda Deleter\n";
			delete[] x;
	});
	std::cout << "-------------------------------------\n";
    return 0;
}

// 输出结果：
Sample
Sample
Sample
-------------------------------------
Sample
Sample
Sample
-------------------------------------
Sample
Sample
Sample
-------------------------------------
Lambda Deleter
~Sample
~Sample
~Sample
Class Deleter
~Sample
~Sample
~Sample
Function Deleter
~Sample
~Sample
~Sample
```

#### shared_ptr 相对于普通指针的优缺点

- 与普通指针相比，shared_ptr仅提供-> 、*和==运算符，没有+、-、++、--、[]等运算符。

```cpp
#include<iostream>
#include<memory>

struct Sample {
	void dummyFunction() {
		std::cout << "dummyFunction" << std::endl;
	}
};

int main()
{
	std::shared_ptr<Sample> ptr = std::make_shared<Sample>();

	(*ptr).dummyFunction(); // 正常
	ptr->dummyFunction(); // 正常

	// ptr[0]->dummyFunction(); // 错误方式
	// ptr++;  // 错误方式
	// ptr--;  // 错误方式

	std::shared_ptr<Sample> ptr2(ptr);
	if (ptr == ptr2) // 正常
		std::cout << "ptr and ptr2 are equal" << std::endl;
	return 0;
}
```

#### 不要使用同一个原始指针多次构造 shared_ptr

- 创建多个 shared_ptr 的正常方法是使用一个已存在的shared_ptr 进行创建，而不是使用同一个原始指针进行创建。

```cpp
int *num = new int(23);
std::shared_ptr<int> p1(num);
std::shared_ptr<int> p2(p1);  // 正确使用方法
std::shared_ptr<int> p3(num); // 不推荐

std::cout << "p1 Reference = " << p1.use_count() << std::endl; // 输出 2
std::cout << "p2 Reference = " << p2.use_count() << std::endl; // 输出 2
std::cout << "p3 Reference = " << p3.use_count() << std::endl; // 输出 1

// 注意：离开作用域会报错，因为p1/p2释放了num，此时num成了悬空指针，而p3又是单独的引用计数，又会去释放一遍，因此会报错。
```

#### 不要用栈中的指针构造 shared_ptr 对象

- shared_ptr 默认的构造函数中使用的是delete来删除关联的指针，所以构造的时候也必须使用**new出来的堆空间的指针**。

```cpp
int main()
{
   int x = 12;
   std::shared_ptr<int> ptr(&x);
   return 0;
}

// 注意：当 shared_ptr 对象超出作用域调用析构函数delete，指针&x时会出错。因为用的是delete，栈上的指针不能用，只能堆上。
```

#### 建议使用 make_shared

- 为了避免以上两种情形，建议使用`make_shared()<>`创建 shared_ptr 对象，而不是使用默认构造函数创建。

```cpp
std::shared_ptr<int> ptr_1 = make_shared<int>();
std::shared_ptr<int> ptr_2(ptr_1);
```

- 另外不建议使用get()函数获取 shared_ptr 关联的原始指针，因为如果在 shared_ptr 析构之前手动调用了delete函数，同样会导致类似的错误。

#### C++新经典--shared_ptr常用操作

![image-20221017092808245](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017092808245.png)

![image-20221017092834018](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017092834018.png)

![image-20221017092903131](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017092903131.png)

![image-20221017092919843](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017092919843.png)

![image-20221017092941491](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017092941491.png)

![image-20221017093000348](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093000348.png)

#### C++新经典--shared_ptr容易踩坑点

![image-20221017093503466](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093503466.png)

![image-20221017093525922](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221017093525922.png)

### 参考链接

[(18条消息) C++ 智能指针 shared_ptr 详解与示例_码农小明的博客-CSDN博客_shared_ptr](https://blog.csdn.net/shaosunrise/article/details/85228823)