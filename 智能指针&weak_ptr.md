# 智能指针&weak_ptr

1. weak_ptr虽然是一个模板类，但是**不能用来直接定义指向原始指针的对象**。
2. weak_ptr接受shared_ptr类型的变量赋值，但是**反过来是行不通**的，需要使用**lock函数**。
3. weak_ptr设计之初就是为了服务于shared_ptr的（用于**解决循环引用**），所以**不增加引用计数**就是它的核心功能。
4. 由于不知道什么之后weak_ptr所指向的对象就会被析构掉，所以使用之前请**先使用expired函数检测一下**。
5. 主要有lock、swap、reset、expired、operator=、use_count几个函数，与shared_ptr相比多了lock、expired函数，但是却少了get函数，甚至连operator* 和 operator->都没有。

### 不会增加引用计数

```cpp
void test1()
{
    // weak_ptr<CA> ptr_1(new CA()); // 报错，不能直接用weak_ptr接管原始指针
    // shared_ptr<CA> ptr_3 = wk_ptr; // 报错，不能用weak_ptr给shared_ptr赋值，除非用lock()；反过来可以

    shared_ptr<CA> ptr_1(new CA());

    cout << "ptr_1 use count : " << ptr_1.use_count() << endl; // 输出：ptr_1 use count : 1

    shared_ptr<CA> ptr_2 = ptr_1;

    cout << "ptr_1 use count : " << ptr_1.use_count() << endl; // 输出：ptr_1 use count : 2
    cout << "ptr_2 use count : " << ptr_2.use_count() << endl; // 输出：ptr_1 use count : 2

    weak_ptr<CA> wk_ptr = ptr_1;

    cout << "ptr_1 use count : " << ptr_1.use_count() << endl; // 输出：ptr_1 use count : 2
    cout << "ptr_2 use count : " << ptr_2.use_count() << endl; // 输出：ptr_1 use count : 2 // 引用计数仍然为2
}
```

### 解决循环引用问题

```cpp
class CB;
class CA
{
public:
    CA() { cout << "CA() called! " << endl; }
    ~CA() { cout << "~CA() called! " << endl; }
    void set_ptr(shared_ptr<CB>& ptr) { m_ptr_b = ptr; }
    void b_use_count() { cout << "b use count : " << m_ptr_b.use_count() << endl; }
    void show() { cout << "this is class CA!" << endl; }
private:
    shared_ptr<CB> m_ptr_b; // 类CA装着类CB的shared_ptr
};

class CB
{
public:
    CB() { cout << "CB() called! " << endl; }
    ~CB() { cout << "~CB() called! " << endl; }
    void set_ptr(shared_ptr<CA>& ptr) { m_ptr_a = ptr; }
    void a_use_count() { cout << "a use count : " << m_ptr_a.use_count() << endl; }
    void show() { cout << "this is class CB!" << endl; }
private:
    shared_ptr<CA> m_ptr_a; // 类CB装着类CA的shared_ptr
};

void test_refer_to_each_other()
{
    shared_ptr<CA> ptr_a(new CA());
    shared_ptr<CB> ptr_b(new CB());

    cout << "a use count : " << ptr_a.use_count() << endl;
    cout << "b use count : " << ptr_b.use_count() << endl;

    ptr_a->set_ptr(ptr_b); // 给各自内部的对方的shared_ptr赋值
    ptr_b->set_ptr(ptr_a);

    cout << "a use count : " << ptr_a.use_count() << endl;
    cout << "b use count : " << ptr_b.use_count() << endl;
}
// 测试结果
CA() called!
CB() called!
a use count : 1
b use count : 1
a use count : 2
b use count : 2
```

通过结果可以看到，最后`CA`和`CB`的对象并没有被析构。

起初定义完ptr_a和ptr_b时，只有①③两条引用，然后调用函数set_ptr后又增加了②④两条引用，当test_refer_to_each_other这个函数返回时，对象ptr_a和ptr_b被销毁，也就是①③两条引用会被断开，但是②④两条引用依然存在，每一个的引用计数都不为0，结果就导致其指向的内部对象无法析构，造成内存泄漏。

