#include "metro.hpp"
#include "test_support.hpp"
#include <array>
#include <fstream>
#include <sstream>

class ConfigFareTable {
    std::array<metro::Money, 3> fares_{};
public:
    explicit ConfigFareTable(std::istream& input) {
        for (auto& fare : fares_) {
            if (!(input >> fare) || fare <= 0 || fare > 1000000000LL)
                throw std::invalid_argument("bad fare configuration");
        }
        std::string extra;
        if (input >> extra) throw std::invalid_argument("trailing configuration data");
    }
    metro::Money fare(const metro::Journey& trip) const {
        if (!metro::validZone(trip.farthest)) throw std::invalid_argument("unknown zone");
        const auto base = fares_[static_cast<std::size_t>(trip.farthest)];
        const auto gross = base + (metro::isPeak(trip.hour, trip.minute)
            ? metro::percentage(base, metro::PEAK_PERCENT) : 0);
        return metro::percentage(gross, 100 - metro::concession(trip.passenger));
    }
};

int main() {
    Checks test;
    std::ifstream input{"fares.txt"};
    if (!input) { std::cerr << "Cannot open fares.txt\n"; return 1; }
    const ConfigFareTable configured{input};
    test.equal("configured Outer peak", configured.fare({metro::Zone::Outer, 8}), 3600);
    for (const auto& text : {"1000 2000", "-1 2000 3000", "1000 abc 3000",
                            "1000 2000 3000 extra", "1000 2000 3000 6000", "1000 2000 1000000001"}) {
        test.rejects("invalid configuration rejected", [&] { std::istringstream bad{text}; ConfigFareTable table{bad}; });
    }
    std::istringstream revised{"1100 2200 3300"};
    const ConfigFareTable revisedTable{revised};
    test.equal("revised Outer peak", revisedTable.fare({metro::Zone::Outer, 8}), 3960);
    return test.finish();
}
