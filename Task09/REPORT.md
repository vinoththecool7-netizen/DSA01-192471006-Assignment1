# Task 9 — Complete fare logic and validation

The complete engine is `include/metro.hpp`; the operator-facing implementation
is `Task08/gate.cpp`. Tests use explicit expected values and independent oracles,
not comparisons between two calls to the same fare function.

Run `python3 scripts/validate.py --task Task09`. `evidence/tests.stdout.txt`
is the expected/actual test table (paise, booleans as 0/1). Every named case prints
PASS or FAIL and any failure gives a nonzero process exit. `input_tests.cpp`
separately tests letter/prefix/overflow recovery and EOF behaviour.

Coverage: all three zones peak and off peak; one minute before and at each
boundary; midnight and 23:59; student/senior pricing; exact reserve and one-paisa
short; unknown/negative zone, invalid time/category; negative/zero recharge;
positive recharge; overflow recharge; extreme fares; half-paisa rounding; and
correct zero-padding. An exhaustive pass checks all 1,440 minutes × 3 zones ×
3 passenger categories = 12,960 quotes against a minutes-based oracle. The
summarised mismatch count covers every combination without a 12,960-row transcript.

Five-journey calculation made from the stated assumptions:

| Journey | Arithmetic in rupees | Expected paise |
|---|---|---|
| Outer 08:00 adult | 30 × 1.20 | 3600 |
| Inner 07:00 student | 10 × 0.90 | 900 |
| Middle 10:00 senior | 20 × 0.80 | 1600 |
| Outer 20:00 adult | 30 | 3000 |
| Inner 17:00 student | 10 × 1.20 × 0.90 | 1080 |
| Total | Rs 101.80 | 10180 |

Starting with 20,000 paise leaves 9,820 paise, so all five are accepted. These
expected constants are independent of the implementation and checked in code.

Alternative table tests store tuples (input,expected) and iterate once. They cost
O(t) time and O(t) stored cases, versus O(t) handwritten checks and O(1) auxiliary
storage beyond code/constants. A new boundary is one tuple. Generated exhaustive
tests use O(1) auxiliary space and O(t) time. Tests do not prove correctness for
every possible Money value, but selected extremes and sanitizers cover important
arithmetic and invalid-index risks. Production authentication, concurrency,
persistence and real route validation remain outside this assignment.
