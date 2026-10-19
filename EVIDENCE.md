# Evidence status and remaining student work

## Completed and reproducible

- Task01–Task12 each have C++ programs and a `REPORT.md` with discussion and
  alternative-concept time/space analysis. Tasks01/12 have Mermaid UML; Task06
  has structured pseudocode. The top-level report brings them together.
- All 17 programs were compiled and executed in this environment; fixed supplied
  outputs were compared exactly. Normal and sanitizer logs are under each task.
- Each manifest records the actual compiler/platform, execution time in UTC,
  command, source/header digests, stdin path and process exit codes.
- Task08 has a consumed input file; Task11 has a consumed configuration file.
  The other programs have explicit embedded input data or generated test cases.
- Tasks09/12 print named expected/actual tables, independently calculated fare
  oracles, validation errors and aggregate mismatch counts. Task10 retains raw
  measured totals and timings. Timings are illustrative single local runs.
- Task04/05 predictions were committed before execution, and Task10 expectations
  before its measurement. They are labelled as assistant-authored predictions.
- One task-specific commit per task was published through the connected GitHub
  account. The history reflects real progression in this session, not invented
  work on different dates.
- Student name confirmed by the user: **S Vinoth Kumar**, register number 192471006.

## Not supplied or not established

1. **Output screenshots:** no terminal screenshots were captured. The text logs
   are real process output, not screenshots. They have not been rendered into
   imitation terminal images. The assignment explicitly requests screenshots,
   so this part of the evidence is incomplete.
2. **Individual student execution/prediction:** these are assistant-run programs.
   The student still needs to execute every task and defend it in the viva. The
   assistant cannot establish a personal pre-execution prediction retrospectively.
3. **SIMATS pseudocode template:** the attachment names the standard but omits
   its detailed style guide. Structured pseudocode is included; exact conformity
   to an unavailable institutional template is unverified.
4. **Faculty sharing/submission:** the repository is public, but no faculty account,
   course portal or submission deadline was provided. No sharing or submission
   action on the student's behalf is claimed.
5. **Unspecified business rules:** concession percentages, calendar-month pass
   interpretation, all-visited-zone coverage, zero-fare reserve and weekend plus
   concession combination are documented assumptions requiring faculty agreement.

## Completing the screenshot requirement

Open a terminal in the repository. Execute `python3 scripts/validate.py --task
Task01`, then inspect `Task01/evidence/main.stdout.txt` and the compiler log.
Capture the genuine terminal showing the command and its output, save it under
`Task01/evidence/`, and repeat for every program listed in the README. For an
individual direct run, use the compile command recorded in the task manifest.
Include Task04's warning/rejected narrowing experiment and Task08's recovery
fixture. Do not replace genuine captures with generated illustrations.

Review each assumption, confirm the institutional template,
and commit any personal evidence with its true creation time before submitting
the public repository link to the faculty.
