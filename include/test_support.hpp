#ifndef TEST_SUPPORT_HPP
#define TEST_SUPPORT_HPP
#include <iostream>
#include <stdexcept>
#include <string>

struct Checks {
    int passed{0};
    int failed{0};
    void equal(const std::string& name, long long actual, long long expected) {
        const bool ok = actual == expected;
        (ok ? passed : failed)++;
        std::cout << name << ',' << expected << ',' << actual << ',' << (ok ? "PASS" : "FAIL") << '\n';
    }
    template<class Function> void rejects(const std::string& name, Function operation) {
        bool rejected = false;
        try { operation(); }
        catch (const std::invalid_argument&) { rejected = true; }
        equal(name, rejected, true);
    }
    int finish() const {
        std::cout << "Summary: " << passed << " passed; " << failed << " failed\n";
        return failed == 0 ? 0 : 1;
    }
};
#endif
