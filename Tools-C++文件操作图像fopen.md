# Tools-C++文件操作图像fopen

```c++
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sstream>

int read_file(char const *ifname, char **buf) {
    if(buf == nullptr) {
        return 0;
    }
    FILE *fp = fopen(ifname, "rb");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    if(buf != nullptr) {
        fseek(fp, 0, SEEK_SET);
        *buf = new char[size];
        fread(*buf, (size_t)size, 1, fp);
    }
    fclose(fp);

    return (int)size;
}

// ...
vector<string> all_test_img = {}; // all_test_img当前文件夹所有文件路径，如test/1.bgr, test/2.bgr...
void* img_buf; // 文件读取结果
for (string &test_file : all_test_img) { // test_file每个文件路径，如test/1.bgr
    // ...
	read_file(test_file.c_str(), (char **)&img_buf);
}
```

