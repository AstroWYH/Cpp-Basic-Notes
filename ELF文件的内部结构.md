### ELF文件简介

对象文件(Object files)有三个种类：

1. **可重定位的对象文件(Relocatable file)**
   这是由汇编器汇编生成的 .o 文件。后面的链接器(link editor)拿一个或一些 Relocatable object files 作为输入，经链接处理后，生成一个可执行的对象文件 (Executable file) 或者一个可被共享的对象文件(Shared object file)。我们可以使用 ar 工具将众多的 .o Relocatable object files 归档(archive)成 .a 静态库文件。

2. **可执行的对象文件(Executable file)**
   文本编辑器vi、调式用的工具gdb、播放mp3歌曲的软件mplayer等等都是Executable object file。

3. **可被共享的对象文件(Shared object file)**
   这些就是所谓的动态库文件，也即 .so 文件。如果拿前面的静态库来生成可执行程序，那每个生成的可执行程序中都会有一份库代码的拷贝。如果在磁盘中存储这些可执行程序，那就会占用额外的磁盘空 间；另外如果拿它们放到Linux系统上一起运行，也会浪费掉宝贵的物理内存。如果将静态库换成动态库，那么这些问题都不会出现。动态库在发挥作用的过程 中，必须经过两个步骤：

   a) 链接编辑器(link editor)拿它和其他Relocatable object file以及其他shared object file作为输入，经链接处理后，生成另外的 shared object file 或者 executable file。

   b) 在运行时，动态链接器(dynamic linker)拿它和一个Executable file以及另外一些 Shared object file 来一起处理，在Linux系统里面创建一个进程映像。

这里以Shared Object File(.so)为分析对象，因为我们在逆向APK中会遇到的绝大部分都是此类文件。

### ELF文件格式

![image-20221123164212093](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221123164212093.png)

![image-20221123164518564](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221123164518564.png)

- ELF文件格式提供了两种视图，分别是链接视图和执行视图。
- 链接视图是以节（section）为单位，执行视图是以段（segment）为单位。
- 链接视图就是在链接时用到的视图，而执行视图则是在执行时用到的视图。上图左侧的视角是从链接来看的，右侧的视角是执行来看的。

总个文件可以分为四个部分：

1. ELF header： 描述整个文件的组织。
2. Program Header Table: 描述文件中的各种segments，用来告诉系统如何创建进程映像的。
3. sections 或者 segments：segments是从运行的角度来描述elf文件，sections是从链接的角度来描述elf文件，也就是说，在链接阶段，我们可以忽略program header table来处理此文件，在运行阶段可以忽略section header table来处理此程序（所以很多加固手段删除了section header table）。从图中我们也可以看出，segments与sections是包含的关系，一个segment包含若干个section。
4. Section Header Table: 包含了文件各个segction的属性信息，我们都将结合例子来解释。
  

![image-20221123164434631](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221123164434631.png)

**程序头部表**（Program Header Table），如果存在的话，告诉系统如何创建进程映像。
**节区头部表**（Section Header Table）包含了描述文件节区的信息，比如大小、偏移等。

执行命令”readelf -S android_server”来查看该可执行文件中有哪些section。

![image-20221123164708092](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221123164708092.png)

执行命令readelf –segments android_server，可以查看该文件的执行视图。

![image-20221123164735173](https://hanbabang-1311741789.cos.ap-chengdu.myqcloud.com/Pics/image-20221123164735173.png)

下面我们分析一些so文件中重要的Section，包括符号表、重定位表。

#### 符号表(.dynsym)

- 符号表包含用来定位、重定位程序中符号定义和引用的信息，简单的理解就是符号表记录了该文件中的所有符号，所谓的符号就是经过修饰了的函数名或者变量名，不同的编译器有不同的修饰规则。
- 例如符号_ZL15global_static_a，就是由global_static_a变量名经过修饰而来。

#### 重定位表(.rel.text/.rel.dyn/.rel.dyn)

- 重定位表在ELF文件中扮演很重要的角色，首先我们得理解重定位的概念，程序从代码到可执行文件这个过程中，要经历编译器，汇编器和链接器对代码的处理。
- 然而编译器和汇编器通常为每个文件创建程序地址从0开始的目标代码，但是几乎没有计算机会允许从地址0加载你的程序。
- 如果一个程序是由多个子程序组成的，那么所有的子程序必需要加载到互不重叠的地址上。
- 重定位就是为程序不同部分分配加载地址，调整程序中的数据和代码以反映所分配地址的过程。简单的言之，则是将程序中的各个部分映射到合理的地址上来。
- 换句话来说，重定位是将符号引用与符号定义进行连接的过程。例如，当程序调用了一个函数时，相关的调用指令必须把控制传输到适当的目标执行地址。具体来说，就是把符号的value进行重新定位。
  

#### 参考链接

[(35条消息) ELF文件详解—初步认识_code&poetry的博客-CSDN博客_elf文件](https://blog.csdn.net/daide2012/article/details/73065204?ops_request_misc={"request_id"%3A"165865554216782425179431"%2C"scm"%3A"20140713.130102334.pc_all."}&request_id=165865554216782425179431&biz_id=0&spm=1018.2226.3001.4187)

[(35条消息) ELF文件格式解析_拉斯的博客-CSDN博客](https://blog.csdn.net/feglass/article/details/51469511?spm=1001.2101.3001.6650.1&utm_medium=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-1-51469511-blog-73065204.pc_relevant_multi_platform_whitelistv4&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~CTRLIST~Rate-1-51469511-blog-73065204.pc_relevant_multi_platform_whitelistv4&utm_relevant_index=2)

