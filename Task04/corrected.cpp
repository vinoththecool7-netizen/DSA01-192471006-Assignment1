#include <iostream>

int main() {
    const int a{7}, b{2};
    const double integerResult{static_cast<double>(a / b)};
    const double fractionalResult{static_cast<double>(a) / static_cast<double>(b)};
    const char ch{static_cast<char>('A' + 2)};
    const bool flag{5 != 0};
    const unsigned int u{1U};
    const int s{-1};
    const bool signedLess{s < 0 || static_cast<unsigned int>(s) < u};
    std::cout << integerResult << ' ' << fractionalResult << '\n'
              << ch << ' ' << static_cast<int>(ch) + 1 << '\n'
              << std::boolalpha << flag << '\n'
              << (signedLess ? "s < u" : "s >= u") << '\n';
}
