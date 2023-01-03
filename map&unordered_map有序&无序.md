```cpp
// 对比map和unordered_map，全部用“213”的顺序插入。
// 可见map的key不管是顺序还是打乱插入，最终的key会自动排成升序。
// 而unordered_map的key不管是顺序还是打乱插入，最终key排序都是不可预测的。但如果插入顺序固定，则key最终排序也固定的。

map<int, char> map1;
map1[2] = 'b';
map1[1] = 'a';
map1[3] = 'c';
for (auto it = map1.begin(); it != map1.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:1 val:a
// key:2 val:b
// key:3 val:c
map<char, int> map2;
map2['b'] = 2;
map2['a'] = 1;
map2['c'] = 3;
for (auto it = map2.begin(); it != map2.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:a val:1
// key:b val:2
// key:c val:3
unordered_map<int, char> unordered_map1;
unordered_map1[2] = 'b';
unordered_map1[1] = 'a';
unordered_map1[3] = 'c';
for (auto it = unordered_map1.begin(); it != unordered_map1.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:3 val:c
// key:2 val:b
// key:1 val:a
unordered_map<char, int> unordered_map2;
unordered_map2['b'] = 2;
unordered_map2['a'] = 1;
unordered_map2['c'] = 3;
for (auto it = unordered_map2.begin(); it != unordered_map2.end(); it++) {
    cout << "key:" << it->first << " val:"<< it->second << endl;
}
// key:c val:3
// key:b val:2
// key:a val:1
```

