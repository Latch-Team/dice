#include "dice_fast.h"

#include <string>
#include <unordered_set>

// You can make this function faster too, if it helps.
static std::unordered_set<std::string> trigrams(std::string_view str) {
  std::unordered_set<std::string> grams;
  if (str.size() < 3) return grams;
  for (size_t i = 0; i + 3 <= str.size(); i += 1) {
    grams.emplace(str.substr(i, 3));
  }
  return grams;
}

// You can make this function faster too, if it helps.
static std::unordered_set<std::string> intersection(
    const std::unordered_set<std::string>& set1,
    const std::unordered_set<std::string>& set2) {
  std::unordered_set<std::string> output;
  for (const auto& elem : set1) {
    for (const auto& compare : set2) {
      if (elem == compare) {
        output.insert(elem);
      }
    }
  }
  return output;
}

// Make this function faster!
double fastDice(std::string_view str1, std::string_view str2) {
  if (str1.empty() || str2.empty()) return 0;

  // Hacky (and incorrect) speedup attempt. This is NOT a good solution.
  str1 = str1.substr(0, std::min<size_t>(str1.size(), 100000));
  str2 = str2.substr(0, std::min<size_t>(str2.size(), 100000));

  const auto grams1 = trigrams(str1);
  const auto grams2 = trigrams(str2);
  const auto intersectSize = intersection(grams1, grams2).size();
  return (2.0 * intersectSize) / (grams1.size() + grams2.size());
}
