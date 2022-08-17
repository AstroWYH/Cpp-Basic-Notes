# Tools-C++通过gdb调试coredump问题

```c
// gdb常见操作
b function // // 添加函数断点
b code.cc:33 // 添加行数断点

d // 删除所有断点

info break // 列出所有断点信息
d 5 // 根据信息，删除5号断点

bt // 查看堆栈信息

// 生成coredump
ulimit -c unlimited // 生成大小不受限制
ulimit -c // 确认coredump开关状态，0为关
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ...) // 需开启-g调试编译选项

// 然后执行./test，可生成coredump文件，重新生成需删除当前路径下coredump
// 通过gdb调试
gdb ./test // 程序进入gdb状态
set args -i xxx -d xxx // 设置参数（如需要）
show args
run (或r) // 运行程序

// 如果出现类似0x0000000000000000 in ??的报错，segmentation fault等，
// 则说明coredump因为程序的crash导致调用栈崩溃，此时应该通过上述gdb调试工具添加断点，
// 在其crash前将问题定位，举例如下：
    
gdb ./go core.629415 // （go是我的可执行程序，core是coredump文件）
(gdb) b My_test // (在My_test函数加断点)
(gdb) run // (运行程序)
(gdb) info threads // (打印线程信息)

Id Target Id Frame
1 Thread 0x7ffff7fd6780 (LWP 777965) “go” 0x00007ffff6cc933d in read () at …/sysdeps/unix/syscall-template.S:84
2 Thread 0x7ffff471a700 (LWP 778007) “go” 0x00007ffff6cd9ad3 in epoll_wait () at …/sysdeps/unix/syscall-template.S:84
3 Thread 0x7ffff7f10700 (LWP 778008) “go” 0x00007ffff6ccf693 in select () at …/sysdeps/unix/syscall-template.S:84
4 Thread 0x7ffff3a32700 (LWP 778009) “go” 0x00007ffff6cd9ad3 in epoll_wait () at …/sysdeps/unix/syscall-template.S:84

(gdb) thread 4 // (切换到挂掉的线程0x7ffff3a32700)
(gdb) bt // (查看该线程中的堆栈信息)
// 之后即可查看到出问题的线程了，接下来排查对应的线程就可以了。
```

