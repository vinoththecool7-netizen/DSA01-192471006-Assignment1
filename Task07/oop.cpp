#include "metro.hpp"
#include <iostream>

int main() {
    metro::SmartCard card{10000};
    const metro::Journey trips[]{
        {metro::Zone::Inner, 7}, {metro::Zone::Outer, 8}, {metro::Zone::Middle, 10}
    };
    for (const auto& trip : trips) {
        const auto fare = metro::FareTable{}.fare(trip);
        const auto before = card.balance();
        const bool accepted = card.deduct(fare);
        if ((!accepted && card.balance() != before) || card.balance() < 5000) return 1;
        std::cout << (accepted ? "Accepted " : "Denied ") << metro::money(card.balance()) << '\n';
    }
    if (card.deduct(-1) || card.balance() != 5400) return 1;
    std::cout << "Denied " << metro::money(card.balance()) << '\n';
    std::cout << "sizeof SmartCard=" << sizeof(card)
              << " sizeof procedural balance=" << sizeof(long long) << '\n';
}
