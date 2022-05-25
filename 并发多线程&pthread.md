# 并发多线程&pthread

- 需要`＃include <pthread.h>`
- 需要链接`libpthread.so`

### pthread_create()

在Linux下创建的线程的API接口是pthread_create()，它的完整定义是：

```cpp
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg); 
```

1. 线程句柄 thread：当一个新的线程调用成功之后，就会通过这个参数将线程的句柄返回给调用者，以便对这个线程进行管理。 
2. 入口函数 start_routine()： 当你的程序调用了这个接口之后，就会产生一个线程，而这个线程的入口函数就是start_routine()。如果线程创建成功，这个接口会返回0。 
3. 入口函数参数 *arg : start_routine()函数有一个参数，这个参数就是pthread_create的最后一个参数arg。这种设计可以在线程创建之前就帮它准备好一些专有数据，最典型的用法就是使用C++编程时的this指针。start_routine()有一个返回值，这个返回值可以通过pthread_join()接口获得。 
4. 线程属性 attr： pthread_create()接口的第二个参数用于设置线程的属性。这个参数是可选的，当不需要修改线程的默认属性时，给它传递NULL就行。具体线程有那些属性，我们后面再做介绍。

### 示例

```cpp
#include <stdio.h> 
#include <pthread.h>
void* thread( void *arg )  
{  
    printf( "This is a thread and arg = %d.\n", *(int*)arg);  
    *(int*)arg = 0;  
    return arg;  
}  
int main( int argc, char *argv[] )  
{  
    pthread_t th;  
    int ret;  
    int arg = 10;  
    int *thread_ret = NULL;  
    ret = pthread_create( &th, NULL, thread, &arg ); // pthread_create
    if( ret != 0 ){  
        printf( "Create thread error!\n");  
        return -1;  
    }  
    printf( "This is the main process.\n" );  
    pthread_join( th, (void**)&thread_ret ); // pthread_join
    printf( "thread_ret = %d.\n", *thread_ret );  
    return 0;  
}

// 执行结果可能为：
$ ./thread
This is the main process.
This is a thread and arg = 10.
thread_ret = 0.

代码分析： 
- 调用pthread_create()接口创建了一个新的线程，这个线程的入口函数是thread()，并且给这个入口函数传递了一个参数，且参数值为10。 
- 这个新创建的线程要执行的任务非常简单，只是将显示“This is a thread and arg = 10”这个字符串，因为arg这个参数值已经定义好了，就是10。之后线程将arg参数的值修改为0，并将它作为线程的返回值返回给系统。
- pthread_join()这个接口的第一个参数就是新创建线程的句柄了，而第二个参数就会去接受线程的返回值。pthread_join()接口会阻塞主进程的执行，直到合并的线程执行结束。由于线程在结束之后会将0返回给系统，那么pthread_join()获得的线程返回值自然也就是0。输出结果“thread_ret = 0”也证实了这一点。
```

### pthread_join()

- pthread_create()接口负责创建了一个线程。那么线程也属于系统的资源，这跟内存没什么两样，而且线程本身也要占据一定的内存空间。

- 众所周知，C/C++编程中如果要通过malloc()或new分配了一块内存，就必须使用free()或delete来回收这块内存，否则就会产生内存泄漏。

- 既然线程和内存没什么两样，那么有创建就必须得有回收，否则就同样产生会资源泄漏问题，这同样也是一个严重的问题。那么线程的合并就是回收线程资源了。

- 线程的合并是一种主动回收线程资源的方案。当一个进程或线程调用了针对其它线程的pthread_join()接口，就是线程合并了。这个接口会阻塞调用进程或线程，直到被合并的线程结束为止。**当被合并线程结束，pthread_join()接口就会回收这个线程的资源**，**并将这个线程的返回值返回给合并者**。
- 本例中，pthread_join()接口会**阻塞主进程的执行，直到合并的线程执行结束。**

### pthread_detach()

- 与线程合并相对应的另外一种线程资源回收机制是线程分离，调用接口是pthread_detach()。

- 线程分离是**将线程资源的回收工作交由系统自动来完成**，也就是说当被分离的线程结束之后，系统会自动回收它的资源。因为线程分离是启动系统的自动回收机制，那么**程序也就无法获得被分离线程的返回值**，这就使得pthread_detach()接口只要拥有一个参数就行了，那就是被分离线程句柄。

- 线程合并和线程分离都是用于回收线程资源的，可以根据不同的业务场景酌情使用。不管有什么理由，你都必须选择其中一种，否则就会引发资源泄漏的问题，这个问题与内存泄漏同样可怕。

### 线程的属性

后续补充

### 参考链接

[(1条消息) pthread详解_提出问题 解决问题的博客-CSDN博客_pthread()](https://blog.csdn.net/networkhunter/article/details/100218945?ops_request_misc=%7B%22request%5Fid%22%3A%22165344789516781685349169%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=165344789516781685349169&biz_id=0&spm=1018.2226.3001.4187)