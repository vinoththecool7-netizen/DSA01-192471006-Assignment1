#include "metro.hpp"
#include "safe_input.hpp"
#include <iostream>
#include <limits>

int main() {
    metro::SmartCard card{10000};
    long long value{};
    std::cout << "Card starts at " << metro::money(card.balance()) << '\n';
    if (!metro::readInteger(std::cin, std::cout, "Recharge paise (0 skips):", 0,
                            std::numeric_limits<long long>::max(), value)) return 0;
    if (value > 0 && !card.recharge(value)) std::cout << "Recharge rejected (overflow).\n";
    unsigned long long tripNumber{0};
    while (metro::readInteger(std::cin, std::cout, "Hour (0..23; -1 quits):", -1, 23, value)) {
        if (value == -1) break;
        const auto hour = static_cast<int>(value);
        if (!metro::readInteger(std::cin, std::cout, "Minute (0..59):", 0, 59, value)) break;
        const auto minute = static_cast<int>(value);
        if (!metro::readInteger(std::cin, std::cout, "Zone (1 Inner; 2 Middle; 3 Outer):", 1, 3, value)) break;
        const auto zone = static_cast<metro::Zone>(value - 1);
        if (!metro::readInteger(std::cin, std::cout, "Passenger (0 Adult; 1 Student; 2 Senior):", 0, 2, value)) break;
        const auto passenger = static_cast<metro::Passenger>(value);
        const auto fare = metro::FareTable{}.fare({zone, hour, minute, passenger});
        const bool accepted = card.deduct(fare);
        std::cout << "Trip " << ++tripNumber << " | " << (accepted ? "Accepted" : "Denied")
                  << " | Fare " << metro::money(fare) << " | Balance "
                  << metro::money(card.balance()) << '\n';
    }
    std::cout << "Final balance " << metro::money(card.balance()) << '\n';
}
