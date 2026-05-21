#include "dice_baseline.h"
#include "dice_fast.h"

#include <nlohmann/json.hpp>

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <vector>

using json = nlohmann::json;
using Clock = std::chrono::steady_clock;

namespace {

constexpr int BENCH_ITERATIONS = 10;

struct BenchResult {
  double result;
  double elapsedMs;  // average ms per call
};

BenchResult bench(const std::function<double()>& fn) {
  double result = 0;
  const auto start = Clock::now();
  for (int i = 0; i < BENCH_ITERATIONS; i++) {
    result = fn();
  }
  const auto end = Clock::now();
  const double totalMs =
      std::chrono::duration<double, std::milli>(end - start).count();
  return {result, totalMs / BENCH_ITERATIONS};
}

std::string formatPercent(double x) {
  char buf[32];
  std::snprintf(buf, sizeof(buf), "%.1f%%", x * 100.0);
  return buf;
}

std::string rockets(double timeDelta) {
  int n = static_cast<int>(std::floor((1.0 - timeDelta) * 10.0));
  if (n < 0) n = 0;
  if (n == 0) return "";
  std::string out = " ";
  for (int i = 0; i < n; i++) out += "🚀";
  return out;
}

std::filesystem::path findTestData() {
  // Search upward from CWD and from the binary's directory for test_data.json.
  for (auto dir : {std::filesystem::current_path(),
                   std::filesystem::current_path().parent_path()}) {
    auto candidate = dir / "test_data.json";
    if (std::filesystem::exists(candidate)) return candidate;
  }
  return "test_data.json";
}

}  // namespace

int main() {
  const auto testDataPath = findTestData();
  std::ifstream f(testDataPath);
  if (!f) {
    std::fprintf(stderr, "Could not open %s\n", testDataPath.string().c_str());
    return 2;
  }
  const json data = json::parse(f);
  const auto& testCases = data.at("testCases");

  struct Alt {
    const char* name;
    std::function<double(std::string_view, std::string_view)> fn;
  };
  const std::vector<Alt> alternatives = {
      {"fastDice", fastDice},
  };

  double totalBaseline = 0;
  std::vector<double> totalAlt(alternatives.size(), 0.0);
  int numFails = 0;

  // Header
  std::printf("%-6s %-8s %-9s %-12s", "index", "length", "Baseline",
              "Baseline ms");
  for (const auto& a : alternatives) {
    std::printf(" %-24s %-22s", a.name,
                (std::string(a.name) + " ms").c_str());
  }
  std::printf("\n");

  int idx = 0;
  for (const auto& tc : testCases) {
    const std::string str1 = tc.at("str1").get<std::string>();
    const std::string str2 = tc.at("str2").get<std::string>();

    const auto baseline =
        bench([&] { return simpleDice(str1, str2); });
    totalBaseline += baseline.elapsedMs;

    char baseStr[16], baseMsStr[16];
    std::snprintf(baseStr, sizeof(baseStr), "%.2f", baseline.result);
    std::snprintf(baseMsStr, sizeof(baseMsStr), "%.2f", baseline.elapsedMs);
    std::printf("%-6d %-8zu %-9s %-12s", idx, str1.size(), baseStr, baseMsStr);

    for (size_t i = 0; i < alternatives.size(); i++) {
      const auto& alt = alternatives[i];
      const auto altRes = bench([&] { return alt.fn(str1, str2); });
      totalAlt[i] += altRes.elapsedMs;

      const double retDelta = baseline.result == 0
                                  ? std::abs(altRes.result)
                                  : std::abs(1.0 - altRes.result / baseline.result);
      const double timeDelta = altRes.elapsedMs / baseline.elapsedMs;
      const bool isPass = retDelta < 0.05;
      if (!isPass) numFails++;

      std::string status = isPass ? "✅" : "❌";
      if (retDelta > 0.01) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), " (%.2f: %s off)", altRes.result,
                      formatPercent(retDelta).c_str());
        status += buf;
      }

      char timeStr[64];
      std::snprintf(timeStr, sizeof(timeStr), "%.2f (%s)%s", altRes.elapsedMs,
                    formatPercent(timeDelta).c_str(),
                    rockets(timeDelta).c_str());

      std::printf(" %-24s %-22s", status.c_str(), timeStr);
    }
    std::printf("\n");
    idx++;
  }

  std::printf("\nTotal elapsed (baseline): %.2fms\n", totalBaseline);
  for (size_t i = 0; i < alternatives.size(); i++) {
    const double elapsed = totalAlt[i];
    std::printf("Total elapsed (%s): %.2fms\n", alternatives[i].name, elapsed);
    const double ratio = elapsed / totalBaseline;
    std::printf("Runtime (%s): %s of baseline%s\n", alternatives[i].name,
                formatPercent(ratio).c_str(), rockets(ratio).c_str());
  }

  if (numFails > 0) {
    std::fprintf(stderr, "❌ %d test case%s failed.\n", numFails,
                 numFails == 1 ? "" : "s");
    return 1;
  }
  std::printf("✅ All test cases passed\n");
  return 0;
}
