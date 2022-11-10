# stl vector常见用法

[std::vector - cppreference.com](https://zh.cppreference.com/w/cpp/container/vector)

需要`#include<vector>`

## vector初始化

```c++
vector vec1; // 默认初始化，最常用
vector vec2 = {1,2,3,4,5}; // 注意是{}
vector vec3(vec); 或 vector vec2 = vec; // 拷贝初始化
vector vec4(vec.begin()+2, vec.end()-1); // 迭代器初始化
vector vec5(7); // 7个元素
vector vec6(7,3); // 7个3
```

## 基本操作

```c++
(1)头文件#include<vector>
(2)创建vector对象，vector<int> vec;
(3)尾部插入数字：vec.push_back(1);
(4)使用下标访问元素，cout<<vec[0]<<endl;记住下标是从0开始的
(5)使用迭代器访问元素
vector<int>::iterator it; // 或直接 auto it = vec.begin();
for(it=vec.begin();it!=vec.end();it++)
    cout<<*it<<endl;
(6)插入元素：vec.insert(vec.begin()+i,a); // 在第i+1个元素前面插入a;
(7)删除元素：vec.erase(vec.begin()+2); // 删除第3个元素
vec.erase(vec.begin()+i,vec.end()+j); // 删除区间[i,j-1];区间从0开始
(8)向量大小:vec.size();
(9)清空:vec.clear();
(10)二维数组:vector<vector<Point2f> > points; 

begin() // 返回起始元素的迭代器
end() // 返回末元素后一元素的迭代器
front() // 返回起始元素的引用
back() // 返回末尾元素的引用
```

![image-20220424020806990](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220424020806990.png)

## 访问元素

```c++
// 下标访问
for(int i=0;i<nSize;i++)  
{  
   cout<<vecClass[i];  
}
// at()访问
for(int i=0;i<nSize;i++)  
{  
   cout<<vecClass.at(i);  
}
// 迭代器访问
for(vector<float>::iterator it = vecClass.begin();it!=vecClass.end();it++)  
{  
    cout<<*it;  
}
```

## 删除元素

```cpp
vector<int> vec = {1,2,3,4};
vec.erase(vec.end()-2, vec.end());
for (auto &v : vec) {
    cout<<v<<endl;
}
// output： 1 2

vector<int> vec = {1,2,3,4};

vec.erase(vec.begin()); // 也可以写成vec.erase(vec.begin(), vec.begin() + 1); 删除的是左闭右开！

for (auto &v:vec) {
    cout<<v<<endl;
}

cout<<"-------------"<<endl;

vec.erase(vec.end() - 1, vec.end());

for (auto &v:vec) {
    cout<<v<<endl;
}
// output
2
3
4
-------------
2
3
```

## 重新赋值

```c++
vector<int> nums;
nums = {0,1,2,3};
for (auto &n : nums) {
    cout<< n << endl; // 0 1 2 3
}

nums = {0,1,5}; // 可以c直接覆盖，传统数组不行。
for (auto &n : nums) {
    cout<< n << endl; // 0 1 5
}
```

## 插入元素

```cpp
// 尾部插入push_back() emplace_back()
vec.push_back(1);

// 位置插入insert（4种方式），返回被插入 value 的迭代器
it = v.insert(iter, 9); //（1）iter位置插入9
v.insert(iter, 3, 1); // （2）iter位置插入3个1
vector<int> vec2(2,400);
vec.insert(iter+2, vec2.begin(), vec2.end()); // （3）iter+位置插入vector<int>指定区间{2,400}
int arr[] = {100, 101};
v.insert(iter, arr, arr+2); // （4）iter位置插入数组区间{100,101}

// 参考：https://zh.cppreference.com/w/cpp/container/vector/insert
```

## 算法

```cpp
// 需要#include<algorithm>
// (1) 使用reverse()将元素翻转，将元素翻转，即逆序排列
reverse(vec.begin(),vec.end());
    
// (2) 使用sort()排序，默认是按升序排列，即从小到大
sort(vec.begin(),vec.end()); 
// 可以通过重写排序比较函数按照降序比较，如下定义排序比较函数
bool Comp(const int &a,const int &b)
{
    return a>b;
}
// 调用时就降序排序
sort(vec.begin(),vec.end(),Comp)
    
// (3) 使用find()查找，查找vec中的6，返回迭代器
vector<int>::iterator it = find(vec.begin(), vec.end(), 6);
if (it != vec.end())
{
    cout<<*it<<endl;
}
else
{
    cout<<"can not find"<<endl;
}
```

## size&capacity

```
- 在STL中，拥有capacity属性的容器只有vector和string。
- size是当前vector容器真实占用的大小，也就是容器当前拥有多少个容器。
- capacity是指在发生realloc前能允许的最大元素数，即预分配的内存空间。
- 当然，这两个属性分别对应两个方法：resize()和reserve()。
- 使用resize()，容器内的对象内存空间是真正存在的。
- 使用reserve()仅仅只是修改了capacity的值，容器内的对象并没有真实的内存空间(空间是"野"的)。
- 此时切记使用[]操作符访问容器内的对象，很可能出现数组越界的问题。
```

```c++
int main(void)
{
	vector<int> v;
	std::cout<<"v.size() == " << v.size() << " v.capacity() = " << v.capacity() << std::endl;
	v.reserve(10);
	std::cout<<"v.size() == " << v.size() << " v.capacity() = " << v.capacity() << std::endl;
	v.resize(10);
	v.push_back(0);
	std::cout<<"v.size() == " << v.size() << " v.capacity() = " << v.capacity() << std::endl;
	return 0;
}
```

![image-20220507230236569](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/image-20220507230236569.png)

### 参考链接

- [(18条消息) C++ 中vector的使用方法_那年聪聪的博客-CSDN博客_c++中vector的用法详解](https://blog.csdn.net/duan19920101/article/details/50617190?spm=1001.2101.3001.6650.8&depth_1-utm_relevant_index=14)
- [C++:vector 六种初始化方法 - 简书 (jianshu.com)](https://www.jianshu.com/p/d058866e0e10)
- [(18条消息) C++ STL之 vector的capacity和size属性区别_sequenceGO的博客-CSDN博客_capacity和size](https://blog.csdn.net/u013575812/article/details/51171135?spm=1001.2101.3001.6650.7&depth_1-utm_relevant_index=13)