# C static&C++ static

## C static

- 栈区： 由编译器自动分配释放，像局部变量，函数参数，都是在栈区。会随着作用于退出而释放空间。
- 堆区：程序员分配并释放的区域，像malloc(c)，new(c++) 。
- 全局数据区(静态区)：全局变量和静态便令的存储是放在一块的，初始化的全局变量和静态变量在一块区域，未初始化的全局变量和未初始化的静态变量在相邻的另一块区域，程序结束释放。
- 代码区：存放程序的代码，即CPU执行的机器指令，只读。

**1 静态局部变量**：用于函数体内部修饰变量，这种变量的生存期长于该函数。

```c++
int foo() {
	static int i = 1; // note:1 // 编译时在全局数据区(静态区)分配内存，初次执行到此处初始化，后续不再初始化，可以一直累加。程序结束才释放。
	//int i = 1;  // note:2 // 每次运行到此处，在栈上重新创建初始化，离开作用域释放。
	i += 1;
	return i;
}
```

1. 该变量在**全局数据区**分配内存(局部变量在栈区分配内存)；
2. 静态局部变量在程序**执行到该对象的声明处时被首次初始化**，即**以后的函数调用不再进行初始化**(局部变量每次函数调用都会被初始化)；
3. 静态局部变量一般在声明处初始化，**如果没有显式初始化，会被程序自动初始化为0**(局部变量不会被初始化)；
4. 静态局部变量**始终驻留在全局数据区，直到程序运行结束**。但其**作用域为局部作用域**，也就是**不能在函数体外面使用它**(局部变量在栈区，在函数结束后立即释放内存)。

**2 静态全局变量**：定义在函数体外，用于修饰全局变量，表示该变量只在本文件可见。

```c++
static int i = 1;  //note:3 // 文件隔离，静态全局变量，仅当前cpp可见。
//int i = 1;  //note:4 // 全局变量，其他cpp也可见。

int foo() {
	i += 1;
	return i;
}
```

假设我有一个文件a.c，我们再新建一个b.c，内容如下。

```c++
//file a.c
//static int n = 15;  //note:5 // 静态全局变量，undeference to "n"，找不到符号定义，文件隔离。
int n = 15;  //note:6 // 全局变量，可以正常调用。

//file b.c
extern int n; // 用extern声明其他文件的n，供本cpp使用。
void fn() {
	n++;
	printf("after: %d\n",n);
}

void main() {
	printf("before: %d\n",n);
	fn();
}
```

**3 静态全局函数**：和静态全局变量类似。

```cpp
//file a.c
void fn() {
	printf("this is non-static func in a"); // 全局函数，可以正常调用。如果加上static，找不到符号定义，文件隔离。
}

//file b.c
extern void fn();  //我们用extern声明其他文件的fn()，供本cpp使用。
void main() {
    fn();
}
```

## C++ static

除了C语言的3种用法，还有额外的2种。

**1 静态数据成员**：用于修饰 class 的数据成员，即所谓“静态成员”。这种数据成员的生存期大于 class 的对象（实体 instance）。静态数据成员是每个 class 有一份，普通数据成员是每个 instance 有一份，因此**静态数据成员**也叫做**类变量**，而**普通数据成员**也叫做**实例变量**。

```cpp
class Rectangle {
private:
	int m_w,m_h;
	static int s_sum;

public:
	Rectangle(int w,int h) {
		this->m_w = w;
		this->m_h = h;
		s_sum += (this->m_w * this->m_h);
	}

	void GetSum() {
		cout<<"sum = "<<s_sum<<endl;
	}
};

int Rectangle::s_sum = 0; // 静态成员(类变量)初始化，需要在类外定义，注意格式。

int main() {
	cout<<"sizeof(Rectangle)="<<sizeof(Rectangle)<<endl;
	Rectangle *rect1 = new Rectangle(3,4);
	rect1->GetSum();
	cout<<"sizeof(rect1)="<<sizeof(*rect1)<<endl;
	Rectangle rect2(2,3);
	rect2.GetSum();
	cout<<"sizeof(rect2)="<<sizeof(rect2)<<endl;
	return 0;
}
```

