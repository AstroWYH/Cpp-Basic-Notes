# Tools-C++字符串string和stringstream表示路径

```c
// 使用std::strings
std::string path = std::string("/xxx/") + name + std::string("_") + std::string(".jpg");
func(path, ...);

// 使用std::stringstream
std::stringstream path;
path << "/xxx/" << name << "_" << ".jpg";
func(path.str(), ...);
```

