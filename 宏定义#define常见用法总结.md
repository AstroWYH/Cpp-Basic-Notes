# 宏定义#define常见用法总结

### 常见用法

```c
#define           // 定义一个预处理宏
#undef            // 在一个程序块中用完宏定义后，为防止后面标识符冲突需要取消其宏定义

#if               // 编译预处理中的条件命令，相当于C语法中的if语句
#ifdef            // 判断某个宏是否被定义，若已定义，执行随后的语句
#ifndef           // 与#ifdef相反，判断某个宏是否未被定义
#elif             // 若#if, #ifdef, #ifndef或前面的#elif条件不满足，则执行#elif之后的语句，相当于C语法中的else-if
#else             // 与#if, #ifdef, #ifndef对应, 若这些条件不满足，则执行#else之后的语句，相当于C语法中的else
#endif            // #if, #ifdef, #ifndef这些条件命令的结束标志
#if defined       // 与#if, #elif配合使用，判断某个宏是否被定义

// #ifdef 和 #if defined 的区别在于，后者可以组成复杂的预编译条件，如:
#if defined (AAA) && defined (BBB)
// ...
#endif

#if defined (AAA) || VERSION > 12
// ...
#endif
```

### #

在宏展开的时候会将#后面的参数替换成字符串，调用p(asdfsadf)的时候会将#exp换成"asdfsadf"

```c++
＃define p(exp) printf(#exp);
```

### ##

将前后两个的单词拼接在一起，调用cat(var, 123)展开后成为var123.

```c++
#define cat(x,y) x##y
```

### #@

将值序列变为一个字符，调用ch(a)展开后成为'a'。

```c
#define ch(x) #@x
```

### ...

宏定义中的省略号表示一串可变的参数, 这一串参数可用args加以引用。

```c
#define PDEBUG(fmt, args...) printk( KERN_DEBUG "scull: " fmt, ## args)

// 示例
PDEBUG("a=%d, b=%d", a, b);
// 宏展开后
printk( KERN_DEBUG "scull: " "a=%d, b=%d", a, b);
// 如果在args与省略号之间加入逗号,就变成args = a, 而省略号指向的那串参数只包含一个b。
```

```cpp
#define TAG "[YOOHAN]"

#define LOG(msg, v...)                  \
    do {                                \
        printf(TAG " : " msg, ##v);     \
    } while (0); printf("\n")

LOG("set a:%d b:%f", a, b);
```



#### 参考链接

[(17条消息) C语言中#undef的常用法_诺诺埃里斯·尤克里里的博客-CSDN博客_c语言undef](https://blog.csdn.net/u014170067/article/details/53561821?ops_request_misc=%7B%22request%5Fid%22%3A%22166006485416782425156608%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fall.%22%7D&request_id=166006485416782425156608&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~hot_rank-1-53561821-null-null.142^v40^pc_search_integral,185^v2^control&utm_term=%23undef&spm=1018.2226.3001.4187)

[(17条消息) ifdef和if defined_weixin_34402090的博客-CSDN博客](https://blog.csdn.net/weixin_34402090/article/details/92612095?spm=1001.2101.3001.6650.12&utm_medium=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-12-92612095-blog-79443014.pc_relevant_multi_platform_whitelistv3&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-12-92612095-blog-79443014.pc_relevant_multi_platform_whitelistv3&utm_relevant_index=18)

[(17条消息) C语言宏定义中#define中的井号#的使用_rtoax的博客-CSDN博客_define 井号](https://blog.csdn.net/Rong_Toa/article/details/82711501?ops_request_misc=&request_id=&biz_id=102&utm_term=?ops_request_misc=&request_id=&biz_id=102&utm_term=&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0-82711501.142^v39^pc_rank_34_1,185^v2^control&spm=1018.2226.3001.4187#define)

[(17条消息) #(井号)在C/C++的#define宏定义中的特殊用法 _Jacky_Dai的博客-CSDN博客](https://blog.csdn.net/Jacky_Dai/article/details/83968176?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2~default~ESLANDING~default-1-83968176-blog-82711501.pc_relevant_multi_platform_whitelistv2eslanding&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~ESLANDING~default-1-83968176-blog-82711501.pc_relevant_multi_platform_whitelistv2eslanding&utm_relevant_index=2)

[(17条消息) C语言#define宏中省略号的含义？_adamska0104的博客-CSDN博客_宏定义里面的省略号](https://blog.csdn.net/adamska0104/article/details/41011789?ops_request_misc={"request_id"%3A"166003163616780366518826"%2C"scm"%3A"20140713.130102334.."}&request_id=166003163616780366518826&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~baidu_landing_v2~default-2-41011789-null-null.142^v39^pc_rank_34_1,185^v2^control&utm_term=%23define)