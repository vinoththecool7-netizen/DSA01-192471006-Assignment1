#ifndef SAFE_INPUT_HPP
#define SAFE_INPUT_HPP
#include <istream>
#include <limits>
#include <ostream>
#include <string>

namespace metro {
// One integer per line. Reject numeric prefixes such as 8abc as well as letters.
inline bool readInteger(std::istream& input, std::ostream& output,
                        const char* label, long long low, long long high, long long& value) {
    while (true) {
        output << label << '\n';
        long long candidate{};
        if (!(input >> candidate)) {
            if (input.eof()) return false;
            input.clear();
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            output << "Invalid integer; try again.\n";
            continue;
        }
        std::string rest;
        std::getline(input, rest);
        if (rest.find_first_not_of(" \t\r") != std::string::npos
            || candidate < low || candidate > high) {
            output << "Out of range or extra text; try again.\n";
            continue;
        }
        value = candidate;
        return true;
    }
}
}
#endif
