### false sharing的概念

- 缓存是由缓存行组成的，通常是64字节（常用处理器的缓存行是64字节的，比较旧的处理器缓存行是32字节的），并且它有效地引用主内存中的一块地址。
- 一个java的long类型是8字节，因此在一个缓存行中可以存8个long类型的变量。

![image-20221122175810061](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221122175810061.png)

- 在程序运行的过程中，缓存每次更新都从主内存中加载连续的64个字节。因此，如果访问一个long类型的数组时，当数组中的一个值被加载到缓存中时，另外7个元素也会被加载到缓存中。但是，如果使用的数据结构中的项在内存中不是彼此相邻的，**比如链表，那么将得不到免费缓存加载带来的好处。**

- 不过，这种免费加载也有一个坏处。设想如果我们有个long类型的变量a，它不是数组的一部分，而是一个单独的变量，并且还有另外一个long类型的变量b紧挨着它，那么当加载a的时候将免费加载b。

- 看起来似乎没有什么问题，但是如果一个cpu核心的线程在对a进行修改，另一个cpu核心的线程却在对b进行读取。当前者修改a时，会把a和b同时加载到前者核心的缓存行中，更新完a后其它**所有包含a的缓存行都将失效**，**因为其它缓存中的a不是最新值了**。而当后者读取b时，发现这个缓存行已经失效了，需要从主内存中重新加载。

- 请记着，我们的缓存都是以缓存行作为一个单位来处理的，所以失效a的缓存的同时，也会把b失效，反之亦然。

![image-20221122175917834](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221122175917834.png)

- 这样就出现了一个问题，b和a完全不相干，每次却要因为a的更新需要从主内存重新读取，它被缓存未命中给拖慢了。这就是传说中的伪共享。

### 如何避免false sharing

伪共享的原理我们知道了，一个缓存行是64字节，一个long类型是8个字节，所以避免伪共享也很简单，大概有以下三种方式：

（1）在两个long类型的变量之间再加7个long类型

```java
class Pointer {
    volatile long x;
    long p1, p2, p3, p4, p5, p6, p7; // 在中间塞字节数，直到x、y不在同一个缓存行
    volatile long y;
}
```

（2）用C++ 11关键字alignas设置64位内存对齐。

```cpp
struct alignas(64) struct_Test
{
	char c;
	int  i;
	double d;
};
```

![image-20221122181628570](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221122181628570.png)

![image-20221122181641321](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221122181641321.png)