## int转string

```cpp
// 使用std::to_string()
#include <iostream>
#include <string>

int main ()
{
  int n = 123;
  std::string str = std::to_string(n); // 使用std::to_string()
  std::cout << n << " ==> " << str << std::endl;

  return 0;
}
```

## string转int

```cpp
#include <iostream>
#include <stdlib.h> // atoi需要的头文件

int main()
{
    std::string str = "668";
    std::cout << atoi(str.c_str()); // 借助char*, 使用atoi()
    return 0;
}

```

