#include "metro.hpp"
#include <array>
#include <iostream>

int main() {
    std::array<bool, 24> peak{};
    for (int h = 0; h < 24; ++h) {
        peak[static_cast<std::size_t>(h)] = (h >= 8 && h < 10) || (h >= 17 && h < 20);
    }
    for (int h = 0; h < 24; ++h)
        if (peak[static_cast<std::size_t>(h)] != metro::isPeak(h)) return 1;
    std::cout << "Peak lookup matches expression for all 24 hours; bytes=" << sizeof(peak) << '\n';
    const auto fare = metro::FareTable{}.fare({metro::Zone::Outer, 8, 0, metro::Passenger::Student});
    if (fare != 3240) return 1;
    std::cout << "Outer peak student: " << metro::money(fare) << '\n';
}
