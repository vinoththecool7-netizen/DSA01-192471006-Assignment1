# Validation summary

Saved normal and AddressSanitizer/UndefinedBehaviorSanitizer runs:
- Task01: normal and sanitized execution passed.
- Task02: normal and sanitized execution passed.
- Task03: normal and sanitized execution passed.
- Task04: normal and sanitized execution passed.
- Task05: normal and sanitized execution passed.
- Task06: normal and sanitized execution passed.
- Task07: normal and sanitized execution passed.
- Task08: normal and sanitized execution passed.
- Task09: normal and sanitized execution passed.
- Task10: normal and sanitized execution passed.
- Task11: normal and sanitized execution passed.
- Task12: normal and sanitized execution passed.

34 program executions checked (17 programs in each mode).

Interactive gate integration checks:
- EOF at recharge: PASS
- quit immediately: PASS
- EOF before minute: PASS
- EOF before zone: PASS
- EOF before passenger: PASS
- letters recover to 10 AM: PASS
- negative recharge recovery: PASS
- recharge overflow: PASS
- out-of-range category recovery: PASS
- unknown zone recovery: PASS
- numeric prefix recovery: PASS

Compiler diagnostics:
- `Task04/evidence/main.compile.txt`: Task04/main.cpp:19:17: warning: comparison of integers of different signs: 'int' and 'unsigned int' [-Wsign-compare]
- `Task04/evidence/narrowing.compile.txt`: Task04/narrowing.cpp.txt:2:20: error: type 'double' cannot be narrowed to 'int' in initializer list [-Wc++11-narrowing]; Task04/narrowing.cpp.txt:2:20: warning: implicit conversion from 'double' to 'int' changes value from 3.7 to 3 [-Wliteral-conversion]

Only the supplied Task04 signed comparison and intentional rejected narrowing
experiment produced diagnostics. No sanitizer findings occurred.

This summary validates assistant-run executions, not student attendance or screenshots.
