#ifndef METRO_HPP
#define METRO_HPP

#include <array>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace metro {
using Money = long long; // Signed paise; reject negative values at boundaries.
enum class Zone { Inner, Middle, Outer };
enum class Passenger { Adult, Student, Senior };
inline constexpr std::array<Money, 3> BASE_PAISE{1000, 2000, 3000};
inline constexpr int PEAK_PERCENT = 20;
// Assumed rates: the assignment specifies eligibility but gives no percentages.
inline constexpr int STUDENT_PERCENT = 10;
inline constexpr int SENIOR_PERCENT = 20;
inline constexpr Money MIN_BALANCE_PAISE = 5000;

inline bool validZone(Zone z) {
    return z == Zone::Inner || z == Zone::Middle || z == Zone::Outer;
}
inline bool isPeak(int hour, int minute = 0) {
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        throw std::invalid_argument("invalid time");
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}
inline int concession(Passenger p) {
    switch (p) {
    case Passenger::Adult: return 0;
    case Passenger::Student: return STUDENT_PERCENT;
    case Passenger::Senior: return SENIOR_PERCENT;
    }
    throw std::invalid_argument("invalid passenger");
}
// Round half up to one paisa. Quotient/remainder avoid amount * percent overflow.
inline Money percentage(Money amount, int percent) {
    if (amount < 0 || percent < 0 || percent > 100)
        throw std::invalid_argument("invalid percentage operand");
    return (amount / 100) * percent + ((amount % 100) * percent + 50) / 100;
}
inline std::string money(Money amount) {
    if (amount < 0) throw std::invalid_argument("negative money");
    std::ostringstream out;
    out << "Rs " << amount / 100 << '.' << std::setw(2)
        << std::setfill('0') << amount % 100;
    return out.str();
}
struct Journey {
    Zone farthest;
    int hour;
    int minute = 0;
    Passenger passenger = Passenger::Adult;
};
class FareTable {
public:
    Money fare(const Journey& trip) const {
        if (!validZone(trip.farthest)) throw std::invalid_argument("unknown zone");
        const bool peak = isPeak(trip.hour, trip.minute);
        const int discount = concession(trip.passenger);
        const auto index = static_cast<std::size_t>(trip.farthest);
        const Money base = BASE_PAISE[index];
        const Money gross = base + (peak ? percentage(base, PEAK_PERCENT) : 0);
        return percentage(gross, 100 - discount);
    }
};
class SmartCard {
    Money balance_;
public:
    explicit SmartCard(Money balance) : balance_{balance} {
        if (balance < 0) throw std::invalid_argument("negative initial balance");
    }
    Money balance() const { return balance_; }
    bool canTravel(Money fare) const {
        // Do not subtract an untrusted fare until its range has been checked.
        return fare >= 0 && balance_ >= MIN_BALANCE_PAISE
            && fare <= balance_ - MIN_BALANCE_PAISE;
    }
    bool deduct(Money fare) {
        if (!canTravel(fare)) return false;
        balance_ -= fare;
        return true;
    }
    bool recharge(Money amount) {
        if (amount <= 0 || amount > std::numeric_limits<Money>::max() - balance_)
            return false;
        balance_ += amount;
        return true;
    }
};
} // namespace metro
#endif
