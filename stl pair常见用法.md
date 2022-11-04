# stl pair常见用法

[std::pair - cppreference.com](https://zh.cppreference.com/w/cpp/utility/pair)

需要`#include <utility>`，`pair`是 [std::tuple](https://zh.cppreference.com/w/cpp/utility/tuple) 的拥有两个元素的特殊情况。
template<class T1, class T2> struct pair;

```c++
pair<T1, T2> p1;            //创建一个空的pair对象（使用默认构造），它的两个元素分别是T1和T2类型，采用值初始化。
pair<T1, T2> p1(v1, v2);    //创建一个pair对象，它的两个元素分别是T1和T2类型，其中first成员初始化为v1，second成员初始化为v2。
make_pair(v1, v2);          // 以v1和v2的值创建一个新的pair对象，其元素类型分别是v1和v2的类型。
p1 < p2;                    // 两个pair对象间的小于运算，其定义遵循字典次序：如 p1.first < p2.first 或者 !(p2.first < p1.first) && (p1.second < p2.second) 则返回true。
p1 == p2；                  // 如果两个对象的first和second依次相等，则这两个对象相等；该运算使用元素的==操作符。
p1.first;                   // 返回对象p1中名为first的公有数据成员
p1.second;                  // 返回对象p1中名为second的公有数据成员
```

## 创建&初始化

```c++
// 创建对象
pair<string, int> word_count;     // 创建一个空对象 word_count, 两个元素类型分别是string和int类型
pair<string, vector<int> > line;  // 创建一个空对象line，两个元素类型分别是string和vector类型

// 创建并初始化
pair<string, string> author("James","Joy"); // 创建并默认初始值为James和Joy
pair<int, double> nums = make_pair(1, 1.2); // 使用make_pair初始化
pair<string, int> name_age2(name_age); // 拷贝构造初始化

// 定义多个相同的pair类型对象，可用typedef简化声明
typedef pair<string,string> Author;
Author proust("March","Proust");
Author Joy("James","Joy");

// 变量间赋值
pair<int, double> p1(1, 1.2);
pair<int, double> p2 = p1; // 拷贝构造
pair<int, double> p3;
p3 = p1; // 拷贝赋值运算符
```

## 访问元素值

```c++
// 访问两个元素操作可以通过first和sencond访问
pair<int ,double> p1;
p1.first = 1;
p1.second = 2.5;
cout<<p1.first<<' '<<p1.second<<endl;
// 输出结果：1 2.5
```

## 通过tie获取元素值

```c++
std::pair<std::string, int> getPreson() {
    return std::make_pair("Sven", 25);
}

int main(int argc, char **argv) {
    std::string name;
    int ages;
    std::tie(name, ages) = getPreson(); // 以pair对象作为返回值时，可以直接通过std::tie进行接收
    std::cout << "name: " << name << ", ages: " << ages << std::endl;
    return 0;
}
```

## 修改pair的值

```cpp
pair<int, double> p1 = make_pair(1, 2.0);
p1.first = 3;
p1.second = 4.0;

cout<<p1.first<<" "<<p1.second<<endl;
// output:3 4.0
```

## 修改tuple的值

```cpp
tuple<int, float, string> t1 = make_tuple(1, 2.0, "haha");

get<0>(t1) = 100;
get<1>(t1) = 200.0;
get<2>(t1) = "hi";

cout<<get<0>(t1)<<" "<<get<1>(t1)<<" "<<get<2>(t1)<<endl;
// output: 100 200 hi
```



### 参考链接

[(18条消息) C++ pair的基本用法总结（整理）_sevencheng798的博客-CSDN博客_c++ pair](https://blog.csdn.net/sevenjoin/article/details/81937695?spm=1001.2101.3001.6650.2&depth_1-utm_relevant_index=4)