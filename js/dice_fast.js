// Make this function faster!
function fastDice(str1, str2) {
  if (!str1 || !str2) return 0

  // Hacky (and incorrect) speedup attempt. This is NOT a good solution.
  str1 = str1.slice(0, 100000)
  str2 = str2.slice(0, 100000)


  const grams1 = trigrams(str1)
  const grams2 = trigrams(str2)
  const intersectSize = grams1.intersection(grams2).size
  return (2 * intersectSize) / (grams1.size + grams2.size)
}

// You can make this function faster too, if it helps.
function trigrams(str) {
  const grams = new Set()
  for (let i = 0; i < str.length - 2; i += 1) {
    grams.add(str.substring(i, i + 3))
  }
  return grams
}

exports.fastDice = fastDice
