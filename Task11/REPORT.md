# Task 11 — Recommended design

Recommend signed long long paise for monetary values, validated signed int
hours/minutes, scoped Zone/Passenger enums, typed constexpr fares/rates and a
private SmartCard balance. Keep FareTable separate from payment mutation. Quote
first, then deduct only after testing reserve equality and rejecting invalid
amounts. A failed operation must not mutate stored value.

Evidence: Task03 measures eight-byte Money and a decimal accumulation error;
Task09 verifies 56 named checks including 12,960 minute/zone/category quotes;
Task10 matches exact totals up to 10.8 billion paise, rejects the int overflow
boundary safely and measures a decimal double error at network scale. Task12
extends pricing with 34,560 more independent quote comparisons and pass tests.
These support exact integer storage and rule separation, rather than a claim
that merely declaring a class fixes arithmetic.

A fare revision edits `BASE_PAISE` or the named percentage in `include/metro.hpp`,
updates independent expected test values, recompiles and reruns the suite before
deployment. Task06 is an independent required teaching skeleton and intentionally
retains its original constants; Task12 is a separate revised scenario. They are
not three copies of configuration inside one deployed engine.

For network correctness, worst-case original daily revenue is
3,000,000×3,600=10,800,000,000 paise, safely within long long. The changed
Airport maximum gives 3,000,000×7,200 =
21,600,000,000 paise (Rs 216,000,000), also safely within long long.
The code does not accumulate an unbounded lifetime total without checks.
Fare lookup/deduction are O(1), processing n journeys is O(n) with O(1) streaming
space. This scale arithmetic does not demonstrate real-time gate throughput.

Limits: assumed concession rates, no eligibility checks, no persistence,
authentication, route graph, concurrent access, refunds, daily caps or audit
ledger. Real deployment must protect quote-and-debit against concurrent writers
and keep durable transactions. The classroom model establishes arithmetic
invariants, not a complete operational ticketing system.

Alternative configuration loading is implemented in `main.cpp`, using the
supplied `fares.txt`. It validates exactly three positive bounded integer fares
and rejects missing, malformed, negative, excessive and trailing data. A complete
valid table is constructed before use; a malformed file fails startup instead
of silently charging defaults. A revised table quotes Outer peak at 3,960 paise
without a source-code change. Actual tests are in `evidence/main.stdout.txt`.

Reading z fares takes O(z) values plus O(L) character parsing time for file length
L, O(z) table storage, and leaves O(1) indexed lookup. Bounded fields make L
proportional to z. Compile-time tables avoid runtime parsing, while file tables
avoid rebuilding executable code on every price change. Production config needs
an authenticated source, schema/version and atomic rollout. For three classroom
fares constexpr is easiest to inspect; for operational revisions validated
configuration is preferable.

Run `python3 scripts/validate.py --task Task11`; the validator runs with Task11
as working directory so `fares.txt` is resolved correctly. To run manually from
the root: `g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude Task11/main.cpp -o build/Task11_main`,
then `cd Task11` and `../build/Task11_main`.
