```cpp
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    double DoubleVal = 1400293.852316;
    float FloatVal = static_cast<float>(DoubleVal);

    std::cout << std::fixed << std::setprecision(20) << "Double: " << DoubleVal << std::endl;
    std::cout << std::fixed << std::setprecision(20) << "Float: " << FloatVal << std::endl;

    return 0;
}

// output
Double: 1400293.85231600003316998482
Float: 1400293.87500000000000000000
```
