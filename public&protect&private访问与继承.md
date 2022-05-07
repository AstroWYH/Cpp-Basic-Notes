# public&protect&private访问与继承

## 访问范围

- **private**：只能由<u>1.该类中的函数</u>；2.其友元函数访问。不能被任何其他访问，该类的对象也不能访问。
- **protected**：可以被<u>1.该类中的函数</u>；<u>2.子类的函数</u>；3.其友元函数访问。但不能被该类的对象访问。
- **public**：可以被<u>1.该类中的函数</u>；<u>2.子类的函数</u>；<u>3.该类的对象访问</u>；4.其友元函数访问。

## 继承

- **private继承**：父类的protected和public属性在子类中变为**private**。
- **protected继承**：父类的protected和public属性在子类中变为**protected**。
- **public继承**：父类中的protected和public属性**不发生改变**。

## Class&Struct对比

- 使用 class 时，类中的成员默认都是 **private** 属性的；而使用 struct 时，结构体中的成员默认都是 **public** 属性的。
- class 继承默认是 **private** 继承，而 struct 继承默认是 **public** 继承。
- class 可以使用模板，而 struct 不能。

## 友元的优缺点

**优点**：利用 friend 修饰符，可以让一些普通函数或另一个类的成员函数，直接对某个类的保护成员和私有成员进行操作，提高了程序的运行效率；同时避免把类的成员都声明为public，最大限度地保护数据成员的安全。
**缺点**：即使是最大限度地保护数据成员，友元也破坏了类的封装性。如果将类的封装比喻成一堵墙的话，那么友元机制就像墙上开了一个门。所以使用友元时一定要慎重。

### 参考链接

[(19条消息) public、private、protected的区别_浮鱼浮鱼的博客-CSDN博客_public,private,protected区别](https://blog.csdn.net/weixin_50849959/article/details/116595077?spm=1001.2101.3001.6650.3&depth_1-utm_relevant_index=6)
[(19条消息) 【C++】友元函数和友元类（作用及优缺点）_Jacky_Feng的博客-CSDN博客_c++友元的作用](https://blog.csdn.net/Jacky_Feng/article/details/109533248)
