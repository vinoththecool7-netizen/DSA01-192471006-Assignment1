# Namma Metro Smart-Card Fare System — Assignment 1

**Repository:** https://github.com/vinoththecool7-netizen/DSA01-192471006-Assignment1

**Register number:** 192471006 (from repository name)

**Student name:** S Vinoth Kumar

**Language:** C++17

**Measured compiler:** Apple clang 21.0.0 (`clang-2100.3.34.2`), arm64 macOS.
On this machine `g++` invokes Apple clang; this is not GNU GCC 21.

Tasks 1–12 have source, explanations, complexity comparisons and actual execution
transcripts. The report is [REPORT.md](REPORT.md); the original attached text is
[ASSIGNMENT_SPECIFICATION.txt](ASSIGNMENT_SPECIFICATION.txt). See
[VALIDATION.md](VALIDATION.md) for final checks and [EVIDENCE.md](EVIDENCE.md) for
honest evidence limits. The student must review, personally execute and be able
to explain the work. Student execution screenshots are still missing.

## Build and run

Prerequisites: a C++17 compiler and Python 3 (standard library only). From the root:

```sh
python3 scripts/validate.py
python3 scripts/validate.py --sanitize
python3 scripts/check_submission.py
```

The first command compiles and runs all 17 programs, compares fixed expected
outputs where supplied, and records stdout, stderr, compiler diagnostics and
source/header digests in each task's `evidence/` directory. The second repeats
with address/undefined-behaviour sanitizers in `evidence/sanitized/`. The third
checks saved results and exercises 11 real gate input/EOF scenarios. Sanitizers
require compiler/platform support; the saved runs passed on this macOS target.
To choose a compiler: `CXX=clang++ python3 scripts/validate.py`.

For one task, e.g. `python3 scripts/validate.py --task Task06`. Every program can
be compiled directly with the exact pattern used by the runner:

```sh
mkdir -p build
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude Task06/main.cpp -o build/Task06_main
./build/Task06_main
```

Replace the source/binary names using the table below. The recorded full command
for every individual source is in its `evidence/*.compile.txt` and `manifest.json`.
Run most binaries from the root; the runner itself uses each task as working
directory. Task11 needs that working directory to load `fares.txt`:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude Task11/main.cpp -o build/Task11_main
(cd Task11 && ../build/Task11_main)
```

Run the gate interactively or with the provided input fixture:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude Task08/gate.cpp -o build/Task08_gate
./build/Task08_gate < Task08/gate.stdin.txt
```

The separate **intentionally invalid** brace-initialisation experiment is not part
of the successful builds. Reproduce its expected compiler rejection with:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -x c++ Task04/narrowing.cpp.txt -o build/narrowing
```

## Programs and observed results

Each row's source paths are relative to that task directory. Tests with no stdin
use the explicit data in the source; no unconsumed input fixture is claimed.

| Task | C++ files | Input | Observed result |
|---|---|---|---|
| [01](Task01/REPORT.md) | `main.cpp` | Embedded journey/card | Rs 36 peak fare, Rs 64 remains, excessive deduction rejected; OOP/UML analysis. |
| [02](Task02/REPORT.md) | `main.cpp` | Shared constexpr values | 1000/2000/3000 paise, 20% surcharge and Rs 50 reserve. |
| [03](Task03/REPORT.md) | `main.cpp` | 10,000 decimal additions; network maximum | Sizes/ranges measured; integer exactness and double drift demonstrated. |
| [04](Task04/REPORT.md) | `main.cpp`, `corrected.cpp` | Embedded supplied program | Saved prediction matches; signed comparison warning and separate narrowing rejection recorded. |
| [05](Task05/REPORT.md) | `main.cpp`, `expressions.cpp` | Embedded supplied program/all hours | Saved prediction matches; all 24 peak-table entries agree. |
| [06](Task06/REPORT.md) | `main.cpp`, `boundaries.cpp` | Supplied three trips; four boundary hours | Exact supplied output; 10:00 and 20:00 off peak. |
| [07](Task07/REPORT.md) | `procedural.cpp`, `oop.cpp` | Same three trips and negative-fare attempt | Both end at Rs 54 and reject invalid deductions; class and raw balance each measured as 8 bytes. |
| [08](Task08/REPORT.md) | `gate.cpp` | `gate.stdin.txt` | Invalid input recovery; final balance Rs 60; exact report fixture matches. |
| [09](Task09/REPORT.md) | `tests.cpp`, `input_tests.cpp` | Named cases/generated minutes and input streams | 56 fare/card checks and 14 input checks pass; includes 12,960 exhaustive quotes. |
| [10](Task10/REPORT.md) | `main.cpp` | 20,000/500,000/3,000,000 additions | Integer totals exact; original network exceeds int; measured fractional-double drift. |
| [11](Task11/REPORT.md) | `main.cpp` | `fares.txt`, malformed configurations | Eight configuration checks pass; final recommendation defended from measurements. |
| [12](Task12/REPORT.md) | `main.cpp` + `revised.hpp` | Named/generated journeys and passes | 39 checks pass; includes 34,560 quotes and 10,000 covered pass trips. |

Normal builds are warning-free except the **unchanged supplied Task04 program's**
deliberate signed/unsigned warning. The narrowing example is deliberately rejected.
All 17 programs passed normal and sanitizer execution; no sanitizer findings occurred.

## Model decisions

- Money uses signed `long long` paise. A card can start below Rs 50 but a successful
  journey must leave at least Rs 50; failed operations leave it unchanged.
- Peak intervals are [08:00,10:00) and [17:00,20:00), using validated local hours/minutes.
- The brief gives no concession rates. This implementation assumes student 10%,
  senior 20%, one category, applied after other pricing; percentage arithmetic
  rounds half up to a paisa. These are classroom assumptions, not official tariffs.
- Task12 assumes an already-purchased pass for one calendar month/year and two
  distinct zones; every visited zone must be covered. Weekend discount replaces
  peak surcharge, then any student/senior concession applies. Pass trips still
  obey the reserve, even with zero fare. Route and weekend data are supplied upstream.
- Original shared constants are in `include/metro.hpp`. The required Task06
  skeleton and Task12's changed scenario intentionally keep independent definitions.

## Evidence and history

The repository has one progressively published commit for each task, plus final
review/validation documentation. Task04/05 predictions appear in the earlier
Task02 commit; Task10 expectations appear in the earlier Task09 commit. No commit
dates are backdated, and this single work session does not establish a week-long
student development history. The assistant wrote the predictions; personal
student prediction evidence cannot be reconstructed afterward.

Actual captured text is retained without invented screenshots. The full SIMATS
pseudocode style guide was not attached, so exact institutional formatting remains
unverified. Faculty sharing/submission and personal
execution/screenshots remain the student's follow-up items.
