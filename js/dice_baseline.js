// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Dice%27s_coefficient#Javascript

// DO NOT MODIFY THIS FUNCTION! This is a baseline. Modify dice_fast.js.
function trigrams(str) {
  const grams = new Set()
  for (let i = 0; i < str.length - 2; i += 1) {
    grams.add(str.substring(i, i + 3))
  }
  return grams
}

// DO NOT MODIFY THIS FUNCTION! This is a baseline. Modify dice_fast.js.
function simpleDice(str1, str2) {
  if (!str1 || !str2) return 0

  const grams1 = trigrams(str1)
  const grams2 = trigrams(str2)
  const intersectSize = grams1.intersection(grams2).size
  return (2 * intersectSize) / (grams1.size + grams2.size)
}

exports.simpleDice = simpleDice
