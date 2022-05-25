# stl set常见用法

[std::set - cppreference.com](https://zh.cppreference.com/w/cpp/container/set)

关联容器的一种，需要`#include<set>`

### 几种对比

```c++
按关键字有序保存元素
set                      关键字即值，只保存关键字的容器
multiset                 关键字可以重复出现的set
 
无序关联容器
unordered_set            用哈希函数组织的set，无序
unordered_multiset       哈希组织的set，关键字可以重复
```

### 常用操作

```c++
set<int> q;     //以int型为例 默认按键值升序
set<int,greater<int>> p;  //降序排列 

set<int> q(n.begin(), n.end()); //n是vector<int>，q是将n去除重复元素的set<int>
vector<int> n(q.begin(), q.end()) //q是set<int>，主要体现两者拷贝构造的相互转换

q.insert(x);	//将x插入q中
q.erase(x);		//删除q中的x元素,返回0或1,0表示set中不存在x
q.clear();		//清空q
q.empty();		//判断q是否为空，若是返回1，否则返回0
q.size();		//返回q中元素的个数
q.find(x);		//在q中查找x，返回x的迭代器，若x不存在，则返回指向q尾部的迭代器即 q.end()
q.lower_bound(x); //返回一个迭代器，指向第一个键值不小于x的元素
q.upper_bound(x); //返回一个迭代器，指向第一个键值大于x的元素

q.rend();		  //返回第一个元素的的前一个元素迭代器
q.begin();		  //返回指向q中第一个元素的迭代器

q.end();		 //返回指向q最后一个元素下一个位置的迭代器
q.rbegin();		 //返回最后一个元素
```

### 插入元素

```c++
s.insert(10);
s.insert(v.begin(), v.end());
```

### 查找和读取元素

```c++
// 与map类似
s.find(key);
```

### 删除元素

```c++
s.erase(s.begin(), s.end());
```

### 容器for()遍历

```c++
for(auto &s : set) // ok
```

### unordered_set不能放入pair

```cpp
unordered_set<pair<int, int>> set; // error，unordered_set不能放入pair，unordered_map同理
set<pair<int, int>> set; // ok
```



### 参考链接

[(18条消息) c++ set用法详解_斯文～的博客-CSDN博客_c++ set](https://blog.csdn.net/weixin_52341477/article/details/119248919)

[(18条消息) C/C++——set的基本操作总结_zhiyong_will的博客-CSDN博客_c set](https://felix.blog.csdn.net/article/details/51720988?spm=1001.2101.3001.6650.3&depth_1-utm_relevant_index=5)