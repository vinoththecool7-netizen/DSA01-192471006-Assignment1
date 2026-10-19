# Task 7 — Procedural and object-oriented implementations

`procedural.cpp` owns a separate balance variable and calls free quote/payment functions.
`oop.cpp` quotes through FareTable and performs the same four payment operations through a private SmartCard balance;
the output prefixes must agree. Both reject a negative fare and an unaffordable
fare without mutating balance. The OOP run also measures the two representations.

The subtraction is safe because `fare >= 0`, `balance >= reserve`, and
`fare <= balance-reserve` are established first. No caller can assign to the
private balance, and no successful mutation creates a negative value. Recharge
checks positivity and overflow. The constructor rejects a negative initial value.

Both versions refer to the same reserve constant, so a change to Rs 60 is one
constant edit in either design followed by updated tests. The class additionally
prevents future callers from bypassing that constant. It does not magically make
a well-factored procedural implementation asymptotically slower or larger.

Alternative public member:
```cpp
struct PublicCard { long long balance; };
// caller: card.balance = -100; // compiles, breaking the monetary invariant
```
Public/private access control itself adds no storage and no per-access runtime
cost. Both store one long long (observed 8 bytes) and check/deduct in O(1) time;
processing n journeys takes O(n). Public state gives every caller responsibility
for the reserve, greatly increasing the locations that can violate it. Prefer
the private class and keep pricing composition separate from card storage.

Run `python3 scripts/validate.py --task Task07`. No external input is required.
