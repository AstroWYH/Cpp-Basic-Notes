# Tools-C++istringstream分割字符串

```cpp
void Stringsplit(string str, const char in)
{
	istringstream iss(str);	// 输入流
	string out;	// 接收缓冲区
    int cnt = 0;
	while (getline(iss, out, in)) // 以split为分隔符
	{
        cnt++;
		cout << "split cout:" << out << endl; // 输出
	}
}

// ...
string str("This-is-a-test");
Stringsplit(str, '-'); // 调用

// 结果
This
is
a
test
```

