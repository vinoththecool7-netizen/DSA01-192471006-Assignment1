# Task 8 — Safe input and a journey report

Run `python3 scripts/validate.py --task Task08` to feed `gate.stdin.txt` to the
interactive gate. It starts with Rs 100, rejects a negative recharge, recovers
from letters, rejects `8abc`, hour 24 and zone 9, and then processes Inner at
10:00 and Outer at 20:00. Those cost Rs 10 and Rs 30, leaving Rs 60. An Outer
peak trip costing Rs 36 is denied, leaving Rs 60 unchanged. -1 quits; EOF also
terminates without spinning. Inputs are one integer per line in paise where stated.

`readInteger` checks stream failure, calls clear(), and ignores the entire bad
line. It validates the range before narrowing to int or enum. Reading the tail
with getline rejects a numeric prefix followed by junk. An incomplete trip at EOF
does not debit a fare. Out-of-range values and unknown zones prompt again rather
than entering the fare engine. Task09 tests recovery and EOF automatically.

For money, integer quotient/remainder and setw(2)/setfill('0') produce exact
two-digit paise, e.g. Rs 0.05; each formatting stream is local, so fill state
cannot leak into a later report. `fixed << setprecision(2)` is appropriate for a
double demonstration but converting money to double merely to display it would
introduce an unnecessary rounding step. Reports include trip number, fare,
approval and remaining balance in aligned fields separated by vertical bars.

Alternative:
```cpp
std::string line;
if (std::getline(std::cin, line)) {
    try {
        std::size_t used{};
        int value = std::stoi(line, &used);
        // Reject trailing non-whitespace and out-of-domain values before use.
    } catch (const std::invalid_argument&) { /* ask again */ }
      catch (const std::out_of_range&) { /* ask again */ }
}
```
Use stoll rather than stoi for Money. Both approaches inspect O(L) characters
per input line. A pure extraction-and-ignore approach can use O(1) auxiliary
space, whereas getline uses O(L). Our strict trailing-text check also stores
the remaining line, so this submitted implementation uses O(L) space. getline
consumes a whole bad line naturally; numeric extraction needs clear/ignore after
failure. stoi alone would wrongly accept `8abc` unless the consumed length is checked.
