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

```c
#include <stdio.h>

int main() {
    char *sa = "hello world";
    sa[0] = 'H';
}
// 编译时会有warning，因为编译器认为sa是应在常量区，会建议写为const char* sa;
// 此外，编译器也会直接把char *sa转换成const char *sa;
// 运行会报错，因为const char *sa;在常量区(虚拟内存的地址很靠前)，不能修改。

int main() {
    const char *sa = "hello world";
    sa[0] = 'H';
}
// 编译直接报错，因为通过sa修改了指针指向的内容。

int main() {
    char sa[] = "hello world";
    sa[0] = 'H';
}
// 编译、运行都正常。因为此时的"="和之前的不同，这里会进行拷贝，而之前是直接把地址赋给指针。
// 此处的sa不再在常量区，而在栈区，可以随意修改。

int main() {
    const char *sa = "hello world";
    char sb[] = "hello world";

    printf("sa:%p\n", sa);
    printf("sb:%p\n", sb);
    printf("main:%p\n", main);
}
// 操作系统会给每个程序一个虚拟内存，地址从0x00...开始，但其物理内存需要经过映射才知道。
// 可以发现，sa和main会很接近，因为main的地址很靠前，sa在常量区，就在main的地址后面一点点。
// 而sb的地址会比较大，因为在栈区。
```

