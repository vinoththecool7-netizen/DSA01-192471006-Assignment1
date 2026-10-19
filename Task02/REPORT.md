# Task 2 — Tokens and constants

The single original-rule definition is `include/metro.hpp`; Task06 deliberately
preserves the independent constants in the supplied skeleton, as required.

| Token kind | Examples in the submitted code | Meaning |
|---|---|---|
| Keyword | `constexpr`, `class`, `return`, `bool` | Reserved language vocabulary. |
| Identifier | `BASE_PAISE`, `SmartCard`, `balance_` | Programmer-defined names. |
| Constant/literal | `1000`, `20`, `'0'`, `"invalid time"` | Fixed literal values. A constexpr variable is a named constant, lexically an identifier. |
| Operator | `+`, `<=`, `&&`, `::` | Arithmetic, comparison, logic, scope resolution. |
| Punctuator | `{`, `}`, `;`, `,`, `(`, `)` | Delimits declarations, blocks and expressions. |

`#define OUTER 3000` is textual preprocessing: it has no C++ scope or type and
can collide with unrelated names. `const Money outer = runtimeValue;` forbids
later assignment but need not be a constant expression. `constexpr` requires
compile-time evaluation for the fare table and enables `static_assert`.
`inline constexpr` permits one header definition shared across translation units
in C++17. The typed array makes both its element type and number of entries visible.

An unscoped `enum Zone { Inner, Middle, Outer };` exposes enumerators in the
surrounding scope and permits implicit integer conversion. `enum class Zone`
requires `Zone::Inner` and an explicit cast for indexing. Even enum class values
cast from an arbitrary int must be validated before array access.

Names begin with a letter or underscore, continue with letters/digits/underscores,
are case sensitive, and cannot be keywords. Avoid reserved implementation names
(double underscores, underscore followed by uppercase, and global leading
underscores). Use PascalCase for types, camelCase for functions, clear units in
money names, and UPPER_SNAKE_CASE for shared constants in this project.

Macro alternative:
```cpp
#define INNER_PAISE 1000
#define PEAK_PERCENT 20
// Macro substitution loses the enum/array relationship and scoped lookup.
```
Both constant expressions and substituted macros take O(1) time to use. Macros
consume no runtime storage. A scalar constexpr may likewise need none when folded;
an addressable constexpr array can occupy O(z) storage (3 * sizeof(Money) here),
though an optimiser can remove it. There is no guaranteed runtime advantage to macros.
Revision: edit one named table/rate, rebuild, and rerun tests.

Run `python3 scripts/validate.py --task Task02`; the three fares, surcharge and
reserve must match `main.expected.txt`. No external input is required.
