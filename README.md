# Optimize Dice's Coefficient

Dice's Coefficient is a simple statistic that measures the similarities of two strings. A Dice's Coefficient of 1 means the strings are exactly the same, and 0 means they have no similarities at all.

Computing Dice's Coefficient is really easy. It goes like this:

1. For `string1`, turn it into a set of trigrams `trigrams1`
2. For `string2`, turn it into a set of trigrams `trigrams2`
3. Calculate the size of the intersection of those two sets (number of triagrams)
4. Multiply the intersection size by two and divide it by the sum of the sizes of `trigrams1` and `trigrams2` (in triagrams)

Example:

For `string1 = "hello"` and `string2 = "help"`:

1. `trigrams1 = {"hel", "ell", "llo"}` (size 3)
2. `trigrams2 = {"hel", "elp"}` (size 2)
3. Intersection: `{"hel"}` (size 1)
4. Dice's Coefficient: `(2 * 1) / (3 + 2) = 0.4`

`dice.js` contains a gold-standard baseline implementation of Dice's Coefficient. The only problem is that it's slow.
`dice_fast.js` contains a hacky attempt to speed it up by approximating the answer. It succeeds in speeding up the result, but fails by returning an incorrect answer - the result is 14.1% off from the baseline on one of the test cases.

Your job is to fix `dice_fast.js` - make the function _as fast as possible,_ while returning a result within _5% of the baseline._

Run `node bench.js` to benchmark your solution. Get the `% of baseline` number as low as possible while not failing the tests.

Feel free to google things and/or use a profiler, but don't use any AI.

It shouldn't be too hard to get to ~50% of baseline with 0% degradation.

This is what Ivo's Head of Engineering got:

| index | length | Baseline | Baseline ms | fastDice | fastDice ms |
| ----- | ------ | -------- | ----------- | -------- | ----------- |
| 0 | 5 | 0.40 | 0.01 | ✅ | 0.00 (66.3%) 🚀🚀🚀 |
| 1 | 61183 | 0.98 | 4.54 | ✅ | 0.82 (18.0%) 🚀🚀🚀🚀🚀🚀🚀🚀 |
| 2 | 360606 | 0.99 | 15.04 | ✅ | 2.22 (14.7%) 🚀🚀🚀🚀🚀🚀🚀🚀 |
| 3 | 145169 | 0.23 | 19.32 | ✅ (0.24: 3.0% off) | 2.60 (13.4%) 🚀🚀🚀🚀🚀🚀🚀🚀 |

```
Total elapsed (baseline): 38.90ms
Total elapsed (fastDice): 20.37ms
Runtime (fastDice): 14.5% of baseline 🚀🚀🚀🚀🚀🚀🚀🚀
✅ All test cases passed
```
