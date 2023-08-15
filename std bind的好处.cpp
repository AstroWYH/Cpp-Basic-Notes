#include <functional>
#include <iostream>

void print_sum(int a, int b) {
    std::cout << "Sum: " << a + b << std::endl;
}

int main() {
    // 绑定函数指针
    auto bind_func = std::bind(print_sum, 5, 3);
    bind_func();  // 输出：Sum: 8

    // 绑定部分参数
    auto bind_partial = std::bind(print_sum, std::placeholders::_1, 10);
    bind_partial(7);  // 输出：Sum: 17，_1 会被绑定为 7

    // 使用 bind 适配标准算法
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int target = 3;
    auto less_than_target = std::bind(std::less<int>(), std::placeholders::_1, target);
    int count = std::count_if(numbers.begin(), numbers.end(), less_than_target);
    std::cout << "Count: " << count << std::endl;  // 输出：Count: 2，有两个数小于 3

    return 0;
}

好处：可以bind后，改变一个固定函数的参数，比如以前需要固定传2个，现在可以改为传1个或不传，以此适配其他地方的调用。

您可以创建一个新的函数对象，可以像调用普通函数一样调用它，但是参数已经被绑定。
您可以通过 _1、_2 等占位符来表示部分参数，这些占位符在调用函数对象时会被替换为实际参数。
您可以适配标准算法，将自定义函数绑定到标准算法的条件中，从而使标准算法更加通用。
总之，std::bind 在需要灵活地处理参数绑定和适配时非常有用，可以减少重复代码，提高代码的可读性和可维护性。
然而，需要注意，C++11 以后引入的 lambda 表达式和函数对象等更现代的功能也可以实现类似的功能，因此在新的代码中可能更常使用这些功能。
