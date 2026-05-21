const path = require("path");
const { simpleDice } = require("./dice_baseline");
const { fastDice } = require("./dice_fast");
const { testCases } = require(path.join(__dirname, "..", "test_data.json"));

const fns = [simpleDice, fastDice];

const BENCH_ITERATIONS = 10;

function bench(fn) {
  let result;
  const startTime = performance.now();
  for (let i = 0; i < BENCH_ITERATIONS; i++) {
    result = fn();
  }
  const endTime = performance.now();
  return {
    result,
    elapsed: (endTime - startTime) / BENCH_ITERATIONS,
  };
}

function formatPercent(x) {
  return `${(x * 100).toFixed(1)}%`;
}

function rockets(timeDelta) {
  const n = Math.max(0, Math.floor((1 - timeDelta) * 10));
  return n ? " " + "🚀".repeat(n) : "";
}

function runTests() {
  const [baselineFn, ...alternatives] = fns;

  const results = [];
  const totalElapsed = new Array(fns.length).fill(0);
  let numFails = 0;

  for (const { str1, str2 } of testCases) {
    const baseline = bench(() => baselineFn(str1, str2));
    totalElapsed[0] += baseline.elapsed;

    const row = {
      length: str1.length,
      Baseline: baseline.result.toFixed(2),
      "Baseline ms": baseline.elapsed.toFixed(2),
    };

    alternatives.forEach((fn, i) => {
      const name = fn.name || `alternative${i + 1}`;
      const alternative = bench(() => fn(str1, str2));
      totalElapsed[i + 1] += alternative.elapsed;
      const retDelta = Math.abs(1 - alternative.result / baseline.result);
      const timeDelta = alternative.elapsed / baseline.elapsed;
      const isPass = retDelta < 0.05;
      if (!isPass) {
        numFails++;
      }
      row[`${name}`] =
        `${isPass ? "✅" : "❌"}${retDelta > 0.01 ? ` (${alternative.result.toFixed(2)}: ${formatPercent(retDelta)} off)` : ""}`;
      row[`${name} ms`] =
        `${alternative.elapsed.toFixed(2)} (${formatPercent(timeDelta)})${rockets(timeDelta)}`;
    });

    results.push(row);
  }

  console.table(results);

  console.log(`Total elapsed (baseline): ${totalElapsed[0].toFixed(2)}ms`);
  alternatives.forEach((fn, i) => {
    const name = fn.name || `alternative${i + 1}`;
    const elapsed = totalElapsed[i + 1];
    console.log(`Total elapsed (${name}): ${elapsed.toFixed(2)}ms`);
    const ratio = elapsed / totalElapsed[0];
    console.log(
      `Runtime (${name}): ${formatPercent(ratio)} of baseline${rockets(ratio)}`
    );
  });
  if (numFails > 0) {
    console.error(
      `❌ ${numFails} test case${numFails === 1 ? "" : "s"} failed.`
    );
    process.exit(1);
  }
  console.log("✅ All test cases passed");
}

runTests();
