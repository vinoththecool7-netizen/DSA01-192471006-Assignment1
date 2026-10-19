#include "metro.hpp"
#include <iostream>

int main() {
    metro::SmartCard card{10000};
    for (const auto fare : {1000LL, 3600LL, 2000LL, -1LL}) {
        const auto before = card.balance();
        const bool accepted = card.deduct(fare);
        if ((!accepted && card.balance() != before) || card.balance() < 5000) return 1;
        std::cout << (accepted ? "Accepted " : "Denied ") << metro::money(card.balance()) << '\n';
    }
    std::cout << "sizeof SmartCard=" << sizeof(card)
              << " sizeof procedural balance=" << sizeof(long long) << '\n';
}
