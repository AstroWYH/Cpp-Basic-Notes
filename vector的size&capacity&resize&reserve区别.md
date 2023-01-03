## size

- 指当前容器所存储的元素个数。
- size是**当前容器存储的个数**。

## capacity

- 指容器在分配新的存储空间之前能存储的元素总数。
- capacity是**容器可存储的最大总数**。
- **capacity是不能小于size的**。

## resize

- **既分配了空间，也创建了对象**。这里空间就是capacity，对象就是容器中的元素。
- resize()**既修改capacity大小，也修改size大小**。

## reserve

- reserve()表示容器**预留空间**，**但不是真正的创建对象**，需要通过insert()或push_back()等操作创建对象。
- reserve()**只修改capacity大小，不修改size大小**。



## 代码实测

```cpp
// 操作1
vector<int> v1;
vector<int> v2;
vector<int> v3;
vector<int> v4;

cout << "v1_size: " << v1.size() << "\t vl_capacity: " << v1.capacity()<<endl;
cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
cout << "v3_size: " << v3.size() << "\t v3_capacity: " << v3.capacity() << endl;    
cout << "v4_size: " << v4.size() << "\t v4_capacity: " << v4.capacity() << endl << endl;

// output
v1_size: 0 ， vl_capacity: 0
v2_size: 0 ， v2_capacity: 0
v3_size: 0 ， v3_capacity: 0
v4_size: 0 ， v4_capacity: 0
// 看来刚创建的空容器，size和capacity是一致的。

// 操作2
v2.push_back(99);
cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;

v2_size: 1，v2_capacity: 1
// 哇！size和capacity同时加1了。此时size可以理解，因为当前v2中的元素个数为1。至于capacity，根据定义，capacity是不能小于size的。
// 这里size和capacity是相等的，但感觉怪怪的。

int nums = 20;
for (int i = 0; i < nums; ++i){
    v2.push_back(i+1);
    cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
}

// output
v2_size: 2 ， v2_capacity: 2
v2_size: 3 ， v2_capacity: 3
v2_size: 4 ， v2_capacity: 4
v2_size: 5 ， v2_capacity: 6
v2_size: 6 ， v2_capacity: 6
v2_size: 7 ， v2_capacity: 9
v2_size: 8 ， v2_capacity: 9
v2_size: 9 ， v2_capacity: 9
v2_size: 10 ， v2_capacity: 13
v2_size: 11 ， v2_capacity: 13
v2_size: 12 ， v2_capacity: 13
v2_size: 13 ， v2_capacity: 13
v2_size: 14 ， v2_capacity: 19
v2_size: 15 ， v2_capacity: 19
v2_size: 16 ， v2_capacity: 19
v2_size: 17 ， v2_capacity: 19
v2_size: 18 ， v2_capacity: 19
v2_size: 19 ， v2_capacity: 19
v2_size: 20 ， v2_capacity: 28
v2_size: 21 ， v2_capacity: 28
// 有时候size和capacity相等，有时候则capacity更大。
    
// 操作3
v1.resize(100);
v2.resize(100);
v2.push_back(99);   // 添加一个元素

v3.reserve(100);
v4.reserve(100);

// output
v1_size: 100 ， vl_capacity: 100
v2_size: 101 ， v2_capacity: 150
v3_size: 0 ， v3_capacity: 100
v4_size: 0 ， v4_capacity: 100
    
// 此时v1的size和capacity都为100，这符合resize的定义：既分配了空间，也创建了对象。这里的空间就是100，对象就是100个元素。猜猜这100个元素值是多少，比如v1[0]。默认初始化为0，即v1[0]=0。
// v2比较有意思，因为比v1多加了一个元素，所以size为101，但其capacity居然是150。大家默认理解就行，毕竟是系统根据vector特性进行分配的存储空间，我们方向使用就好。
// v3和v4则一致，size为0，capacity为100。这符合reserve操作的定义：reserve()表示容器预留空间，但不是真正的创建对象。所以这里的size为0，当前容器里根本没有元素。
    
// 操作4
v1.resize(200);
v2.reserve(200);
v3.reserve(200);
v4.resize(200);

cout << "v1_size: " << v1.size() << "\t vl_capacity: " << v1.capacity() << endl;
cout << "v2_size: " << v2.size() << "\t v2_capacity: " << v2.capacity() << endl;
cout << "v3_size: " << v3.size() << "\t v3_capacity: " << v3.capacity() << endl;
cout << "v4_size: " << v4.size() << "\t v4_capacity: " << v4.capacity() << endl << endl;

// output
v1_size: 200 ， vl_capacity: 200
v2_size: 101 ， v2_capacity: 200
v3_size: 0 ， v3_capacity: 200
v4_size: 200 ， v4_capacity: 200

// v1的size和capacity都为200，符合resize()的作用：size和capacity都改变。
// v2的size为101，capacity为200，符合reserve()的作用：不改变size，但改变capacity。
// v3经过两次reserve()后，size为0，capacity为200，同样符合reserve()的作用。
// v4先经过一次reserve()，此时size为0，capacity为200；再经过resize()后，size和capacity都为200。

// 操作5
v4.push_back(8);
cout << "v4_size: " << v4.size() << "\t v4_capacity: " << v4.capacity() << endl << endl;

// output
v4_size: 201 ， v4_capacity: 300

// 在给v4添加新元素之前，v4的size和capacity都为200。我们给v4添加一个元素后，size为201，capacity却已经变成300了。
// 可见系统给vector动态分配的存储空间，真的难以捉摸。不过我们作为使用者，其实并不需要考虑这些问题。
```



#### 参考链接

[(50条消息) C++ vector中的resize，reserve，size和capacity函数讲解_Amusi（CVer）的博客-CSDN博客_vector的resize](https://blog.csdn.net/amusi1994/article/details/81106455?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-1-81106455-blog-99623866.pc_relevant_multi_platform_whitelistv6&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-1-81106455-blog-99623866.pc_relevant_multi_platform_whitelistv6&utm_relevant_index=2)