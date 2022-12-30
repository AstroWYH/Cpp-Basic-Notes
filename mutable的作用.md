### mutable的用法

```cpp
class A
{
public:
	A(int a):m_a(a){}
	void matest()const;
	void macout()const
	{
		cout << m_a << endl;
	}
private:
	mutable int m_a; // 在定义时用mutable来突破这层限制
};

void A::matest() const // 正常情况下，成员函数被const修饰，则函数内不能修改成员变量
{
	m_a = 10; // 现在就可以修改了
	cout << m_a << endl;
}
```

