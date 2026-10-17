"""Run a translator, compile its output, and check observable behavior."""
import argparse
import json
import pathlib
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
class Failure(Exception):
    pass

def run(args, data, timeout, cwd=None):
    try:
        result = subprocess.run(args, input=data, text=True, capture_output=True,
                                timeout=timeout, cwd=cwd)
    except (OSError, subprocess.TimeoutExpired) as error:
        raise Failure(str(error)) from error
    if result.returncode:
        raise Failure(f"{args[0]} exited {result.returncode}: {result.stderr[:2000]}")
    return result.stdout

def compile_run(cpp, data, timeout):
    with tempfile.TemporaryDirectory(prefix="albat-compat-") as directory:
        source = pathlib.Path(directory) / "program.cpp"
        binary = pathlib.Path(directory) / "program"
        source.write_text(cpp)
        run(["g++", "-std=gnu++20", str(source), "-o", str(binary)], "", max(timeout, 10))
        return run([str(binary)], data, timeout)

def assert_output(actual, expected, name):
    if actual != expected:
        raise Failure(f"{name}: expected {expected!r}, got {actual!r}")

def check_case(binary, case, timeout):
    source = case.get("source")
    if source is None:
        source = (ROOT / case["path"]).read_text()
    cpp = run([str(binary)], source, timeout, ROOT)
    actual = compile_run(cpp, case["stdin"], timeout)
    assert_output(actual, case["stdout"], case["id"])
    return cpp, actual

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--reference", type=pathlib.Path)
    parser.add_argument("--candidate", required=True, type=pathlib.Path)
    parser.add_argument("--cases", type=pathlib.Path, default=ROOT / "tests/fixtures/cases.json")
    parser.add_argument("--timeout", type=float, default=10)
    args = parser.parse_args()
    cases = json.loads(args.cases.read_text())
    failed = 0
    for case in cases:
        try:
            candidate_cpp, candidate_output = check_case(args.candidate.resolve(), case, args.timeout)
            if args.reference:
                reference_cpp, reference_output = check_case(args.reference.resolve(), case, args.timeout)
                assert_output(candidate_output, reference_output, case["id"])
                # Save exact generated code when diagnosing; do not normalize arbitrary identifiers.
            print("PASS", case["id"], case["classification"])
        except Failure as error:
            failed += 1
            print("FAIL", case["id"], error)
    print(f"{len(cases) - failed}/{len(cases)} passed")
    return bool(failed)

if __name__ == "__main__":
    raise SystemExit(main())
