// Make this file faster!

class DiceBloomer {
  constructor(strSize) {
    // Choose a nice prime size (to reduce modulo collisions). Smaller -> more
    // collisions (incorrectly higher Dice value) but better cache cohesion
    // (faster).
    this.size =
      strSize > 100000
        ? 91193
        : strSize > 50000
          ? 39119
          : strSize > 20000
            ? 19937
            : 11939
    this.bitmaps = new Array(2).fill().map((_) => new Array(this.size).fill(0))
  }

  // returns a number between 0 and 1<<24
  trigram(str, i) {
    return (
      (str.charCodeAt(i) << 16) |
      (str.charCodeAt(i + 1) << 8) |
      str.charCodeAt(i + 2)
    )
  }

  bloom(str, dim) {
    for (let i = 0; i < str.length - 2; i++) {
      this.bitmaps[dim][this.trigram(str, i) % this.size] = 1
    }
  }

  score() {
    let overlaps = 0
    let total = 0
    for (let i = 0; i < this.size; i++) {
      const has1 = this.bitmaps[0][i]
      const has2 = this.bitmaps[1][i]
      total += has1 + has2
      overlaps += has1 && has2
    }
    return (2 * overlaps) / total
  }
}

function fastDice(str1, str2) {
  if (str1.length < 10000) {
    return diceCoefficient(str1, str2)
  }

  const bloomer = new DiceBloomer(str1.length)
  bloomer.bloom(str1, 0)
  bloomer.bloom(str2, 1)
  return bloomer.score()
}


function diceCoefficient(str1, str2) {
  if (!str1 || !str2) return 0
  const grams1 = trigrams(str1)
  const grams2 = trigrams(str2)
  const intersectSize = grams1.intersection(grams2).size
  return (2 * intersectSize) / (grams1.size + grams2.size)
}

function trigrams(str) {
  const grams = new Set()
  for (let i = 0; i < str.length - 2; i += 1) {
    grams.add(str.substring(i, i + 3))
  }
  return grams
}

exports.fastDice = fastDice
