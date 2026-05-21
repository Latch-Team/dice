# https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Dice%27s_coefficient


# DO NOT MODIFY THIS FUNCTION! This is a baseline. Modify dice_fast.py.
def trigrams(s: str) -> set[str]:
    grams: set[str] = set()
    for i in range(len(s) - 2):
        grams.add(s[i : i + 3])
    return grams


# DO NOT MODIFY THIS FUNCTION! This is a baseline. Modify dice_fast.py.
def simple_dice(str1: str, str2: str) -> float:
    if not str1 or not str2:
        return 0.0

    grams1 = trigrams(str1)
    grams2 = trigrams(str2)
    intersect_size = len(grams1 & grams2)
    return (2 * intersect_size) / (len(grams1) + len(grams2))
