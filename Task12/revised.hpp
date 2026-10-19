#ifndef REVISED_METRO_HPP
#define REVISED_METRO_HPP
#include "metro.hpp"
#include <algorithm>
#include <array>
#include <vector>

namespace revised {
using metro::Money;
enum class Zone { Inner, Middle, Outer, Airport };
inline constexpr std::array<Money, 4> BASE_PAISE{1000, 2000, 3000, 6000};
inline constexpr int WEEKEND_PERCENT = 10;
inline bool validZone(Zone z) {
    return z == Zone::Inner || z == Zone::Middle || z == Zone::Outer || z == Zone::Airport;
}
struct Month {
    int year;
    int number;
    void validate() const {
        if (year < 1 || year > 9999 || number < 1 || number > 12)
            throw std::invalid_argument("invalid calendar month");
    }
};
struct Journey {
    std::vector<Zone> visited; // All travelled zones, including intermediate zones.
    int hour;
    int minute;
    bool weekend;
    Month month;
    metro::Passenger passenger = metro::Passenger::Adult;
};
class MonthlyPass {
    Zone first_;
    Zone second_;
    Month month_;
public:
    MonthlyPass(Zone first, Zone second, Month month)
        : first_{first}, second_{second}, month_{month} {
        month.validate();
        if (!validZone(first) || !validZone(second) || first == second)
            throw std::invalid_argument("pass needs two distinct valid zones");
    }
    bool covers(const Journey& trip) const {
        if (trip.visited.empty() || trip.month.year != month_.year || trip.month.number != month_.number)
            return false;
        return std::all_of(trip.visited.begin(), trip.visited.end(), [&](Zone z) {
            return z == first_ || z == second_;
        });
    }
};
class FareTable {
public:
    Money fare(const Journey& trip, const MonthlyPass* pass = nullptr) const {
        // Validate even a potentially free journey; a pass cannot bypass invalid input.
        trip.month.validate();
        const bool peak = metro::isPeak(trip.hour, trip.minute);
        const int retained = 100 - metro::concession(trip.passenger);
        if (trip.visited.empty()) throw std::invalid_argument("empty journey");
        Money base = 0;
        for (const auto z : trip.visited) {
            if (!validZone(z)) throw std::invalid_argument("unknown zone");
            base = std::max(base, BASE_PAISE[static_cast<std::size_t>(z)]);
        }
        if (pass && pass->covers(trip)) return 0;
        // Weekend discount replaces peak adjustment; never apply both.
        const Money adjusted = trip.weekend ? metro::percentage(base, 100 - WEEKEND_PERCENT)
            : base + (peak ? metro::percentage(base, metro::PEAK_PERCENT) : 0);
        return metro::percentage(adjusted, retained);
    }
};
}
#endif
