#!/usr/bin/env python3
"""Compile and execute task programs, saving authentic commands and results."""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import platform
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument('--task', help='Task01 through Task12; omit to run all')
parser.add_argument('--sanitize', action='store_true')
args = parser.parse_args()
cxx = os.environ.get('CXX', 'g++')
flags = ['-std=c++17', '-Wall', '-Wextra', '-Wpedantic']
if args.sanitize:
    flags += ['-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer']
tasks = [ROOT / args.task] if args.task else sorted(ROOT.glob('Task[0-9][0-9]'))
failures = []
for task in tasks:
    evidence = task / 'evidence'
    if args.sanitize:
        evidence /= 'sanitized'
    evidence.mkdir(parents=True, exist_ok=True)
    (ROOT / 'build').mkdir(exist_ok=True)
    records = []
    for source in sorted(task.glob('*.cpp')):
        binary = ROOT / 'build' / f'{task.name}_{source.stem}'
        if args.sanitize:
            binary = binary.with_name(binary.name + '_sanitized')
        cmd = [cxx, *flags, '-Iinclude', str(source.relative_to(ROOT)), '-o', str(binary.relative_to(ROOT))]
        compiled = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
        (evidence / f'{source.stem}.compile.txt').write_text(
            '$ ' + shlex.join(cmd) + '\n' + compiled.stdout + compiled.stderr
            + f'\nexit_code={compiled.returncode}\n')
        if compiled.returncode:
            failures.append(f'{task.name}/{source.name}: compilation failed')
            continue
        input_file = source.with_suffix('.stdin.txt')
        data = input_file.read_text() if input_file.exists() else ''
        run = subprocess.run([str(binary)], input=data, cwd=task, capture_output=True, text=True, timeout=60)
        (evidence / f'{source.stem}.stdout.txt').write_text(run.stdout)
        (evidence / f'{source.stem}.stderr.txt').write_text(run.stderr)
        expected = source.with_suffix('.expected.txt')
        match = run.stdout == expected.read_text() if expected.exists() else None
        if run.returncode or match is False:
            failures.append(f'{task.name}/{source.name}: exit={run.returncode}, match={match}')
        records.append({'source': str(source.relative_to(ROOT)),
                        'sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                        'compile': shlex.join(cmd), 'compile_exit': compiled.returncode,
                        'run': f'../build/{binary.name}',
                        'stdin': str(input_file.relative_to(ROOT)) if input_file.exists() else '(none)',
                        'run_exit': run.returncode, 'matches_expected': match})
        print(f'{task.name}/{source.name}: exit={run.returncode}, expected={match}')
    manifest = {'generated_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
                'platform': platform.platform(),
                'compiler': subprocess.run([cxx, '--version'], capture_output=True, text=True).stdout,
                'header_sha256': {str(p.relative_to(ROOT)): hashlib.sha256(p.read_bytes()).hexdigest()
                                  for p in sorted((ROOT / 'include').glob('*.hpp')) + sorted(task.glob('*.hpp'))},
                'programs': records}
    (evidence / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
if failures:
    print('\n'.join(failures), file=sys.stderr)
    sys.exit(1)
