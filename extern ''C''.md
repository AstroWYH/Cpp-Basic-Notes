# extern "C"

### 作用

- extern "C"是C++特有的指令（C无法使用该指令），目的在于**支持C++与C混合编程**。
- extern "C"的作用是告诉C++编译器**用C规则编译指定的代码**（除函数重载外，extern "C"不影响C++其他特性）。

### 原因

为什么要用C规则编译C++代码呢？因为C和C++的编译规则不一样，主要区别体现在**编译期间生成函数符号**的规则不一致。

C++比C出道晚，但是增加了很多优秀的功能，**函数重载**就是其中之一。由于C++需要支持重载，单纯的函数名无法区分出具体的函数，所以在编译阶段就需要将形参列表作为附加项增加到函数符号中。

#### C和C++编译Function生成的符号对比

```cpp
void Function(int a, int b)
{
    printf("Hello!!! a = %d, b = %d\n", a, b);
}
// C 汇编结果
...
Function: // C生成的符号
.LFB11:
    .cfi_startproc
    movl    %esi, %edx
    xorl    %eax, %eax
    movl    %edi, %esi
    movl    $.LC0, %edi
    jmp    printf
    .cfi_endproc
...
// C++ 汇编结果
...
_Z8Functionii: // C++生成的符号
.LFB12:
    .cfi_startproc
    movl    %esi, %edx
    xorl    %eax, %eax
    movl    %edi, %esi
    movl    $.LC0, %edi
    jmp    printf
    .cfi_endproc
...
```

#### C++和C不兼容的例子

```cpp
/* MyFunction.c */
void Function(int a, int b)
{
    printf("Hello!!! a = %d, b = %d\n", a, b);
}

/* main.cpp */
extern void Function(int a, int b);
 
int main()
{
    Function(1, 2);
}

// 编译阶段没报错，链接报错
main.cpp:(.text.startup+0xf)：对‘Function(int, int)’未定义的引用
collect2: 错误：ld 返回 1
```

#### 两个文件的汇编结果

```assembly
    .file    "MyFunction.c"
    .section    .rodata.str1.1,"aMS",@progbits,1
.LC0:
    .string    "Hello!!! a = %d, b = %d\n"
    .text
    .p2align 4,15
    .globl    Function
    .type    Function, @function
Function:
.LFB11:
    .cfi_startproc
    movl    %esi, %edx
    xorl    %eax, %eax
    movl    %edi, %esi
    movl    $.LC0, %edi
    jmp    printf
    .cfi_endproc
.LFE11:
    .size    Function, .-Function
    .ident    "GCC: (GNU) 6.4.1 20170727 (Red Hat 6.4.1-1)"
    .section    .note.GNU-stack,"",@progbits
/********************************************************/
    .file    "main.cpp"
    .section    .text.startup,"ax",@progbits
    .p2align 4,,15
    .globl    main
    .type    main, @function
main:
.LFB0:
    .cfi_startproc
    subq    $8, %rsp
    .cfi_def_cfa_offset 16
    movl    $2, %esi
    movl    $1, %edi
    call    _Z8Functionii # 调用的_Z8Functionii，而非Function，
    xorl    %eax, %eax
    addq    $8, %rsp
    .cfi_def_cfa_offset 8
    ret
    .cfi_endproc
.LFE0:
    .size    main, .-main
    .ident    "GCC: (GNU) 6.4.1 20170727 (Red Hat 6.4.1-1)"
    .section    .note.GNU-stack,"",@progbits
```



### 参考链接

[(1条消息) C++中extern使用_AplusX的博客-CSDN博客_extern在c++中是什么意思](https://blog.csdn.net/qq_41598072/article/details/118608272)

[(1条消息) extern “C“的作用及理解_米碎师兄的博客-CSDN博客_extern “c”](https://blog.csdn.net/sinat_36817189/article/details/110423243)