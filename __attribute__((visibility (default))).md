# __attribute__((visibility ("default/hidden")))

- “default”：用它定义的符号将被导出，动态库中的函数默认是可见的。
- “hidden”：用它定义的符号将不被导出，并且不能从其它对象进行使用，动态库中的函数是被隐藏的。
- default意味着该方法对其它模块是可见的。而hidden表示该方法符号不会被放到动态符号表里，所以其它模块(可执行文件或者动态库)不可以通过符号表访问该方法。

[(9条消息) Linux下__attribute__((visibility ("default")))的使用_fengbingchun的博客-CSDN博客___attribute__ visibility](https://blog.csdn.net/fengbingchun/article/details/78898623?ops_request_misc=%7B%22request%5Fid%22%3A%22166203631816781790776928%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fall.%22%7D&request_id=166203631816781790776928&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~hot_rank-2-78898623-null-null.142^v44^pc_ran_alice&utm_term=__attribute__ ((visibility (default))

