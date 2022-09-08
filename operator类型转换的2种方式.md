# operator类型转换的2种方式

![image-20220908113921089](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220908113921089.png)

```c
// 1）可以在Apple里写operator Orange() const;无返回类型，可将Apple转换成Orange
// 2）可在Orange构造里写需要Apple，则编译器会自动隐式转换

// f(a)时会报错，因为两者转换都可以，会冲突，最简单改法可将第2种改为expicit Orange(Apple);即可
```

