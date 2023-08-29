```cpp
#include <iostream>

using namespace std;

class Test {
public:
    double val = 5;

    // 构造函数，用于将 double 值隐式转换为 Test 类型
    Test(double value) : val(value) {
      cout << "construct enter" << endl;
    }
    
    // 重载 std::ostream的 << 运算符，用于自定义输出 Test 对象，需要理解
    friend std::ostream& operator<<(std::ostream& os, const Test& t) {
        os << "os(val = " << t.val << ")";
        return os;
    }
};

int main() {
    double doubleValue = 10.0;
    {
      Test testValue = doubleValue; // 隐式类型转换
      std::cout << "[Test] " << testValue << std::endl;
    }
    
    {
      const Test& testValue = doubleValue;
      // 实测，这个地方偶尔会出现问题，可能并不一定发生隐式类型转换，导致testValue异常。比如Eigen类。
      std::cout << "[const Test&] " << testValue << std::endl;
    }
    
    {
      const auto& testValue = doubleValue; // 还是double
      std::cout << "[const auto&] " << testValue << std::endl;
    }

    return 0;
}

// output
construct enter
[Test] os(val = 10)
construct enter
[const Test&] os(val = 10)
[const auto&] 10

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <iostream>
#include <typeinfo>
#include "Eigen/Dense"
using namespace std;

int main() {
  Eigen::Matrix2d kalman_gain_matrix = Eigen::Matrix2d::Identity();
  kalman_gain_matrix << 0.524929, -0.0314678, -0.0316971, 0.500378;
  Eigen::Vector2d measurment_residual(0.814527, 0.185237);

  {
    Eigen::Vector2d state_gain = kalman_gain_matrix * measurment_residual;
    cout << "Eigen state_gain\n" << state_gain << endl;
    cout << "Eigen typeinfo state_gain " << typeid(state_gain).name() << std::endl;
  }

  {
    const auto& state_gain =
        kalman_gain_matrix * measurment_residual;
    cout << "const auto& state_gain\n" << state_gain << endl;
    cout << "const auto& typeinfo state_gain " << typeid(state_gain).name() << std::endl;
  }

  {
    const Eigen::Vector2d& state_gain =
        kalman_gain_matrix * measurment_residual; // 此时 const Eigen& 未发生隐式类型转换，为空导致异常
    cout << "const Eigen& state_gain\n" << state_gain << endl;
    cout << "const Eigen& typeinfo state_gain " << typeid(state_gain).name() << std::endl;
  }

// output
Eigen state_gain
0.42174
0.0668704
Eigen typeinfo state_gain N5Eigen6MatrixIdLi2ELi1ELi0ELi2ELi1EEE
const auto& state_gain
0.42174
0.0668704
const auto& typeinfo state_gain N5Eigen17CoeffBasedProductIRKNS_6MatrixIdLi2ELi2ELi0ELi2ELi2EEERKNS1_IdLi2ELi1ELi0ELi2ELi1EEELi6EEE
const Eigen& state_gain
6.94e-310 // 此时 const Eigen& 未发生隐式类型转换，为空导致异常
4.6553e-310
const Eigen& typeinfo state_gain N5Eigen6MatrixIdLi2ELi1ELi0ELi2ELi1EEE
```
