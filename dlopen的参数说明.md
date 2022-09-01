# dlopen的参数说明

```c
void* dlopen(const char* __filename, int __flag); 返回一个void *类型的handle，否则返回NULL。       

// 1、解析方式
RTLD_LAZY：在dlopen返回前，对于动态库中的未定义的符号不执行解析（只对函数引用有效，对于变量引用总是立即解析）。
RTLD_NOW： 需要在dlopen返回前，解析出所有未定义符号，如果解析不出来，在dlopen会返回NULL，错误为：undefined symbol。
// 2、作用范围，可与解析方式通过“|”组合使用
RTLD_GLOBAL：动态库中定义的符号可被其后打开的其它库重定位。 
RTLD_LOCAL：与RTLD_GLOBAL作用相反，动态库中定义的符号不能被其后打开的其它库重定位。如果没有指明是RTLD_GLOBAL还是RTLD_LOCAL，则缺省为RTLD_LOCAL。
// 3、作用方式
RTLD_NODELETE：在dlclose()期间不卸载库，并且在以后使用dlopen()重新加载库时不初始化库中的静态变量。这个flag不是POSIX-2001标准。 
RTLD_NOLOAD：不加载库。可用于测试库是否已加载(dlopen()返回NULL说明未加载，否则说明已加载），也可用于改变已加载库的flag，如：先前加载库的 flag为RTLD＿LOCAL，用dlopen(RTLD_NOLOAD|RTLD_GLOBAL)后flag将变成RTLD_GLOBAL。这个flag不是POSIX-2001标准。
RTLD_DEEPBIND：在搜索全局符号前先搜索库内的符号，避免同名符号的冲突。这个flag不是POSIX-2001标准。
```

[(9条消息) dlopen参数总结_小妞的大熊的博客-CSDN博客_dlopen](https://blog.csdn.net/xl19862005/article/details/115186284?spm=1001.2101.3001.6650.4&utm_medium=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-4-115186284-blog-52590182.pc_relevant_aa_2&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-4-115186284-blog-52590182.pc_relevant_aa_2&utm_relevant_index=7)