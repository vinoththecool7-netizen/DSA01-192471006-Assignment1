#include "metro.hpp"
#include <iostream>

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
    for (const auto fare : {1000LL, 3600LL, 2000LL, -1LL}) {
        const auto before = balance;
        const bool accepted = deduct(balance, fare);
        if ((!accepted && balance != before) || balance < 5000) return 1;
        std::cout << (accepted ? "Accepted " : "Denied ") << metro::money(balance) << '\n';
    }
}
