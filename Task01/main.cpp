#include "metro.hpp"
#include <iostream>

int main() {
    metro::SmartCard card{10000};
    const metro::Journey trip{metro::Zone::Outer, 8};
    const auto fare = metro::FareTable{}.fare(trip);
    std::cout << "Outer peak: " << metro::money(fare) << '\n';
    std::cout << "Accepted: " << std::boolalpha << card.deduct(fare)
              << "; balance " << metro::money(card.balance()) << '\n';
    const auto before = card.balance();
    if (card.deduct(2000) || card.balance() != before) return 1;
    std::cout << "Rejected fare leaves balance unchanged.\n";
}
