### alignas关键字用来设置内存中对齐方式，最小是8字节对齐，可以是16，32，64，128等。

```cpp
#include <iostream>
using namespace std;
 
struct  struct_Test1
{
	char c;
	int  i;
	double d;
};
 
struct alignas(8) struct_Test2
{
	char c;
	int  i;
	double d;
};
 
struct alignas(16) struct_Test3
{
	char c;
	int  i;
	double d;
};
 
struct alignas(32) struct_Test4
{
	char c;
	int  i;
	double d;
};
 
int _tmain(int argc, _TCHAR* argv[])
{
	struct_Test1 test1;
	struct_Test2 test2;
	struct_Test3 test3;
	struct_Test4 test4;
 
	cout<<"char size:"<<sizeof(char)<<endl;
	cout<<"int size:"<<sizeof(int)<<endl;
	cout<<"double size:"<<sizeof(double)<<endl;
 
	cout<<"test1 size:"<<sizeof(test1)<<endl;
	cout<<"test2 size:"<<sizeof(test2)<<endl;
	cout<<"test3 size:"<<sizeof(test3)<<endl;
	cout<<"test4 size:"<<sizeof(test4)<<endl;
 
	system("pause");
 
	return 0;
}
// 结果
alignas数据对齐测试结果C++
char size:1
int size:4
double size:8
test1 size:16
test2 size:16
test3 size:16
test4 size:32
请按任意键继续. . .
```

![image-20221122175321741](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221122175321741.png)

test1理论字节大小：13

不用数据对齐方式test1占用16个字节，为什么？因为计算机会做字节对齐，一般都是对齐8位，如果不用alignas关键字，默认一般是8位对齐，但也有机器不是8位对齐。

test2占用16字节，对齐方式alignas(8)

test3占用16字节，对齐方式alignas(16)

test4占用32字节，对齐方式alignas(32)

计算方法就是对齐数的整数倍，比如32位对齐，实际数据大小不到32字节，但内存还是占用32字节。实际数据大于32字节小于64字节，内存占用64字节。

这种明确规定占用字节大小后，编写代码将更具有跨平台性。