```cpp
// cmake
cmake_minimum_required(VERSION 3.0)
project(LOG_TEST)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-g)

include_directories(${CMAKE_SOURCE_DIR}/glog/include
                    ${CMAKE_SOURCE_DIR}/gflags/include
                    ${CMAKE_SOURCE_DIR}/blog
                    ${CMAKE_SOURCE_DIR}/alog)

add_executable(log_test main.cc)

target_link_libraries(log_test
                      ${CMAKE_SOURCE_DIR}/glog/lib/libglog.so
                      ${CMAKE_SOURCE_DIR}/gflags/lib/libgflags.so
                      -Wl,-rpath,${CMAKE_SOURCE_DIR}/gflags/lib/
                      -Wl,-rpath,${CMAKE_SOURCE_DIR}/glog/lib/)

link_directories(${CMAKE_SOURCE_DIR}/gflags/lib
                 ${CMAKE_SOURCE_DIR}/glog/lib)

// cpp
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <gflags/gflags.h>

using namespace std;

// 定义命令行参数，第一个参数是参数名，第二个参数是默认值，第三个参数是帮助信息
DEFINE_string(name, "John", "The name of the person.");
DEFINE_int32(age, 30, "The age of the person.");

int main(int argc, char* argv[]) {
    // 初始化gflags库
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // 使用定义的命令行参数
    std::cout << "Name: " << FLAGS_name << std::endl;
    std::cout << "Age: " << FLAGS_age << std::endl;

    return 0;
}

// output
wangyuhang08@pnc-archpool-1:~/project/astrowang_test/build$ ./log_test 
Name: John
Age: 30

wangyuhang08@pnc-archpool-1:~/project/astrowang_test/build$ ./log_test --name Alice --age 25
Name: Alice
Age: 25
```
<img width="168" alt="image" src="https://github.com/AstroWYH/Cpp-Basic-Notes/assets/94472801/d68881c0-2b23-447c-ba3a-cc8c37646d45">
