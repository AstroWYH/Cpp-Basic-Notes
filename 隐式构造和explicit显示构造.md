# 隐式构造和explicit显示构造

![](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220908112308348.png)

```c
string abc("abc");
PathName xyz(abc); // 没问题，直接调用构造
xyz = abc; // 如果没explicit就可以编译器自动隐式构造帮忙转换类型(string->PathName)，否则不行
xyz = PathName(abc); // 自己写的，这样可能应该可以，显示构造了PathName
```

