#include <chrono>
#include <climits>
#include <iomanip>
#include <iostream>

int main() {
    using Clock = std::chrono::steady_clock;
    std::cout << "fare_paise,journeys,expected_paise,long_long_paise,double_rupees,error_rupees,int_fits,long_long_us,double_us\n";
    for (const long long fare : {3600LL, 3240LL}) {
        for (const long long n : {20000LL, 500000LL, 3000000LL}) {
            const long long expected = n * fare;
            long long total = 0;
            const auto startInteger = Clock::now();
            for (long long i = 0; i < n; ++i) total += fare;
            const auto stopInteger = Clock::now();
            const double rupees = static_cast<double>(fare) / 100.0;
            double floating = 0.0;
            const auto startDouble = Clock::now();
            for (long long i = 0; i < n; ++i) floating += rupees;
            const auto stopDouble = Clock::now();
            const long double exactRupees = static_cast<long double>(expected) / 100.0L;
            const long double error = static_cast<long double>(floating) - exactRupees;
            const auto integerUs = std::chrono::duration_cast<std::chrono::microseconds>(stopInteger-startInteger).count();
            const auto doubleUs = std::chrono::duration_cast<std::chrono::microseconds>(stopDouble-startDouble).count();
            std::cout << fare << ',' << n << ',' << expected << ',' << total << ','
                      << std::setprecision(18) << floating << ',' << error << ','
                      << (expected <= INT_MAX ? "yes" : "no") << ',' << integerUs << ',' << doubleUs << '\n';
            if (total != expected || (fare == 3600 && error != 0.0L)) return 1;
        }
    }
    // Stop before signed overflow. This demonstrates the failure boundary without UB.
    int narrowTotal = 0;
    long long accepted = 0;
    while (narrowTotal <= INT_MAX - 3600) { narrowTotal += 3600; ++accepted; }
    std::cout << "checked int additions before overflow=" << accepted
              << " total=" << narrowTotal << "; next addition rejected\n";
    std::cout << "sizeof int=" << sizeof(int) << " long long=" << sizeof(long long)
              << " double=" << sizeof(double) << '\n';
}