![image-20220510151854958](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220510151854958.png)

由图可知：sizeof(Rectangle)=8bytes=sizeof(m_w)+sizeof(m_h)。也就是说**static静态成员(类变量)并不占用Rectangle的内存空间。**
那么static在哪里分配内存的呢？是的，**全局数据区(静态区)**。
再看看GetSum()，第一次`12=3*4`，第二次`18=12+2*3`。由此可得，static只会被初始化一次，于实例无关。

1. 对于非静态数据成员，每个类对象(实例)都有自己的拷贝。
2. 静态数据成员被当作是类的成员，由该类型的所有对象共享访问,对该类的多个对象来说，**静态数据成员只分配一次内存**。
   静态数据成员存储在**全局数据区**。
3. 静态数据成员定义时要分配空间，**所以不能在类声明中定义，注意类外定义的格式。**
4. 也就是说，你每new一个Rectangle，并不会为static int s_sum的构建一份内存拷贝，它是不管你new了多少Rectangle的实例，因为它只与类Rectangle挂钩，而跟你每一个Rectangle的对象没关系。

**2 静态成员函数**：用于修饰 class 的成员函数。

```c++
class Rectangle {
private:
	int m_w,m_h;
	static int s_sum;

public:
	Rectangle(int w,int h) {
		this->m_w = w;
		this->m_h = h;
		s_sum += (this->m_w * this->m_h);
	}

	static void GetSum() {  //这里加上static 
		cout<<"sum = "<<s_sum<<endl;
	}
};

int Rectangle::s_sum = 0;  //静态成员(类变量)初始化，需要在类外定义，注意格式

int main() {
	cout<<"sizeof(Rectangle)="<<sizeof(Rectangle)<<endl;
	Rectangle *rect1 = new Rectangle(3,4);
	rect1->GetSum();
	cout<<"sizeof(rect1)="<<sizeof(*rect1)<<endl;
	Rectangle rect2(2,3);
	rect2.GetSum();  //可以用对象名.函数名访问
	cout<<"sizeof(rect2)="<<sizeof(rect2)<<endl;
	Rectangle::GetSum();  //也可以可以用类名::函数名访问
	return 0;
}
```

1. **静态成员之间可以相互访问**，包括静态成员函数访问静态数据成员和访问静态成员函数。
2. **非静态**成员函数**可以任意地访问静态**成员函数和静态数据成员。**（非静态->静态）**
3. **静态**成员函数**不能访问非静态**成员函数和非静态数据成员。
4. **调用静态成员函数**，可以用成员访问**操作符(.)和(->)**为一个类的对象或指向类对象的指针调用静态成员函数,也可以用**类名::函数名**调用(因为他本来就是属于类的，用类名调用很正常)。

## 全局变量&static变量的初始化

1. static变量、全局变量一样，数据都存放在**全局数据区(静态区)**内存分布情况，所以在主程序之前，**编译器已经为其分配好了内存**。
2. C中，初始化发生在代码执行之前，编译阶段分配好内存之后，就会进行初始化。
3. C++中，初始化时在执行相关代码时才会进行初始化，主要是由于C++引入对象后，要进行初始化必须执行相应构造函数和析构函数，在构造函数或析构函数中经常会需要进行某些程序中需要进行的特定操作，并非简单地分配内存。

### 参考链接

[(19条消息) 全局变量什么时候被初始化，在进入main函数之前吗？_学无止境2022的博客-CSDN博客_全局变量什么时候初始化](https://blog.csdn.net/dianqicyuyan/article/details/122956111?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)

[(19条消息) c++中static的用法详解_码农在新加坡的博客-CSDN博客_c++ static](https://blog.csdn.net/majianfei1023/article/details/45290467)