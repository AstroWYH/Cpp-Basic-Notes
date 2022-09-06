# const char*字符串的理解

```c
const char* p = "abcde"; 
// 常用来表示字符串，其中const char* p的p这个指针，就代表的是这个字符串"abcde"。
// 而*p代表的则是"a"，这个字符串的首地址解析，即第一个字母。
// 虽然是const char*，指向物不能变，底层const，指针常量。
// 但可以再改为p = "xyz"等，因为这个字符串是指针，指针可以改，而指向物不能改。
```

```c
// const char* > int的转换方法：
#include <stdio.h>

int main() {
    const char* s = "56";
    printf("s:%s\n", s); // "56"
    // printf("s首字符:%s\n", *s); // 报错，可能因为*s是首字符，不能用%s打印，%s是打印字符串
    printf("*s首字符的ascii码:%d\n", *s); // 53
    printf("*s首字符的数字形式:%d\n", *s - '0'); // 53 - 48 = 5
}
```

