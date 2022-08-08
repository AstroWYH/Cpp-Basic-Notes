# 问题积累-CMake转义字符串的正确写法

### 需求

CMake进行宏定义，宏定义内容为字符串，字符串中包括了分号，想传入.cpp文件，以下为正确的转义方式：

```cmake
target_compile_definitions(some_module PUBLIC "-DMEGVII_SCENE_CATEGORY_LIMIT_STRING=\"A\;B"")
```

