# Boats: Greedy Interval Scheduling with Two-Sided Frontier

## Algorithm tag

**Greedy + Interval Scheduling**

---

## Problem in a nutshell

There are `n` boats. Boat `i` has length `ell_i` and a fixed ring position `p_i in R` on the river bank (all `p_i` distinct). Choosing boat `i` means placing an interval `[L_i, R_i]` of length `ell_i` such that

`R_i - L_i = ell_i,   p_i in [L_i, R_i].`

The objective is to select a maximum-size subset of boats and choose their placements so that the resulting intervals are pairwise non-overlapping (touching at endpoints allowed). Output the maximum number of boats that can be placed simultaneously. 

---

## Algorithm idea

Sort boats by ring position `p` from left to right. While scanning, maintain the best achievable “rightmost occupied coordinate” after placing a certain number of boats. The key is that for each count it is enough to remember only two candidate endpoints: one for the last accepted boat, and one alternative that is potentially better (smaller right endpoint) if the current boat can “replace” a previous choice. When the current boat conflicts with the current plan, decide whether it is better to discard the current boat or to replace the last chosen boat by the current one, based on which option yields a smaller right endpoint. A smaller right endpoint is always better because it leaves more room for later boats.

---

## How the greedy state works

After sorting by `p`, process boats `(ell, p)` in that order. Maintain two values:

* `X_2`: the right endpoint of the interval configuration for the current chosen count (the “active plan”).
* `X_1`: the right endpoint of the configuration with one less chosen boat (used as a fallback for replacement).

Interpretation: after having committed to `count` boats so far, `X_2` is the smallest possible rightmost endpoint among valid placements that realize `count` boats. Similarly `X_1` is the corresponding value for `count - 1`.

For a boat `(ell, p)`, the best possible right endpoint if it is appended after a configuration ending at `X` is:

`end(X; ell, p) = max(X + ell, p),`

because the boat must start at `<= p` and end at `>= p`, and also cannot start before `X` (non-overlap), so the earliest start is `X` and thus the earliest feasible end is `max(X + ell, p)`.

Two cases:

1. **No conflict with the active plan** (`p >= X_2`):  
   The boat can be appended to the `count`-boat configuration. Commit it:

   `X_1 <- X_2,   X_2 <- max(X_2 + ell, p),   count++.`

2. **Conflict** (`p < X_2`):  
   The boat’s ring lies before the current right endpoint, so it cannot simply be appended. The only helpful action is to see if replacing the last chosen boat by this one yields a better (smaller) right endpoint for the same `count`. Replacing means attaching this boat after the best `(count-1)` configuration:

   `X_2 <- min( X_2,  max(X_1 + ell, p) ).`

   If the replacement end is smaller, future boats become easier to place; otherwise ignore this boat.

This greedy is correct because the scan order by `p` ensures that only the rightmost endpoint matters as a summary of the past: among all feasible placements of the same number of boats using only processed rings, the one with the smallest current right endpoint dominates all others.

---

## Complexity

* Sorting boats by ring position: `O(n log n)`.
* Single scan with `O(1)` work per boat: `O(n)`.
* Extra memory: `O(1)` beyond the input storage.

**Total:** `O(n log n)` time, `O(1)` auxiliary space.

## Points

```text
Testing solution >>>>
   Test set 1 (30 pts / 2 s) : Correct answer      (0.002s)
   Test set 2 (30 pts / 2 s) : Correct answer      (0.005s)
   Test set 3 (40 pts / 2 s) : Correct answer      (0.673s)

Total score: 100