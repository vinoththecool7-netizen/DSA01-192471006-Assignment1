#include "metro.hpp"
#include <iostream>

int main() {
    for (const int hour : {8, 10, 17, 20}) {
        const auto fare = metro::FareTable{}.fare({metro::Zone::Outer, hour});
        const auto expected = (hour == 8 || hour == 17) ? 3600LL : 3000LL;
        if (fare != expected) return 1;
        std::cout << hour << ":00 Outer " << metro::money(fare) << '\n';
    }
}
