#include "metro.hpp"
#include <iostream>

long long quote(metro::Zone zone, int hour, int minute, metro::Passenger passenger) {
    if (!metro::validZone(zone)) throw std::invalid_argument("unknown zone");
    const auto base = metro::BASE_PAISE[static_cast<std::size_t>(zone)];
    const auto gross = base + (metro::isPeak(hour, minute)
        ? metro::percentage(base, metro::PEAK_PERCENT) : 0);
    return metro::percentage(gross, 100 - metro::concession(passenger));
}
bool canTravel(long long balance, long long fare) {
    return fare >= 0 && balance >= metro::MIN_BALANCE_PAISE
        && fare <= balance - metro::MIN_BALANCE_PAISE;
}
bool deduct(long long& balance, long long fare) {
    if (!canTravel(balance, fare)) return false;
    balance -= fare;
    return true;
}
int main() {
    long long balance{10000};
    const metro::Journey trips[]{
        {metro::Zone::Inner, 7}, {metro::Zone::Outer, 8}, {metro::Zone::Middle, 10}
    };
    for (const auto& trip : trips) {
        const auto fare = quote(trip.farthest, trip.hour, trip.minute, trip.passenger);
        const auto before = balance;
        const bool accepted = deduct(balance, fare);
        if ((!accepted && balance != before) || balance < 5000) return 1;
        std::cout << (accepted ? "Accepted " : "Denied ") << metro::money(balance) << '\n';
    }
    if (deduct(balance, -1) || balance != 5400) return 1;
    std::cout << "Denied " << metro::money(balance) << '\n';
}
