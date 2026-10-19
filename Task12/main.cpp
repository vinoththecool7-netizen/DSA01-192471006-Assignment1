#include "revised.hpp"
#include "test_support.hpp"

int main() {
    using revised::Zone;
    using revised::Journey;
    Checks test;
    const revised::FareTable table;
    const revised::Month october{2026, 10};
    const revised::MonthlyPass pass{Zone::Inner, Zone::Middle, october};
    std::cout << "case,expected,actual,result\n";
    struct Case { const char* name; Journey trip; metro::Money expected; };
    const Case cases[]{
        {"Airport weekday off peak", {{Zone::Airport}, 10, 0, false, october}, 6000},
        {"Airport weekday peak", {{Zone::Airport}, 8, 0, false, october}, 7200},
        {"Airport weekend peak replaced", {{Zone::Airport}, 8, 0, true, october}, 5400},
        {"Airport weekend off peak", {{Zone::Airport}, 20, 0, true, october}, 5400},
        {"Outer weekend peak replaced", {{Zone::Outer}, 17, 0, true, october}, 2700},
        {"Inner weekend", {{Zone::Inner}, 8, 0, true, october}, 900},
        {"Airport weekend student", {{Zone::Airport}, 8, 0, true, october, metro::Passenger::Student}, 4860},
        {"Airport weekend senior", {{Zone::Airport}, 8, 0, true, october, metro::Passenger::Senior}, 4320},
        {"Airport weekday peak student", {{Zone::Airport}, 8, 0, false, october, metro::Passenger::Student}, 6480},
        {"Airport 07:59", {{Zone::Airport}, 7, 59, false, october}, 6000},
        {"Airport 10:00", {{Zone::Airport}, 10, 0, false, october}, 6000},
        {"Airport 17:00", {{Zone::Airport}, 17, 0, false, october}, 7200},
        {"Airport 20:00", {{Zone::Airport}, 20, 0, false, october}, 6000},
        {"Farthest across reversed route", {{Zone::Outer, Zone::Inner}, 10, 0, false, october}, 3000}
    };
    for (const auto& c : cases) test.equal(c.name, table.fare(c.trip), c.expected);
    const Journey covered{{Zone::Inner, Zone::Middle, Zone::Inner}, 8, 0, false, october};
    test.equal("two covered zones free", table.fare(covered, &pass), 0);
    test.equal("one covered zone free", table.fare({{Zone::Middle}, 20, 0, true, october}, &pass), 0);
    test.equal("third zone charges normally", table.fare({{Zone::Inner, Zone::Middle, Zone::Outer}, 8, 0, false, october}, &pass), 3600);
    test.equal("expired month fallback", table.fare({{Zone::Inner, Zone::Middle}, 8, 0, false, {2026, 11}}, &pass), 2400);
    test.equal("before valid month fallback", table.fare({{Zone::Inner}, 8, 0, false, {2026, 9}}, &pass), 1200);
    test.equal("different year fallback", table.fare({{Zone::Inner}, 8, 0, false, {2027, 10}}, &pass), 1200);
    const revised::MonthlyPass distant{Zone::Inner, Zone::Airport, october};
    test.equal("uncovered intermediate zone", table.fare({{Zone::Inner, Zone::Middle, Zone::Airport}, 8, 0, false, october}, &distant), 7200);
    const revised::MonthlyPass december{Zone::Inner, Zone::Outer, {2026, 12}};
    test.equal("year rollover expires", table.fare({{Zone::Outer}, 10, 0, false, {2027, 1}}, &december), 3000);
    test.rejects("same pass zones", [&] { revised::MonthlyPass bad{Zone::Inner, Zone::Inner, october}; });
    test.rejects("unknown pass zone", [&] { revised::MonthlyPass bad{Zone::Inner, static_cast<Zone>(99), october}; });
    test.rejects("invalid pass month", [] { revised::MonthlyPass bad{Zone::Inner, Zone::Outer, {2026, 13}}; });
    test.rejects("invalid journey month", [&] { table.fare({{Zone::Inner}, 8, 0, false, {2026, 0}}, &pass); });
    test.rejects("invalid journey year", [&] { table.fare({{Zone::Inner}, 8, 0, false, {0, 10}}, &pass); });
    test.rejects("empty route", [&] { table.fare({{}, 8, 0, false, october}, &pass); });
    test.rejects("invalid zone even with pass", [&] { table.fare({{static_cast<Zone>(9)}, 8, 0, false, october}, &pass); });
    test.rejects("invalid time even with pass", [&] { table.fare({{Zone::Inner}, 24, 0, false, october}, &pass); });
    test.rejects("invalid category even with pass", [&] { table.fare({{Zone::Inner}, 8, 0, false, october, static_cast<metro::Passenger>(9)}, &pass); });
    metro::SmartCard exact{12200};
    test.equal("Airport leaves exact reserve", exact.deduct(table.fare(cases[1].trip)), true);
    test.equal("Airport final reserve", exact.balance(), 5000);
    test.equal("covered pass works at reserve", exact.deduct(table.fare(covered, &pass)), true);
    metro::SmartCard poor{4999};
    test.equal("pass does not bypass reserve", poor.deduct(table.fare(covered, &pass)), false);
    metro::SmartCard recurring{5000};
    int successes = 0;
    for (int i = 0; i < 10000; ++i) if (recurring.deduct(table.fare(covered, &pass))) ++successes;
    test.equal("10000 pass journeys accepted", successes, 10000);
    test.equal("unlimited pass balance unchanged", recurring.balance(), 5000);

    int checked = 0, mismatches = 0;
    for (int m = 0; m < 1440; ++m) for (int z = 0; z < 4; ++z)
        for (int w = 0; w < 2; ++w) for (int p = 0; p < 3; ++p) {
            const metro::Money base = z == 3 ? 6000 : (z+1)*1000;
            const bool peak = (m >= 480 && m < 600) || (m >= 1020 && m < 1200);
            const int adjustment = w ? 90 : (peak ? 120 : 100);
            const int retained = p == 1 ? 90 : (p == 2 ? 80 : 100);
            const auto expected = base * adjustment * retained / 10000;
            const auto actual = table.fare({{static_cast<Zone>(z)}, m/60, m%60,
                                           w != 0, october, static_cast<metro::Passenger>(p)});
            if (actual != expected) ++mismatches;
            ++checked;
        }
    test.equal("revised exhaustive quotes", checked, 34560);
    test.equal("revised exhaustive mismatches", mismatches, 0);
    return test.finish();
}
