# vector

## vector 六种初始化方法
1.vector<int> list1; 默认初始化，最常用
2.vector<int> list2(list); 或者 vector<int> ilist2 = ilist; //拷贝初
始化 ,"="
3.vector<int> list = {1,2,3.0,4,5,6,7};
  4.vector<int> list3(list.begin()+2, list.end()-1); //比较常用

  
### 参考链接
- [C++:vector 六种初始化方法 - 简书 (jianshu.com)](https://www.jianshu.com/p/d058866e0e10)
- [(18条消息) C++ vector的用法（整理）_一个幽默且帅气的程序员的博客-CSDN博客_c++vector](https://blog.csdn.net/wkq0825/article/details/82255984)
- [C++ 获取vector容器最后一个元素 - 国立秀才 - 博客园 (cnblogs.com)](https://www.cnblogs.com/guolixiucai/p/4878295.html)
- [(18条消息) vector的size和capacity有什么区别？怎么使capacity值为0？_pengshengli的博客-CSDN博客_vector的capacity](https://blog.csdn.net/pengshengli/article/details/86287794)
