#!/usr/bin/env python3
"""Assemble the readable submission report from per-task analysis and real logs."""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
parts = [
    '# Assignment report: Namma Metro Smart-Card Fare System',
    '',
    '**Student name:** S Vinoth Kumar',
    '**Register number:** 192471006',
    '**Repository:** https://github.com/vinoththecool7-netizen/DSA01-192471006-Assignment1',
    '**Execution environment:** Apple clang 21.0.0, C++17, arm64 macOS.',
    '**Evidence provenance:** assistant-authored analysis and assistant-run executions.',
    '',
    'This report follows the attached Tasks 1–12. Per-task source, inputs and logs',
    'are retained in the linked repository. The complete assignment text is in',
    '`ASSIGNMENT_SPECIFICATION.txt`. Full execution commands and digests are in',
    'each `evidence/manifest.json`; the summary is in `VALIDATION.md`.',
    '',
    'Evidence limits: genuine text transcripts are included, but terminal screenshots,',
    'student personal execution/prediction evidence, and',
    'confirmation against the unavailable SIMATS style guide remain incomplete.',
    'Concession/pass ambiguities are documented assumptions. All task commits reflect',
    'this actual work session; no week-long personal development history is claimed.',
    '',
    '## Contents',
    '',
]
for number in range(1, 13):
    parts.append(f'- Task {number}: `Task{number:02d}/REPORT.md` and its executed programs')
for task in sorted(ROOT.glob('Task[0-9][0-9]')):
    analysis = (task / 'REPORT.md').read_text()
    # Demote headings without changing preprocessor directives inside code fences.
    in_fence = False
    adjusted = []
    for line in analysis.splitlines():
        if line.startswith('```'):
            in_fence = not in_fence
        adjusted.append('#' + line if line.startswith('#') and not in_fence else line)
    analysis = '\n'.join(adjusted)
    parts += ['', '---', '', analysis]
    algorithm = task / 'ALGORITHM.txt'
    if algorithm.exists():
        parts += ['', '### Submitted pseudocode', '', '```text', algorithm.read_text().rstrip(), '```']
    predictions = [task / 'prediction.txt', task / 'EXPECTATIONS.md']
    for prediction in predictions:
        if prediction.exists():
            parts += ['', '### Saved pre-execution expectation', '', '```text', prediction.read_text().rstrip(), '```']
    parts += ['', '### Observed execution', '']
    for log in sorted((task / 'evidence').glob('*.stdout.txt')):
        parts += [f'**{log.name}**', '', '```text', log.read_text().rstrip(), '```', '']
    if task.name == 'Task04':
        for name in ('main.compile.txt', 'narrowing.compile.txt'):
            parts += [f'**Actual compiler diagnostics: {name}**', '', '```text',
                      (task / 'evidence' / name).read_text().rstrip(), '```', '']
parts += ['', '---', '', (ROOT / 'EVIDENCE.md').read_text(), '', (ROOT / 'VALIDATION.md').read_text()]
(ROOT / 'REPORT.md').write_text('\n'.join(parts) + '\n')
print('Created REPORT.md from task analysis and actual execution evidence.')
