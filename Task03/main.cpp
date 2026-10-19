#include "metro.hpp"
#include <climits>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    std::cout << "sizeof bytes: bool=" << sizeof(bool) << " char=" << sizeof(char)
              << " int=" << sizeof(int) << " long long=" << sizeof(long long)
              << " unsigned=" << sizeof(unsigned) << " float=" << sizeof(float)
              << " double=" << sizeof(double) << '\n';
    std::cout << "CHAR_BIT=" << CHAR_BIT << " INT_MIN=" << INT_MIN
              << " INT_MAX=" << INT_MAX << " LLONG_MAX=" << LLONG_MAX
              << " UINT_MAX=" << UINT_MAX << '\n';
    const auto count = 3000000LL;
    const long long network = count * 3600;
    std::cout << "auto count bytes=" << sizeof(count)
              << " network paise=" << network
              << " fits int=" << std::boolalpha << (network <= INT_MAX) << '\n';
    const int i{7};
    const double mixed = i + 0.5 + true;
    std::cout << "int + double + bool = " << mixed << '\n';
    double rupees = 0.0;
    long long paise = 0;
    for (int n = 0; n < 10000; ++n) { rupees += 0.1; paise += 10; }
    std::cout << std::setprecision(17) << "10000 x Rs 0.10: double=" << rupees
              << " integer=" << paise << " paise; error rupees=" << rupees - 1000.0 << '\n';
    if (paise != 100000 || network != 10800000000LL) return 1;
}
