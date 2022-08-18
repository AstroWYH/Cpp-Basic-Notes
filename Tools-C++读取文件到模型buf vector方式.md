# Tools-C++读取文件到模型buf vector方式

```c
// 实现
std::vector<unsigned char> read_binary(char const* path) {
    std::vector<unsigned char> data;
    FILE* fin = fopen(path, "rb");
    if (!fin) {
        printf("failed to read %s\n", path);
        return std::vector<unsigned char>();
    }

    fseek(fin, 0, SEEK_END);
    size_t len = ftell(fin);
    rewind(fin);

    data.resize(len);
    size_t read_size = fread(data.data(), 1, len, fin);
    if (read_size != len) {
        printf("read file error\n");
        return std::vector<unsigned char>();
    }
    fclose(fin);

    return data;
}

// 调用
const char *model_path = "";
model_path = "./model";

std::vector<unsigned char> model_data = read_binary(model_path); // 调用
LOG("model_data.size():%zu", model_data.size());

auto ret = Invoke(&model_data[0], ...);
```

