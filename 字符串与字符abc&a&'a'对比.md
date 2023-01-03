## "abc"&"a"&'a'对比

```cpp
#include <iostream>
#include <typeinfo> // 可用于打印类型
using namespace std;

int main() {
    string str1 = "abc";
    string str2 = "a";
    // string str3 = 'b'; // 报错，单引号是char不是string类型
    cout << typeid(str1).name() << endl; // cout: NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
    for (auto s : str1) {
        cout << typeid(s).name() << endl; // cout: c
    }
    cout << typeid(str2).name() << endl; // cout: NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
}
```

