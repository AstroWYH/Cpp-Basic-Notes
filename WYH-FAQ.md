# FAQ

## 书&视频

- C++：C++ Primer、Effective C++、C++标准库、程序员的自我修养、**设计模式、多线程、性能优化（需要学习）**
- 图形学：Games101、**OpenGL编程指南（重点学习）**、Unity Shader、Unity
- 图像处理：OpenCV编程入门（毛星云）
- 集体学习：软件系统性能工程、计算机图形学、MediaPipe

## 难点

**要做的点**

1. 视频neon项目代码，工程能力，框架。

   包括：性能优化、多线程、设计模式。

2. OpenGL优化，Shader至少学懂neon的。

3. Mediapipe熟悉。

4. Android能力，android studio app开发。

**C++问题积累**

1. 小细节问题：1）不能对临时变量加引用；2）const变量（或指针）不能调用其非const成员函数。3）vector的insert()和map的insert()返回值是不同的。4）shared_ptr问题：轻易传入get()给到Adpot()导致被释放；传入其引用给新线程，没增加引用计数，导致在新线程里释放。

2. vector的底层实现。

3. dlclose nodelete问题，导致内存常驻；编译器优化了pthreadkey_fix，导致crash的问题始终得不到解决，ndk的bug。

4. 多实例句柄覆盖导致内存泄漏。

5. 二级指针带出问题，使用一级指针导致问题。

6. 接口头文件兼容问题，导致接口出问题。

7. 多线程的一个例子（需要准备）：yuvsiq多实例句柄覆盖导致内存泄漏。；segment算子、segment task的同步；尖司机条件变量wait、notify的运用（生产者消费者）。

8. 视频项目性能优化的例子（需要准备）：避免不必要的图像拷贝；

9. 性能优化之一：用std::move，用unique_ptr的reset(x)接管，用emplace_back()替代push_back()，都是为了指针直接指向，减少copy过程。

10. segment cache和out 异步线程需要2个buffer，经典双缓存，一个跑异步segment，一个送下个render算子（新线程）。

11. 可以加一个条件变量condition_variable的问题，然后在过程中把wait、notify、ready的原因搞明白。

12. 终极问题：segment算子、segment task的同步。

    task中，seg、depth顺序执行，如果seg时间不超过30ms，seg没问题，如果seg+depth超过30ms，则depth出问题。

    因为input_img_mask、input_img_depth虽通过值传递进入task，但本质是指针，input_img_mask的data是process的中packet（packet虽是局部变量，但返回的引用）获取而来。正常情况下，process（segment算子线程）是会很快结束的（比如2ms），只不过下一个process会隔30ms再来。所以，猜测此时的input_img_mask是靠packet的引用强行续命到30ms，因此task mask过程不受影响。

    而input_img_depth则没有那么好运，因为下一个process来时，task depth还没结束（此时帧率会低于30fps，因为下一个process会wait上一个segment task线程执行完毕，比如task总时间为40ms，这样就会逐渐造成延时积累），所以input_img_depth如果不在WaitUntilDone后赋值，那么input_img_depth就很可能被下一个process到来时污染（然后process卡在WaitUntilDone处，wait上一个task），此时task depth还正在用input_img_depth，因此dump的input_img_depth就有问题。

    seg出图正常、depth异常问题，排查为seg算子与task两个线程同步异常导致，task中depth任务在执行时，其depth_input的buffer生命周期结束，且新的depth_input在同步前被污染，采用depth_input成员变量延长生命周期+在同步后赋值解决。

13. ```cpp
    cache_input_consume_ = std::make_unique<ImageFrame>();
    std::unique_ptr<ImageFrame> input_image_frame_consume = input_packet.Consume<ImageFrame>();
    cache_input_consume_.reset(input_image_frame_consume.release()); // wyh 这里是栈内存换成堆内存？
    ```

14. 

**OpenGL基础&优化**

1. VAO和VBO。
1. 帧缓冲；纹理和渲染缓冲区；附着（颜色、深度、模板）。概念上的区分。
1. if分支需要少写。
1. 得学会一个render分析工具。

**Android性能&内存&调试解决问题经验**

1. 工具perf等，得简单用过。
2. 内存泄漏问题 asan（工具 学会使用）。
3. 性能跟踪 systrace（工具 学会使用）。
4. crash、backtrace问题，addr2line反解。
5. 抓取分析log，定位各种问题。

## 项目描述

**视频效果SDK**

- C++工程能力。**fusion霓虹光斑、bokeh虚化**、segment分割、retain留色、doubleexposure双曝光、mask美颜。SDK代码撰写。
- google mediapipe视频开源框架了解，各calculator组合pipeline。
- **OpenGL、Shader基础和优化**。代码重构，为算法打造效率更高的OpenGL工程环境、渲染架构。
- SDK性能优化。这点**依赖于性能优化的学习**。
- Android Studio/Java App了解一点开发经验。

**YuvSiq画质SDK**

- C++工程能力。fullhdr、mfnr多帧降噪、night夜景，SDK代码撰写。
- 接口撰写设计（**要学会原理，为何C风格**）；
- pipeline、processor框架通路打通（**依赖设计模式的学习**）；
- Image2MAT、MAT2Image、utils等工具类（**依赖OpenCV的学习**）；能接触到OpenCV，自己实现一些简单工具类。
- Mempool等通路修改；segment分割、nnflow模型代码模块适配。跟算法Model打交道。
- CMake的熟悉，**构建工程的能力**。**（依赖编译、链接、装载知识）**。
- CI覆盖率。设计ut单元测试的能力。CI持续构建能力，Git熟练使用。C++ google代码规范。

**Huawei-Honor多媒体Camera HAL**

- **Camera HAL流程**（camx chi-cdkrequest metadata）
- 集成算法（拍照：RAW域 YUV域 OIS+EIS+DRQ 电影模式：滤镜 3dLut HDR10+）

## 读研

- **深度学习 卷积神经网络（需要简单回顾）** EI国际会议 基于卷积神经网络的故障诊断研究
- 数据通信嵌入式软硬件开发 数据库 上位机

## 擅长

1. 会主动利用闲暇时间自己撰写、收集CSDN、知乎、Stack Overflow的一些高赞答案，以及参考cpp reference等标准网站，并将他们进行整理。包括C++特性用法、容易混淆的复杂概念、stl各类容器常见用法、计算机底层基础知识等。
2. 同上，会记录图形学OpenGL、Shader基础知识，性能优化。
3. 同上，会记录LeetCode算法与数据结构。
4. 喜欢研究技术，写笔记记录问题；喜欢看书、看视频；喜欢持续学习的感觉。

## 灵魂拷问

1. 为什么这么短？
2. 为什么又到Peking？
2. bilibili代码随想录