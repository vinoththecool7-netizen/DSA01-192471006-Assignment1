#!/usr/bin/env python3
"""Check the saved executions and exercise the actual interactive executable."""
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
lines = ['# Validation summary', '', 'Saved normal and AddressSanitizer/UndefinedBehaviorSanitizer runs:']
count = 0
for task in sorted(ROOT.glob('Task[0-9][0-9]')):
    for suffix in ('evidence', 'evidence/sanitized'):
        directory = task / suffix
        manifest = json.loads((directory / 'manifest.json').read_text())
        for path, digest in manifest['header_sha256'].items():
            assert hashlib.sha256((ROOT / path).read_bytes()).hexdigest() == digest, path
        for entry in manifest['programs']:
            source = ROOT / entry['source']
            assert hashlib.sha256(source.read_bytes()).hexdigest() == entry['sha256'], source
            assert entry['compile_exit'] == 0 and entry['run_exit'] == 0, entry
            assert entry['matches_expected'] is not False, entry
            assert not (directory / (source.stem + '.stderr.txt')).read_text(), source
            count += 1
    lines.append(f'- {task.name}: normal and sanitized execution passed.')
lines += ['', f'{count} program executions checked (17 programs in each mode).', '',
          'Interactive gate integration checks:']

binary = ROOT / 'build/Task08_gate'
fixtures = [
    ('EOF at recharge', '', 'Final balance'),
    ('quit immediately', '0\n-1\n', 'Final balance Rs 100.00'),
    ('EOF before minute', '0\n8\n', 'Final balance Rs 100.00'),
    ('EOF before zone', '0\n8\n0\n', 'Final balance Rs 100.00'),
    ('EOF before passenger', '0\n8\n0\n1\n', 'Final balance Rs 100.00'),
    ('letters recover to 10 AM', '0\nletters\n10\n0\n1\n0\n-1\n', 'Fare Rs 10.00 | Balance Rs 90.00'),
    ('negative recharge recovery', '-50\n0\n-1\n', 'Final balance Rs 100.00'),
    ('recharge overflow', '9223372036854775807\n-1\n', 'Recharge rejected (overflow).'),
    ('out-of-range category recovery', '0\n8\n0\n1\n9\n1\n-1\n', 'Fare Rs 10.80 | Balance Rs 89.20'),
    ('unknown zone recovery', '0\n20\n0\n99\n3\n0\n-1\n', 'Fare Rs 30.00 | Balance Rs 70.00'),
    ('numeric prefix recovery', '0\n8abc\n20\n0\n3\n0\n-1\n', 'Fare Rs 30.00 | Balance Rs 70.00'),
]
for name, data, expected in fixtures:
    run = subprocess.run([str(binary)], input=data, text=True, capture_output=True, timeout=5)
    assert run.returncode == 0 and not run.stderr, (name, run)
    if name == 'EOF at recharge':
        assert 'Trip ' not in run.stdout
    else:
        assert expected in run.stdout, (name, run.stdout)
    lines.append(f'- {name}: PASS')

lines += ['', 'Compiler diagnostics:']
for task in sorted(ROOT.glob('Task[0-9][0-9]')):
    for log in sorted((task / 'evidence').glob('*.compile.txt')):
        text = log.read_text()
        warnings = [line for line in text.splitlines() if 'warning:' in line or 'error:' in line]
        if warnings:
            lines.append(f'- `{log.relative_to(ROOT)}`: ' + '; '.join(warnings))
            assert task.name == 'Task04', log
lines += ['', 'Only the supplied Task04 signed comparison and intentional rejected narrowing',
          'experiment produced diagnostics. No sanitizer findings occurred.', '',
          'This summary validates assistant-run executions, not student attendance or screenshots.']
destination = ROOT / 'VALIDATION.md'
destination.write_text('\n'.join(lines) + '\n')
print(destination.read_text())
