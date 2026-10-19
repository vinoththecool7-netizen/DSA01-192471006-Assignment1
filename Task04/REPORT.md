# Task 4 — Implicit conversions

`prediction.txt` was saved before first execution. `main.cpp` is the supplied
program, including the deliberate signed/unsigned comparison. The exact expected
stdout is separate so the validator can compare it automatically. Personal
student prediction evidence cannot be supplied by the assistant.

1. `a / b` uses integer division, yielding 3; assignment converts it to double
   3.0, printed as `3`. Casting a before division causes b to convert to double,
   giving 3.5. Casting after division cannot recover the fraction.
2. `'A'` promotes to int for addition; the representable constant `'A'+2`
   converts back to char C on the target. `ch+1` promotes char to int, printing 68.
3. Nonzero int 5 converts to bool true. The stream first prints 1, then the
   boolalpha manipulator changes later bool output to `true`.
4. The usual arithmetic conversions change s=-1 into unsigned UINT_MAX.
   Comparing that to unsigned 1 is false, so the final line is `s >= u`.

`corrected.cpp` makes the intended numeric conversions explicit and tests
negativity before converting a signed value for comparison. Its final comparison
therefore prints `s < u`; that is an intentional semantic correction, not an
unexpected prediction mismatch. `int x{3.7};` is ill formed and is kept in a
separate non-build source. Its actual failed compilation is recorded separately.

Core conversions audit: validated Zone explicitly casts to size_t for lookup;
integer literals initialise Money by exact widening; percentage intermediates
promote percent to Money; comparisons use signed Money operands; stream insertion
formats integers without floating conversion; fixed bool conditions need no
numeric cast. Task03 explicitly converts its mathematical reference to double
only for the precision experiment. Task08 checks long long input range before an
explicit int or enum cast. sizeof returns size_t and is printed directly.

Explicit casts do not inherently change O(1) time or storage, and numeric casts
usually compile to the same operations. Brace initialisation rejects narrowing
at compile time. A static_cast deliberately permits many narrowing operations,
so it is not a runtime range check; validate first. Simply writing casts around
the buggy signed comparison would preserve the bug instead of fixing it.

Run `python3 scripts/validate.py --task Task04`. No external input is required.
