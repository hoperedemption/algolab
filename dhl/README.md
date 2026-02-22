# DHL - 2D Dynamic Programming

## Algorithm tag

**DP**

---

## Problem in a nutshell (math, no story)

Given two sequences `a_1, ..., a_n` and `b_1, ..., b_n`. We must perform a sequence of rounds; in each round we remove **at least one** remaining element from the end of stack `a` and **at least one** remaining element from the end of stack `b`. Let a round remove a suffix block from each stack; if in some round we remove a block from `a` whose values sum to `S_A` and size `k_A`, and from `b` whose values sum to `S_B` and size `k_B`, then the round cost is

`(S_A - k_A) * (S_B - k_B).`

All elements must be removed, and both sequences must become empty in the same final round. Minimize the total cost. 

---

## Algorithm logic (high level)

The DP views the process as walking on an `n x n` grid: state `(i, j)` means “we have decided the last round includes `a_i` and `b_j` as the *current* top parcels under consideration.” The key observation that we can exploit is that the incremental cost contribution can be expressed locally as a product of per-item terms `(a_i - 1) * (b_j - 1)`, so the optimal schedule corresponds to choosing a monotone path from `(0, 0)` to `(n-1, n-1)` minimizing the sum of those local costs. From each `(i, j)`, you can advance by consuming one more item from stack A, or from stack B, or from both in the same “round boundary,” which gives exactly the three transitions:

`dp[i][j] = (a_i - 1) * (b_j - 1) + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]).`

The final answer is `dp[n-1][n-1]`.

---

## Complexity

* DP table has `n^2` states, each computed in `O(1)`.
* **Time:** `O(n^2)`
* **Memory:** `O(n^2)` (can be optimized to `O(n)` with rolling rows if desired).

## Points

```text
Testing solution >>>>
   Test set 1 (20 pts / 1 s) : Correct answer      (0.0s)
   Test set 2 (40 pts / 1 s) : Correct answer      (0.0s)
   Test set 3 (39 pts / 1 s) : Correct answer      (0.004s)
   Test set 4 (01 pts / 1 s) : Correct answer      (0.041s)

Total score: 100