# FAQ

## 书&视频

- C++：C++ Primer、Effective C++、C++标准库、程序员的自我修养、**设计模式、多线程、性能优化（需要学习）**
- 图形学：Games101、**OpenGL编程指南（重点学习）**、Unity Shader、Unity
- 图像处理：OpenCV编程入门（毛星云）
- 集体学习：软件系统性能工程、计算机图形学、MediaPipe

## 难点

**C++问题积累**

1. vector的底层实现。
2. dlclose nodelete问题，导致内存常驻；编译器优化了pthreadkey_fix，导致crash的问题始终得不到解决，ndk的bug。
3. 多实例句柄覆盖导致内存泄漏。
4. 二级指针带出问题，使用一级指针导致问题。
5. 接口头文件兼容问题，导致接口出问题。
7. 多线程的一个例子（需要准备）。
7. 视频项目性能优化的例子（需要准备）。

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