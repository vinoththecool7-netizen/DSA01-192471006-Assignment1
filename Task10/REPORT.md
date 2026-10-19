# Task 10 — Expectations versus execution

`EXPECTATIONS.md` was committed before first execution. The local measurement
is `evidence/main.stdout.txt`; its manifest records compiler, platform, source
digest, command, UTC execution time and exit status. Values below are taken
from that run (timings may change when rerun).

| Fare rupees | Journeys | Expected and actual integer paise | double error, rupees |
|---|---|---|---|
| 36.00 | 20,000 | 72,000,000 | 0 |
| 36.00 | 500,000 | 1,800,000,000 | 0 |
| 36.00 | 3,000,000 | 10,800,000,000 | 0 |
| 32.40 | 20,000 | 64,800,000 | +0.000000244006514549 |
| 32.40 | 500,000 | 1,620,000,000 | +0.000129528343678 |
| 32.40 | 3,000,000 | 9,720,000,000 | +0.00335706770420 |

All integer results matched the pre-execution expectations. Rs 36 is exactly
representable and these totals are below 2^53, so the double control also agreed
exactly. Rs 32.40 is not binary exact; repeated rounding produced a positive
error that grew across these three measurements. At three million journeys
the measured error is about 0.336 paise, not a whole paisa. Other fare mixtures
or scales may produce different error signs and amounts; no universal linear
error law is claimed. Integer paise avoid that ambiguity entirely.

The observed 32-bit int fits station and line totals for these fares, but not
the network totals. At 3,600 paise, 596,523 additions produce 2,147,482,800;
the next would exceed INT_MAX=2,147,483,647. The checked loop stops before it,
so it does not execute undefined signed overflow or pretend a wraparound is
a valid signed-int result. long long accommodates up to
floor(9,223,372,036,854,775,807/3600) such additions from zero. A double's
limitations are binary fraction approximation and eventually the spacing
between representable values; it has no fixed-point paisa guarantee. Its
integer exactness limit 2^53 does not make decimal fractions exact below it.

Each addition is O(1); summing n fares is O(n), O(1) auxiliary space. Observed
sizes are int=4 bytes, long long=8, double=8. The program saves microsecond
timings for both loops. They are single runs of an unoptimised teaching build,
affected by warmup, scheduling and clock resolution, and are not a throughput
benchmark or a reason to trade monetary correctness for apparent speed.

Warnings: `Task04/evidence/main.compile.txt` records `-Wsign-compare`: int -1
converts to unsigned before comparison, changing the answer. The corrected
program checks negativity first and compiles cleanly. The intentional narrowing
experiment records `-Wc++11-narrowing` (an error) and `-Wliteral-conversion`
(fraction lost). It is excluded from normal successful builds. The other task
programs were compiled with `-Wall -Wextra -Wpedantic`; the final validation
summary reports their actual diagnostic status rather than suppressing it.

Run `python3 scripts/validate.py --task Task10`. No external input is required;
the two fares and three journey counts are explicit input data in the program.
