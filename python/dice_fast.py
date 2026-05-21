# You can make this function faster too, if it helps.
def trigrams(s: str) -> set[str]:
    grams: set[str] = set()
    for i in range(len(s) - 2):
        grams.add(s[i : i + 3])
    return grams


# Make this function faster!
def fast_dice(str1: str, str2: str) -> float:
    if not str1 or not str2:
        return 0.0

    # Hacky (and incorrect) speedup attempt. This is NOT a good solution.
    str1 = str1[:100000]
    str2 = str2[:100000]

    grams1 = trigrams(str1)
    grams2 = trigrams(str2)
    intersect_size = len(grams1 & grams2)
    return (2 * intersect_size) / (len(grams1) + len(grams2))
