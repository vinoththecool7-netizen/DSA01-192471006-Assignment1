# Task 6 — Decisions and loops

`ALGORITHM.txt` was written before compiling the implementation. `main.cpp`
completes the two TODO bodies of the supplied skeleton; the surrounding program,
including its intentionally whole-rupee balance display, is preserved. It uses
an invalid fare sentinel -1 and rejects it, without adding an include. Subtracting
the reserve from a validated balance avoids overflow from an arbitrary fare.
Its actual three-line stdout must match the supplied expected output exactly.
`boundaries.cpp` adds 08:00, 10:00, 17:00 and 20:00 journeys using the full model.

Use if/else for validation, peak/concession policy and approval. A switch names
the finite zone alternatives; break prevents fallthrough. The conditional operator
is appropriate for a small choice such as the peak extra amount, not a long
chain of policy branches. A switch with no matching case and no default simply
continues after the switch; an uninitialised fare would then be unsafe. This
implementation includes default rejection.

A range-based for is clearest for an existing array of trips. An indexed for
is useful when an index belongs in a report. A while loop fits interactive input
until EOF or quit (Task08). Continue skips invalid journeys; break exits a switch
or an input loop on quit. Neither is needed arbitrarily in a straight-line quote.

Alternative array lookup: first validate the enum, then
`return BASE_PAISE[static_cast<std::size_t>(z)] + peakExtra;`.
Both switch and array lookup are O(1) for a fixed number of zones; the compiler
may implement a switch as branches or a jump table. A three-entry long long array
requires 3*sizeof(long long)=24 bytes on this target. Adding Airport requires
a new enum value plus switch case plus table value; the array design needs the
enum and table update, with bounds checks still essential. O(n) time processes
n journeys, with O(1) auxiliary space when streaming.

Run `python3 scripts/validate.py --task Task06`. No external input is required.
