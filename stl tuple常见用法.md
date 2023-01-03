`std::pair`是 `std::tuple` 的拥有两个元素的特殊情况。

需要`#include <tuple>`

## 修改&访问tuple的值

```cpp
tuple<int, float, string> t1 = make_tuple(1, 2.0, "haha");

get<0>(t1) = 100;
get<1>(t1) = 200.0;
get<2>(t1) = "hi";

cout<<get<0>(t1)<<" "<<get<1>(t1)<<" "<<get<2>(t1)<<endl;
// output: 100 200 hi
```

