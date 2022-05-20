# sting&char*&char[]

## string转换成const char *（容易）

```cpp
string str = "Hello World";
const char *ch1 = str.c_str();
const char *ch2 = str.data();
// 此时，ch1与ch2的内容将都是”Hello World”。但是只能转换成const char*，如果去掉const编译不能通过。
```

## string转换成char *（麻烦）

```c++
string str = "Hello World";
int len = str.length();
char *data = new char[len+1];  // 这里需要注意
strcpy(data, str.c_str());  // const char *data = new char[len+1];  strcpy(data, str);
```

## string转换成char[ ]（麻烦）

```c++
string str = "Hello World";
int len=str.length();
char ch[255]={};
for( int i=0;i<str.length();i++)
    ch[i] = str[i];
ch[len+1] = '\0';
printf("%s\n", ch);
```

## char *转换成string（容易）

```c++
string str; 
const char *pc = "Hello World"; 
str = pc;
printf(“%s\n”, str);  // 错误
printf("%s",str.c_str()); // 正确
```

## char[ ] 转换成string（容易）

```c++
char ch [] = "ABCDEFG";
string str(ch); //也可string str = ch;
```

### 参考链接

[(18条消息) String用法详解_manonghouyiming的博客-CSDN博客_string](https://blog.csdn.net/manonghouyiming/article/details/79827040)