# stl map常见用法

[std::map - cppreference.com](https://zh.cppreference.com/w/cpp/container/map)

- map是STL的一个关联容器，在关联容器中，对象的位置取决于和它关联的键的值。
- 第一个关键字(key)，第二个该关键字的值(value)。
- 需要`#include <map>`

## 几种对比

```c++
按关键字有序保存元素
map                      关联数组；保存关键字-值对
multimap                 关键字可以重复出现的map

无序关联容器
unordered_map            用哈希函数组织的map，无序
unordered_multimap       哈希组织的map；关键字可以重复
```

1. 
   map是基于红黑树实现的有序关联容器。红黑树作为一种自平衡二叉树，保障了良好的最坏情况运行时间，即它可以做到在O(log n)时间内完成查找，插入和删除。map比起unordered_map的稳定性更强，最坏为O(log n)，且有排序、范围查找key的功能。在对单次时间敏感的场景下比较建议使用map做为容器。而 unordered_map靠hash,如果碰撞最坏情况是 O(n)。
2. unordered_map是基于哈希表实现的无序关联容器，unordered_map的增插改平均为常数时间O(1)，虽然会耗费更多的内存，如果追求多次查找平均效率最高，推荐unordered_map。
3. multimap与map类似，区别只在于multimap关键字可以重复出现。

## 初始化&**插入元素**

```cpp
// 定义一个map对象
map<int, string> mapStudent;
// 定义一个map并初始化
map<int, string> authors = { {123, "James"},
                                {456, "Jane"},
                                {789, "Charles"} };
// 第一种 用insert函數插入pair
mapStudent.insert(pair<int, string>(000, "student_zero"));
// 第二种 用insert函数插入value_type数据
mapStudent.insert(map<int, string>::value_type(001, "student_one"));
// 第三种 用"array"方式插入
mapStudent[002] = "student_first";
mapStudent[002] = "student_second"; // "array"方式，可以覆盖key:002
mapStudent.insert(map<int, string>::value_type (003, "student_one"));
mapStudent.insert(map<int, string>::value_type (003, "student_two")); // insert方式，不能覆盖key:003
```

##  查找元素

```cpp
// find 返回迭代器指向当前查找元素的位置，否则返回map::end()位置
iter = mapStudent.find("123"); // find(key)，返回iter
if (iter != mapStudent.end())
       cout<<"Find, the value is"<<iter->second<<endl; // iter->second为key的val值
else
   cout<<"Do not Find"<<endl;
```

```c++
find和count区别:
find函数是查找key的位置（返回迭代器）,find(key)。
count函数返回的是一个容器中，某一元素出现的次数。对于map，即返回key出现的次数，但是map中的key是不允许重复出现的，故count函数返回值只能是1（存在）或0（不存在），count(key)。

// 参考：https://blog.csdn.net/u013095333/article/details/90760382
```

##  刪除与清空元素

```c++
// 用迭代器刪除
iter = mapStudent.find("123");
mapStudent.erase(iter);
// 用关键字刪除
int n = mapStudent.erase("123"); // 如果刪除了会返回1，否则返回0
// 用迭代器范围刪除: 把整个map清空
mapStudent.erase(mapStudent.begin(), mapStudent.end());
// 清空
mapStudent.clear()
```

## 检查大小

```c++
int nSize = mapStudent.size();
```

##  基本操作函数

```cpp
 begin()         返回指向map头部的迭代器
 clear()         删除所有元素
 count()         返回指定元素出现的次数, (因为key值不会重复，所以只能是1 or 0)
 empty()         如果map为空则返回true
 end()           返回指向map末尾后一元素的迭代器
 equal_range()   返回特殊条目的迭代器对
 erase()         删除一个元素
 find()          查找一个元素
 get_allocator() 返回map的配置器
 insert()        插入元素
 key_comp()      返回比较元素key的函数
 lower_bound()   返回键值>=给定元素的第一个位置
 max_size()      返回可以容纳的最大元素个数
 rbegin()        返回一个指向map尾部的逆向迭代器
 rend()          返回一个指向map头部的逆向迭代器
 size()          返回map中元素的个数
 swap()          交换两个map
 upper_bound()    返回键值>给定元素的第一个位置
 value_comp()     返回比较元素value的函数
```
## insert() 插入元素

- vector insert()的返回值：指向被插入 `value` 的迭代器。
- map insert()的返回值：返回由指向被插入元素的迭代器（或阻止插入的元素的迭代器）和指代插入是否发生的 bool 组成的 pair 。

## map&unordered_map ”有序“&”无序“

```cpp
// 对比map和unordered_map，全部用“213”的顺序插入。
// 可见map的key不管是顺序还是打乱插入，最终的key会自动排成升序。
// 而unordered_map的key不管是顺序还是打乱插入，最终key排序都是不可预测的。但如果插入顺序固定，则key最终排序也固定的。

map<int, char> map1;
map1[2] = 'b';
map1[1] = 'a';
map1[3] = 'c';
for (auto it = map1.begin(); it != map1.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:1 val:a
// key:2 val:b
// key:3 val:c
map<char, int> map2;
map2['b'] = 2;
map2['a'] = 1;
map2['c'] = 3;
for (auto it = map2.begin(); it != map2.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:a val:1
// key:b val:2
// key:c val:3
unordered_map<int, char> unordered_map1;
unordered_map1[2] = 'b';
unordered_map1[1] = 'a';
unordered_map1[3] = 'c';
for (auto it = unordered_map1.begin(); it != unordered_map1.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:3 val:c
// key:2 val:b
// key:1 val:a
unordered_map<char, int> unordered_map2;
unordered_map2['b'] = 2;
unordered_map2['a'] = 1;
unordered_map2['c'] = 3;
for (auto it = unordered_map2.begin(); it != unordered_map2.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:c val:3
// key:b val:2
// key:a val:1
```

### 参考链接

- [(18条消息) C++ map用法总结（整理）_sevencheng798的博客-CSDN博客_c++map](https://blog.csdn.net/sevenjoin/article/details/81943864)
- [(18条消息) c++ map和unorder_map的区别_末日在做什么呢的博客-CSDN博客](https://blog.csdn.net/h2517956473/article/details/117152535?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)
- [(18条消息) C++ map容器和multimap容器（STL map容器）_MagnumLu的博客-CSDN博客_multimap](https://blog.csdn.net/qq_28584889/article/details/83855734?spm=1001.2101.3001.6661.1&depth_1-utm_relevant_index=1)
