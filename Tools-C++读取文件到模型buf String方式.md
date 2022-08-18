# Tools-C++读取文件到模型buf String方式

```c
// 实现
#include <fstream>
std::string Handler::GetFileData(char const *path)
{
	std::ifstream ifs{path, std::ios::binary | std::ios::ate};
	if (!ifs)
		return std::string();
	ifs.seekg(0, std::ios::end);
	auto size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::string data(size, '\0');
	ifs.read(&data[0], size);
	ifs.close();
	return data;
}

// 调用
const char *model_path = "";
model_path = "./model";
auto model_data = GetFileData(model_path); // 调用
if (model_data.empty())
{
    LOG("loading model %s failed\n", model_path);
}
config.model_data = {model_data.data(), model_data.size()};
// ...
```

