```cpp

在main.cpp中，同时包含A.h和B.h
其中，A.h include "B.h"， B.h include "A.h"
产生循环引用问题
解决方式：采用class前置声明 

// A.h
#pragma once

//#include "B.h"
class B;

class A
{
public:
	A() {}
	void doSomethingWithB(B* objB);
	int val;
};

////#ifndef A_H
////#define A_H
// #pragma once
//
//#include "B.h"
//
//class ClassA {
//public:
//    ClassA();
//    //void doSomethingWithB(ClassB* objB);
//};
//
////#endif // A_H

// B.h
#pragma once

//#include "A.h"
class A;

class B
{
public:
	B() {}
	void doSomethingWithA(A* objA);
	int val;
};

////#ifndef B_H
////#define B_H
//#pragma once
//
//#include "A.h"
//
//class ClassB {
//public:
//    ClassB();
//    //void doSomethingWithA(ClassA* objA);
//};
//
////#endif // B_H


// main.cpp
// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "A.h"
#include "B.h"

int main()
{
    std::cout << "Hello World!\n";
    A a;
    B b;
}

//#include <iostream>
//
//#include "A.h"
//#include "B.h"
//
//int main() {
//    std::cout << "Hello World!\n";
//    ClassA objA;
//    ClassB objB;
//
//    //objA.doSomethingWithB(&objB);
//    //objB.doSomethingWithA(&objA);
//
//    return 0;
//}
```