![image-20220516113854734](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220516113854734.png)

解决这种状况的办法就是将两个类中的一个成员变量改为weak_ptr对象，因为weak_ptr不会增加引用计数，使得引用形不成环，最后就可以正常的释放内部的对象，不会造成内存泄漏，比如**将CB中的成员变量改为weak_ptr对象**。

```cpp
class CB
{
public:
    CB() { cout << "CB() called! " << endl; }
    ~CB() { cout << "~CB() called! " << endl; }
    void set_ptr(shared_ptr<CA>& ptr) { m_ptr_a = ptr; }
    void a_use_count() { cout << "a use count : " << m_ptr_a.use_count() << endl; }
    void show() { cout << "this is class CB!" << endl; }
private:
    weak_ptr<CA> m_ptr_a; // 将CB中的类CA的shared_ptr成员变量，改为weak_ptr对象
};

// 测试结果
CA() called!
CB() called!
a use count : 1
b use count : 1
a use count : 1
b use count : 2
~CA() called!
~CB() called!
```

通过这次结果可以看到，CA和CB的对象都被正常的析构了，引用关系如下图所示，流程与上一例子相似，但是不同的是④这条引用是通过weak_ptr建立的，并不会增加引用计数，也就是说CA的对象只有1个引用计数，而CB的对象只有2个引用计数，当test_refer_to_each_other这个函数返回时，对象ptr_a和ptr_b被销毁，也就是①③两条引用会被断开，此时CA对象的引用计数会减为0，对象被销毁，其内部的m_ptr_b成员变量也会被析构，导致CB对象的引用计数会减为0，对象被销毁，进而解决了引用成环的问题。

![image-20220516145357741](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220516145357741.png)
### 常用函数用法示例

```cpp
void test2()
{
    shared_ptr<CA> ptr_a(new CA());     // 输出：CA() called!
    shared_ptr<CB> ptr_b(new CB());     // 输出：CB() called!

    cout << "ptr_a use count : " << ptr_a.use_count() << endl; // 输出：ptr_a use count : 1
    cout << "ptr_b use count : " << ptr_b.use_count() << endl; // 输出：ptr_b use count : 1
    
    weak_ptr<CA> wk_ptr_a = ptr_a;
    weak_ptr<CB> wk_ptr_b = ptr_b;

    if (!wk_ptr_a.expired())
    {
        wk_ptr_a.lock()->show();        // 输出：this is class CA!
    }

    if (!wk_ptr_b.expired())
    {
        wk_ptr_b.lock()->show();        // 输出：this is class CB!
    }

    // 编译错误
    // 编译必须作用于相同的指针类型之间
    // wk_ptr_a.swap(wk_ptr_b);         // 调用交换函数

    wk_ptr_b.reset();                   // 将wk_ptr_b的指向清空
    if (wk_ptr_b.expired())
    {
        cout << "wk_ptr_b is invalid" << endl;  // 输出：wk_ptr_b is invalid 说明改指针已经无效
    }

    wk_ptr_b = ptr_b;
    if (!wk_ptr_b.expired())
    {
        wk_ptr_b.lock()->show();        // 输出：this is class CB! 调用赋值操作后，wk_ptr_b恢复有效
    }

    // 编译错误
    // 编译必须作用于相同的指针类型之间
    // wk_ptr_b = wk_ptr_a;

    // 最后输出的引用计数还是1，说明之前使用weak_ptr类型赋值，不会影响引用计数
    cout << "ptr_a use count : " << ptr_a.use_count() << endl; // 输出：ptr_a use count : 1
    cout << "ptr_b use count : " << ptr_b.use_count() << endl; // 输出：ptr_b use count : 1
}
```
### 参考链接

[(18条消息) 智能指针（三）：weak_ptr浅析_AlbertS的博客-CSDN博客_weakptr](https://blog.csdn.net/albertsh/article/details/82286999?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)