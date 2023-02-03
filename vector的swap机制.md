概述：由下述可见，vector的swap可以直接传入nums[i]，其会自动识别索引，即使相同的值也能交换。

原理：

`void swap( T& a, T& b );`

- swap函数需传入两个左值的引用，如果是两个指针，就直接对其指向内容进行交换。如果是两个num[i]、nums[j]，就对齐进行取地址，然后对内容进行交换。
- num[i]、nums[j]是左值，因为其可以被取地址，也可以被赋值。
- 如果是直接swap(1,3)，那肯定就不行了。

```cpp
vector<int> nums = { 1,2,3,3 };
print_container(nums);
swap(nums[0], nums[3]);
print_container(nums);

vector<int> nums1 = { 1,2,3,3 };
print_container(nums1);
swap(nums1[0], nums1[2]);
print_container(nums1);

//1 2 3 3
//3 2 3 1
//1 2 3 3
//3 2 1 3
```

