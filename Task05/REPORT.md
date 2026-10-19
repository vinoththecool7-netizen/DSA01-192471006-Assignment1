# Task 5 — Expressions and precedence

The pre-execution prediction is in `prediction.txt`; compare `main.expected.txt`
with `evidence/main.stdout.txt`. Explanations, line by line:

1. Multiplication precedes addition: 5+3*2=11. Parentheses force addition
   first in (5+3)*2=16. Stream insertion then prints the already grouped values.
2. Postincrement x++ supplies old 5 then sets x=6; preincrement ++y sets y=4
   and supplies 4; z=9. The operands modify different objects, so their unspecified
   relative evaluation order cannot affect the result.
3. Relational `>` binds more tightly than `?:`; the true branch yields x=6.
   `%` and `*` have equal precedence and associate left: (6%4)*2=4.
4. Unary minus precedes division/remainder. Integer division truncates toward
   zero: 10/3=3, -10/3=-3. Remainders are 1 and -1, obeying a=(a/b)*b+a%b
   and taking the dividend's sign when nonzero. Dividing by zero is invalid.
5. The loop visits i=0,3,6,9. Compound assignments add into k, producing 18.
   `i += 3` means update by three with the left operand evaluated once.

Peak: `(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)`.
Relational comparisons bind before &&, and && before ||. Omitting parentheses
is equivalent but obscures the two intervals. && and || short circuit.
The core validates hour before testing. Base surcharge is
`base + (peak ? percentage(base,20) : 0)`; the parenthesised conditional chooses
the extra amount before addition. Concession is
`percentage(gross,100-concession)` and follows the surcharge. Every percentage
operation uses integer paise and explicit rounding. `balance -= fare` performs
the mutation only after the reserve test.

`++x` yields the updated value; `x++` yields the previous value. Avoid `i=i++`:
in C++17 the right side is sequenced before assignment, so it ultimately writes
the old value back and does not advance i. In earlier standards this could be
undefined. It is a mistake to call it unconditionally undefined in C++17.
Precedence determines grouping, not general operand evaluation order.

Alternative: precompute `bool peak[24]` and index only after checking 0..23.
The demonstration exhaustively checks the table against the expression. Both
checks take O(1) time; a table has one load, the expression several comparisons
with possible short circuit. No universal timing improvement is claimed. The
table adds 24*sizeof(bool) bytes (24 on the measured target); generating it takes
O(24) startup work, or compile-time work for a constexpr implementation.

Run `python3 scripts/validate.py --task Task05`. No external input is required.
