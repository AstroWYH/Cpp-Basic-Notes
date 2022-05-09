# new&delete&malloc&free

![image-20220506171434723](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20220506171434723.png)

1. new/delete是C++**关键字**、**操作符**，需要编译器支持，**不需要头文件**支持。malloc/free是**库函数**，**需要头文件**支持。
2. new做两件事，**一是分配内存**，**二是调用类的构造函数**，delete会调用类的析构函数和释放内存。malloc/free**只是分配和释放内存**。
3. new建立的是一个对象，而malloc分配的是一块内存；new建立的对象可以用成员函数访问，不要直接访问它的地址空间；malloc分配的是一块内存区域，用指针访问，可以在里面移动指针；new出来的指针是带有类型信息的，而malloc返回的是void指针。
4. new操作符申请内存分配**无须指定内存块的大小**，编译器会根据类型信息自行计算。malloc需要**显式地指出所需内存的尺寸**。
5. new操作符内存分配成功时，返回的是**对象类型的指针**，类型严格与对象匹配，无须进行类型转换，是符合类型安全性的操作符。malloc内存分配成功则是返回**void*** ，需要通过强制类型转换将void* 指针转换成需要的类型。
6. new内存分配失败时，会抛出**bad_alloc异常**。malloc分配内存失败时返回**NULL**。
7. new会先调用operator new函数，申请足够的内存（通常底层使用malloc实现）。然后调用类型的构造函数，初始化成员变量，最后返回自定义类型指针。delete先调用析构函数，然后调用operator delete函数释放内存（通常底层使用free实现）。
8. C++**允许重载**new/delete操作符，malloc**不允许重载**。
9. new操作符从**自由存储区（free store）**上为对象动态分配内存空间，而malloc函数从**堆（heap）**上动态分配内存。

## **自由存储区 堆**

1. 自由存储是C++中通过new与delete动态分配和释放对象的抽象概念，而堆（heap）是C语言和操作系统的术语，是操作系统维护的一块动态分配内存。
2. new所申请的内存区域在C++中称为自由存储区。即由堆实现的自由存储，可以说new所申请的内存区域在堆上。
3. 堆与自由存储区还是有区别的，它们并非等价。

### **参考链接**

- https://blog.csdn.net/nyist_zxp/article/details/80810742?spm=1001.2101.3001.6650.3&depth_1-utm_relevant_index=6
- https://blog.csdn.net/nie19940803/article/details/76358673
- https://www.cnblogs.com/litao-tech/p/4318424.html
- https://www.cnblogs.com/QG-whz/p/5060894.html

