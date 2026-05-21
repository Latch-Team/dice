import json
import math
import sys
from pathlib import Path
from time import perf_counter
from typing import Callable

from dice_baseline import simple_dice
from dice_fast import fast_dice

BENCH_ITERATIONS = 10

TEST_DATA_PATH = Path(__file__).resolve().parent.parent / "test_data.json"


def bench(fn: Callable[[], float]) -> tuple[float, float]:
    result = 0.0
    start = perf_counter()
    for _ in range(BENCH_ITERATIONS):
        result = fn()
    end = perf_counter()
    return result, (end - start) * 1000 / BENCH_ITERATIONS


def format_percent(x: float) -> str:
    return f"{x * 100:.1f}%"


def rockets(time_delta: float) -> str:
    n = max(0, math.floor((1 - time_delta) * 10))
    return " " + ("🚀" * n) if n else ""


def main() -> int:
    with TEST_DATA_PATH.open() as f:
        data = json.load(f)
    test_cases = data["testCases"]

    alternatives: list[tuple[str, Callable[[str, str], float]]] = [
        ("fastDice", fast_dice),
    ]

    total_baseline = 0.0
    total_alt = [0.0] * len(alternatives)
    num_fails = 0

    header = ["index", "length", "Baseline", "Baseline ms"]
    for name, _ in alternatives:
        header.extend([name, f"{name} ms"])
    widths = [6, 8, 9, 12] + [24, 22] * len(alternatives)
    print("".join(f"{h:<{w}}" for h, w in zip(header, widths)))

    for idx, tc in enumerate(test_cases):
        str1, str2 = tc["str1"], tc["str2"]
        base_result, base_ms = bench(lambda: simple_dice(str1, str2))
        total_baseline += base_ms

        row = [
            str(idx),
            str(len(str1)),
            f"{base_result:.2f}",
            f"{base_ms:.2f}",
        ]

        for i, (name, fn) in enumerate(alternatives):
            alt_result, alt_ms = bench(lambda fn=fn: fn(str1, str2))
            total_alt[i] += alt_ms

            ret_delta = (
                abs(alt_result)
                if base_result == 0
                else abs(1 - alt_result / base_result)
            )
            time_delta = alt_ms / base_ms if base_ms > 0 else 0
            is_pass = ret_delta < 0.05
            if not is_pass:
                num_fails += 1

            status = "✅" if is_pass else "❌"
            if ret_delta > 0.01:
                status += f" ({alt_result:.2f}: {format_percent(ret_delta)} off)"

            row.append(status)
            row.append(
                f"{alt_ms:.2f} ({format_percent(time_delta)}){rockets(time_delta)}"
            )

        print("".join(f"{cell:<{w}}" for cell, w in zip(row, widths)))

    print(f"\nTotal elapsed (baseline): {total_baseline:.2f}ms")
    for i, (name, _) in enumerate(alternatives):
        elapsed = total_alt[i]
        print(f"Total elapsed ({name}): {elapsed:.2f}ms")
        ratio = elapsed / total_baseline if total_baseline > 0 else 0
        print(
            f"Runtime ({name}): {format_percent(ratio)} of baseline{rockets(ratio)}"
        )

    if num_fails > 0:
        plural = "" if num_fails == 1 else "s"
        print(f"❌ {num_fails} test case{plural} failed.", file=sys.stderr)
        return 1
    print("✅ All test cases passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
