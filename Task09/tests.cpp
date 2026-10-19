#include "metro.hpp"
#include "test_support.hpp"
#include <limits>
#include <vector>

int main() {
    using namespace metro;
    Checks test;
    std::cout << "case,expected,actual,result\n";
    const FareTable fares;
    struct Case { const char* name; Journey trip; Money expected; };
    const std::vector<Case> cases{
        {"Inner off peak", {Zone::Inner, 7}, 1000},
        {"Middle off peak", {Zone::Middle, 12}, 2000},
        {"Outer off peak", {Zone::Outer, 20}, 3000},
        {"Inner peak", {Zone::Inner, 8}, 1200},
        {"Middle peak", {Zone::Middle, 17}, 2400},
        {"Outer peak", {Zone::Outer, 19}, 3600},
        {"07:59", {Zone::Outer, 7, 59}, 3000},
        {"08:00", {Zone::Outer, 8, 0}, 3600},
        {"09:59", {Zone::Outer, 9, 59}, 3600},
        {"10:00", {Zone::Outer, 10, 0}, 3000},
        {"16:59", {Zone::Outer, 16, 59}, 3000},
        {"17:00", {Zone::Outer, 17, 0}, 3600},
        {"19:59", {Zone::Outer, 19, 59}, 3600},
        {"20:00", {Zone::Outer, 20, 0}, 3000},
        {"00:00", {Zone::Inner, 0, 0}, 1000},
        {"23:59", {Zone::Inner, 23, 59}, 1000},
        {"Student peak", {Zone::Outer, 8, 0, Passenger::Student}, 3240},
        {"Senior off peak", {Zone::Middle, 10, 0, Passenger::Senior}, 1600}
    };
    for (const auto& c : cases) test.equal(c.name, fares.fare(c.trip), c.expected);
    test.rejects("unknown zone", [&] { fares.fare({static_cast<Zone>(99), 8}); });
    test.rejects("negative zone", [&] { fares.fare({static_cast<Zone>(-1), 8}); });
    test.rejects("hour -1", [&] { fares.fare({Zone::Inner, -1}); });
    test.rejects("hour 24", [&] { fares.fare({Zone::Inner, 24}); });
    test.rejects("minute -1", [&] { fares.fare({Zone::Inner, 8, -1}); });
    test.rejects("minute 60", [&] { fares.fare({Zone::Inner, 8, 60}); });
    test.rejects("unknown passenger", [&] { fares.fare({Zone::Inner, 8, 0, static_cast<Passenger>(9)}); });
    test.rejects("negative initial balance", [] { SmartCard bad{-1}; });
    SmartCard exact{6000};
    test.equal("exact reserve accepted", exact.deduct(1000), true);
    test.equal("exact reserve balance", exact.balance(), 5000);
    SmartCard shortCard{5999};
    test.equal("one paisa short denied", shortCard.deduct(1000), false);
    test.equal("denial unchanged", shortCard.balance(), 5999);
    test.equal("negative fare denied", exact.deduct(-1), false);
    test.equal("extreme fare denied", exact.deduct(std::numeric_limits<Money>::max()), false);
    test.equal("LLONG_MIN fare denied", exact.deduct(std::numeric_limits<Money>::min()), false);
    test.equal("negative recharge denied", exact.recharge(-100), false);
    test.equal("zero recharge denied", exact.recharge(0), false);
    test.equal("bad recharge unchanged", exact.balance(), 5000);
    test.equal("positive recharge", exact.recharge(250), true);
    test.equal("positive recharge balance", exact.balance(), 5250);
    SmartCard maximal{std::numeric_limits<Money>::max()};
    test.equal("recharge overflow denied", maximal.recharge(1), false);
    test.equal("overflow unchanged", maximal.balance(), std::numeric_limits<Money>::max());
    SmartCard poor{4999};
    test.equal("zero fare below reserve denied", poor.deduct(0), false);
    SmartCard reserve{5000};
    test.equal("zero fare at reserve accepted", reserve.deduct(0), true);
    test.equal("half paisa rounds upward", percentage(5, 10), 1);
    test.equal("below half paisa rounds down", percentage(4, 10), 0);
    test.equal("max amount identity percent", percentage(std::numeric_limits<Money>::max(), 100), std::numeric_limits<Money>::max());
    test.equal("money five paise", money(5) == "Rs 0.05", true);
    test.equal("money whole rupees", money(1000) == "Rs 10.00", true);

    int mismatches = 0;
    int checked = 0;
    for (int minuteOfDay = 0; minuteOfDay < 1440; ++minuteOfDay) {
        // Independent oracle uses minutes from midnight and literal specification fares.
        const bool peak = (minuteOfDay >= 480 && minuteOfDay < 600)
                       || (minuteOfDay >= 1020 && minuteOfDay < 1200);
        for (int z = 0; z < 3; ++z) for (int p = 0; p < 3; ++p) {
            const Money base = (z + 1) * 1000LL;
            const int retained = p == 1 ? 90 : (p == 2 ? 80 : 100);
            const Money expected = base * (peak ? 120 : 100) / 100 * retained / 100;
            const Money actual = fares.fare({static_cast<Zone>(z), minuteOfDay / 60,
                                            minuteOfDay % 60, static_cast<Passenger>(p)});
            if (actual != expected) ++mismatches;
            ++checked;
        }
    }
    test.equal("exhaustive cases checked", checked, 12960);
    test.equal("exhaustive fare mismatches", mismatches, 0);
    const std::vector<Journey> five{
        {Zone::Outer, 8}, {Zone::Inner, 7, 0, Passenger::Student},
        {Zone::Middle, 10, 0, Passenger::Senior}, {Zone::Outer, 20},
        {Zone::Inner, 17, 0, Passenger::Student}
    };
    SmartCard day{20000};
    Money total = 0;
    for (const auto& trip : five) {
        const auto fare = fares.fare(trip);
        test.equal("hand batch accepted", day.deduct(fare), true);
        total += fare;
    }
    test.equal("hand five total paise", total, 10180);
    test.equal("hand five final balance", day.balance(), 9820);
    return test.finish();
}
