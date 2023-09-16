```cpp
#include <iostream>
#include <memory>

// 基本的 Class Reference 模板
template <typename T>
class ClassReference {
public:
    // 创建指定类的对象实例
    std::unique_ptr<T> CreateInstance() {
        return std::make_unique<T>();
    }
};

class BaseClass {
public:
    virtual void Display() {
        std::cout << "BaseClass" << std::endl;
    }
};

class DerivedClass : public BaseClass {
public:
    void Display() override {
        std::cout << "DerivedClass" << std::endl;
    }
};

int main() {
    // 定义一个类引用，引用 BaseClass 类
    ClassReference<BaseClass> classRef;

    // 使用类引用创建 BaseClass 的对象实例
    std::unique_ptr<BaseClass> obj1 = classRef.CreateInstance();
    obj1->Display();

    // 更新类引用，引用 DerivedClass 类
    classRef = ClassReference<DerivedClass>();

    // 使用类引用创建 DerivedClass 的对象实例
    std::unique_ptr<DerivedClass> obj2 = classRef.CreateInstance();
    obj2->Display();

    return 0;
}
在这个示例中，我们创建了一个名为 ClassReference 的模板类，它接受一个类类型作为模板参数。
这个类有一个 CreateInstance 函数，用于创建指定类的对象实例。
在 main 函数中，我们首先创建一个引用，引用 BaseClass 类，然后使用它来创建 BaseClass 和 DerivedClass 的对象实例。
然后，我们可以调用对象的 Display 方法来显示它们的类。
```
