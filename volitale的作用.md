#### “volatile应该解释为“直接存取原始内存地址”比较合适，“易变的”这种解释简直有点误导人”！

```cpp
int main()
{
	int i = 10;
	int a = i;

	printf("%d", i);

	//下面汇编语句的作用就是改变内存中i的值，但是又不让编译器知道
	__asm
	{
		mov dword ptr[ebp - 4], 20h
	}
	int b = i;
	printf("i=%d", b);
	return 0;
}
```

然后，在debug（调试）版本模式运行程序，输出结果如下：

i = 10
i = 32

然后在release版本模式运行下，输出结果如下：

i = 10
i = 10

```cpp
int main()
{
    // volatile
	volatile int i = 10;
	int a = i;

	printf("%d", i);

	//下面汇编语句的作用就是改变内存中i的值，但是又不让编译器知道
	__asm
	{
		mov dword ptr[ebp - 4], 20h
	}
	int b = i;
	printf("i=%d", b);
	return 0;
}
```

加上volatile后，分别在debug和release版本运行结果都是如下：

i = 10
i = 32

这说明volatile发挥了它的作用。



#### 另一个例子

发薪资时，会计每次都把员工叫来登记他们的银行卡号；一次会计为了省事，没有即时登记，用了以前登记的银行卡号；刚好一个员工的银行卡丢了，已挂失该银行卡号；从而造成该员工领不到工资

员工 －－ 原始变量地址

银行卡号 －－ 原始变量在寄存器的备份