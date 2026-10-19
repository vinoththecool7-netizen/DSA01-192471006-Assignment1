#include "safe_input.hpp"
#include "test_support.hpp"
#include <sstream>

int main() {
    Checks test;
    for (const auto& bad : {"letters\n", "8abc\n", "24\n", "-2\n",
                           "9999999999999999999999999999999999\n"}) {
        std::istringstream input{std::string{bad} + "10\n"};
        std::ostringstream output;
        long long result{-9};
        test.equal(std::string{"recovery from "} + bad, metro::readInteger(input, output, "Hour:", 0, 23, result), true);
        test.equal("recovered hour", result, 10);
    }
    std::istringstream empty;
    std::ostringstream output;
    long long value{123};
    test.equal("EOF returns false", metro::readInteger(empty, output, "Hour:", 0, 23, value), false);
    test.equal("EOF leaves output untouched", value, 123);
    std::istringstream finalLine{"20"};
    test.equal("final line without newline", metro::readInteger(finalLine, output, "Hour:", 0, 23, value), true);
    test.equal("final hour", value, 20);
    return test.finish();
}
