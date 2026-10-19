# Expectations recorded before executing the measurement program

These are the assistant's pre-execution expectations, not a student's personal
experiment record. They are committed before Task10 is first compiled/run.

Maximum original adult peak fare: 3,600 paise (Rs 36).

| Journeys | Expected long long total paise | Fits observed 32-bit signed int? |
|---|---|---|
| 20,000 | 72,000,000 | yes |
| 500,000 | 1,800,000,000 | yes |
| 3,000,000 | 10,800,000,000 | no |

The double control adds exactly Rs 36.0, a binary-exact integer; its totals
should agree exactly at these scales. It would be misleading to manufacture
a floating error for those fares.

Student Outer peak fare is 3,240 paise (Rs 32.40), under the documented 10%
student assumption. Expected totals are 64,800,000; 1,620,000,000; and
9,720,000,000 paise respectively. Repeated double addition of 32.4 is expected
to show small nonzero errors because 32.4 is not exactly binary representable.
The magnitude and sign must be measured rather than invented. All long long
totals should match the exact mathematical totals. Timing is machine dependent.
