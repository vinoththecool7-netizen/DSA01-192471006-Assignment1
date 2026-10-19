# Task 3 — Types and compatibility

The exact target sizes and `<climits>` ranges are printed in `evidence/main.stdout.txt`;
they are measurements, not portable language guarantees. The target uses 8-bit bytes.

| Value | Chosen type | Typical target size | Reason |
|---|---|---|---|
| Fare, balance, revenue | `long long` (Money) | 8 | Signed validation, exact integer paise, large collection headroom. |
| Peak, acceptance | `bool` | 1 | Logical state. |
| Printed fill character | `char` | 1 | `'0'` padding, not monetary arithmetic. |
| Hour, minute, percentages | `int` | 4 | Small validated ranges; negative input can be rejected. |
| Zone | `enum class Zone` | normally 4 | Named categories; underlying type is int here. |
| Day journey count | `long long` or `auto count = 3000000LL` | 8 | Avoid accidental narrow aggregate arithmetic; auto deduces long long. |
| Array index | `std::size_t` (unsigned) | target dependent | Use only after validating the signed zone. |
| Rounding demonstration | `float`, `double` | 4, 8 | Measurements only; neither stores decimal fractions exactly in general. |

Signed int's observed range is INT_MIN through INT_MAX. Maximum original adult
fare is 3,600 paise: 3,000,000 * 3,600 = 10,800,000,000 paise (Rs 108,000,000),
which does not fit the observed 32-bit int, but easily fits long long.
Multiplying `int count * int fare` overflows before assignment to long long;
use a long long operand before multiplying, as in the program. Signed overflow
is undefined behaviour, so this demonstration computes the result safely and
checks its range rather than executing an overflowing int expression.

In `i + 0.5 + true`, int 7 converts to double, then bool true promotes to int 1
and converts to double; the result is double 8.5. Assignment to a narrower type
can lose information; mixed signed/unsigned expressions need separate care (Task04).

The 10,000 additions of Rs 0.10 deliberately use a non-binary-exact fraction:
the mathematical answer is Rs 1,000 and the integer answer is exactly 100,000
paise. The saved double value and error show the actual local rounding drift.
Individual integer-paise additions are exact while the sum stays in range.

Alternative: `double balanceRupees; balanceRupees -= fareRupees;` needs explicit
rounding/tolerance rules and can corrupt a boundary comparison near Rs 50.
Both additions cost O(1); both values occupy 8 bytes on this target, while n
repeated additions take O(n) time and O(1) auxiliary space. Floating error can
accumulate, but is not guaranteed to grow monotonically or by an equal amount
per addition. Task10 measures three scales and an exactly representable control.

Run `python3 scripts/validate.py --task Task03`. No external input is required.
