#include "metro.hpp"
#include <iostream>

int main() {
    for (const auto fare : metro::BASE_PAISE) std::cout << fare << " paise\n";
    std::cout << "Peak surcharge: " << metro::PEAK_PERCENT << "%\n"
              << "Minimum remaining: " << metro::money(metro::MIN_BALANCE_PAISE) << '\n';
    static_assert(metro::BASE_PAISE[2] == 3000, "fare table is constexpr");
}
