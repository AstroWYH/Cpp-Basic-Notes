# Tools-C++遍历文件夹返回路径+文件名

```cpp
vector<string> findfile(string path) // path为当前文件夹路径，string
{
    DIR *dp;
    struct dirent *dirp;
    vector<std::string> filename;
    if ((dp = opendir(path.c_str())) == NULL)
        perror("open dir error");
    while ((dirp = readdir(dp)) != NULL)
        filename.push_back(path + string(dirp->d_name));
    closedir(dp);
    return filename; // filename为当前文件夹下，所有文件路径+名称的集合，vector<string>
}
```

