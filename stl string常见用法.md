# stl string常见用法

[std::basic_string - cppreference.com](https://zh.cppreference.com/w/cpp/string/basic_string)

需要`include<string>`

## 初始化

```cpp
1)  string s;  // 生成一个空字符串s 
2)  string s(str) ; // 拷贝构造函数生成str的复制品 
3)  string s(str, stridx);  // 将字符串str内"始于位置stridx"的部分当作字符串的初值 
4)  string s(str, stridx, strlen) ; // 将字符串str内"始于stridx且长度顶多strlen"的部分作为字符串的初值 
5)  string s(cstr) ;  // 将C字符串（以NULL结束）作为s的初值 
6)  string s(chars, chars_len) ;  // 将C字符串前chars_len个字符作为字符串s的初值。 
7)  string s(num, ‘c’) ;  // 生成一个字符串，包含num个c字符 
8)  string s(“value”);  string s=“value”;  // 将s初始化为一个字符串字面值副本
9)  string s(str.begin(), str.end());  // 以区间begin/end(不包含end)内的字符作为字符串s的初值 
10) s.~string();  //销毁所有字符，释放内存 
```

## 常用函数

```c++
1) =, s.assign() // 赋以新值 
2) swap() // 交换两个字符串的内容 
3) +=, s.append(), s.push_back() // 在尾部添加字符 
4) s.insert() // 插入字符 
5) s.erase() // 删除字符 
6) s.clear() // 删除全部字符 
7) s.replace() // 替换字符 
8) + // 串联字符串 
9) ==,!=,<,<=,>,>=,compare() // 比较字符串 
10) size(),length() // 返回字符数量 
11) max_size() // 返回字符的可能最大个数 
12) s.empty() // 判断字符串是否为空 
13) s.capacity() // 返回重新分配之前的字符容量 
14) reserve() // 保留一定量内存以容纳一定数量的字符 
15) [ ], at() // 存取单一字符 
16) >>,getline() // 从stream读取某值 
17) << // 将谋值写入stream 
18) copy() // 将某值赋值为一个C_string 
19) c_str() // 返回一个指向正规C字符串(C_string)的指针 内容与本string串相同 有’\0’ 
20) data() // 将内容以字符数组形式返回 无’\0’ 
21) s.substr() // 返回某个子字符串 
22) begin() end() // 提供类似STL的迭代器支持 
23) rbegin() rend() // 逆向迭代器 
24) get_allocator() // 返回配置器
```

## 常用函数

```c++
// assign()
s.assign(str);
s.assign(str,1,3); // 如果str是"iamangel" 就是把"ama"赋给字符串 
s.assign(str,2,string::npos); // 把字符串str从索引值2开始到结尾赋给s 
s.assign("gaint");
s.assign("nico",5); // 把’n’ ‘I’ ‘c’ ‘o’ ‘\0’赋给字符串 
s.assign(5,'x'); // 把五个x赋给字符串

str.length()——求字符串长度
str.size()——和 length() 一样
str.capacity()——获取容量，包含了不用增加内存就能使用的字符数
str.resize(10)——设置当前 str 的大小为10，若大小大与当前串的长度，\0 来填充
str.resize(10,char c)——设置当前 str 的大小为10，若大小大与当前串的长度，字0符c 来填充
str.reserve(10)——设置str的容量 10，不会填充数据
str.swap(str1)——交换 str1 和 str 的字符串
str.push_back('A')——在str末尾添加一个字符  'A' ，参数必须是字符形式
str.append("ABC")——在str末尾添加一个字符串 "ABC"，参数必须是字符串形式

// insert()
str.insert(2,3,'A')——在str下标为2的位置添加 3个 字符'A'
str.insert(2,"ABC")——在str下标为2的位置添加 字符串 "ABC"
str.insert(2,"ABC",1)——在str下标为2的位置添加 字符串 "ABC" 中 1个 字符
str.insert(2,"ABC",1,1)——在str下标为2的位置添加 字符串 "ABC" 中从位置 1 开始的 1 个字符
// 注：上个函数参数中加粗的 1 ，可以是 string::npos，这时候最大值，从 位置1 开始后面的全部字符
str.insert( iterator pos, size_type count, CharT ch )——在 str 中，迭代器指向的 pos位置 插入 count个 字符 ch
s4.insert(++str1.begin(),2,'a');        结果：s4：ABCD    ->    AaaBCD
str.insert( iterator pos, InputIt first, InputIt last )——在 str 中，pos位置 插入 str1 的 开始位置 到 结束为止
s4.insert(s4.begin(),str1.begin(),str1.end());        结果：s4：ABCD  str1：abc   ->    abcABCD

str.erase(2)——删除 下标2 的位置开始，之后的全删除
str.erase(2,1)——删除 下标2 的位置开始，之后的 1个 删除
str.clear()——删除 str 所有
str.replace(2,4,"abcd")——从 下标2 的位置，替换 4个字节 ，为"abcd"
str.empty()——判空
    
// 反转相关(位于头文件<algorithm>)
reverse(str.begin(),str.end())——str的开始 到 结束字符反转 

// 查找相关：查找成功返回位置 ，查找失败，返回-1
// find函数：从头查找
str.find('A')——查找 'A'
str.find("ABC")——查找 "ABC"
int n=s4.find("ABC");   s4：ABCD    ->   n = 0
str.find('B',1)——从 位置1 处，查找'B'
str.find("ABC",1,2)——从 位置1 处，开始查找 'ABC' 的前 2个 字符
// rfind函数：从尾部查找
str.rfind('A')——查找 'A'
str.rfind("ABC")——查找 "ABC"
int n=s4.rfind("ABC");   s4：AAAABCD    ->   n = 3
str.rfind('B',1)——从 位置1 处，向前查找'B'
str.rfind("ABC",1,2)——从 位置1 处，开始向前查找 'ABC' 的前 2个 字符

// 如果字符串不存在包含关系，那么返回值就一定是npos
if(a.find(b)!=string::npos){
	cout<<"yes"<<endl;
}else{
	cout<<"no"<<endl;
}

// find_first_of()函数
// 查找是否包含有子串中任何一个字符
str.find_first_of("abBc")——查找 "abBc" 和str 相等的任何字符，"abBc" 中有就返回位置
str.find_first_of("abBc",1)——查找 "abBc" 和str 相等的任何字符，从 位置1 处，开始查找"abBc" 中的字符，"abBc" 中有的就返回位置
str.find_first_of("abBc",1,2)——查找 "abBc" 和str 相等的任何字符，从 位置1 处，开始查找"abBc" 的前 2个 字符，"abBc" 中有的就返回位置
// find_last_of()函数
// find_first_not_of ()末尾查找, 从末尾处开始,向前查找是否包含有子串中任何一个字符

str.find_last_of("abBc")——查找 "abBc" 和str 相等的任何字符，向前查找，"abBc" 中有的返回位置
str.find_last_of("abBc",1)——查找 "abBc" 和str 相等的任何字符，从 位置1 处，开始向前查找"abBc" 中的字符，"abBc" 中有的就返回位置
str.find_last_of("abBc",10,2)——查找 "abBc" 和str 相等的任何字符，从 位置10 处，开始向前查找"abBc" 的前 2个 字符，"abBc" 中有的就返回位置

// substr()
str1=str.substr(2)——提取子串，提取出 str 的 下标为2 到末尾，给 str1
str1=str.substr(2,3)——提取子串，提取出 str 的 下标为2 开始，提取三个字节，给 str1
const char* s1=str.data()——将string类转为字符串数组，返回给s1
char* s=new char[10]
// copy()
str.copy(s,count,pos)——将 str 里的 pos 位置开始，拷贝 count个 字符，存到 s 里

// compare函数：（str原串）与（str新串）ASCII值相等返回0；（str原串）小于（str新串）返回-1；（str原串）大于（str新串）返回1。
string str(“abcd”)
str.compare(“abcd”)——返回0。
str.compare(“abce”)——返回-1。
str.compare(“abcc”)——返回1。
str.compare(0,2,str,2,2)——用str的 下标0 开始的 2个字符 和 str的 下标2 开始的 2个 字符比较——就是用 "ab" 和 "cd”" 比较，结果返回-1。
str.compare(1,2,”bcx”,2)——用str的 下标1 开始的 2个字符 和 "bcx"的 前 2个 字符比较——就是用 "bc" 和 "bc”" 比较，返回0。
```

## "abc"&"a"&'a'对比

```c++
#include <iostream>
#include <typeinfo> // 可用于打印类型
using namespace std;

int main() {
    string str1 = "abc";
    string str2 = "a";
    // string str3 = 'b'; // 报错，单引号是char不是string类型
    cout << typeid(str1).name() << endl; // cout: NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
    for (auto s : str1) {
        cout << typeid(s).name() << endl; // cout: c
    }
    cout << typeid(str2).name() << endl; // cout: NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
}
```

### 参考链接

[(18条消息) String用法详解_manonghouyiming的博客-CSDN博客_string](https://blog.csdn.net/manonghouyiming/article/details/79827040)
