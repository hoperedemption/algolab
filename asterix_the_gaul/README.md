# Astérix the Gaul: Meet-in-the-Middle Subset Sum + Binary Search on Answer

## Algorithm tag

**MITM + BinarySearch**

---

## Problem in a nutshell

There are `n <= 30` moves. Move `i` has distance `d_i` and time `t_i`. A subset `S ⊆ {1, ..., n}` is feasible if

`sum_{i in S} t_i < T.`

After drinking `g` gulps, each chosen move gains an additional `s_g` distance (with `s_0 = 0`), so the achieved distance is

`sum_{i in S} d_i + |S| * s_g.`

Goal: find the minimum `g in {0, ..., m}` such that there exists a subset `S` with `sum_{i in S} t_i < T` and

`sum_{i in S} d_i + |S| * s_g >= D.`

If no such `g` exists, output **“Panoramix captured”**. 

---

## Algorithm idea

Feasibility is monotone in `g`: larger `g` never reduces any achievable distance. Therefore, binary search can be used on `g`. For a fixed `g`, checking feasibility is still exponential in `n`, but `n <= 30` is small enough for **meet-in-the-middle**:

* Split the moves into two halves `L` and `R` of sizes `<= 15`.
* Enumerate all subsets of each half and keep pairs `(time, dist)`, where `dist` already includes the potion bonus `|S| * s_g`.
* For the right half, sort by time and preprocess a prefix structure so that for any time budget `x`, the maximum distance among subsets with time `<= x` can be obtained quickly. That is we want to make sure that we always get more distance if we use up more time.
* For each left subset `(t_L, d_L)`, look up the best compatible right subset within remaining time `T - t_L` and check whether `d_L + d_R >= D`.

---

## Complexity

Let `n_L = floor(n/2)`, `n_R = n - n_L` (both `<= 15`).

For a single feasibility check at fixed `g`:

* enumerate subsets: `O(2^{n_L} + 2^{n_R})`,
* sort right list: `O(2^{n_R} * log(2^{n_R}))`,
* scan left list with binary searches: `O(2^{n_L} * log(2^{n_R}))`.

Binary search over `g in [0, m]`: `O(log(m + 1))` iterations.

**Total per test case:**

`O(log(m + 1) * (2^{n/2} * log(2^{n/2})))`

which is easily fast for `n <= 30`.  
Memory: `O(2^{n/2})` for storing the subset lists.

## Points

```text
Testing solution >>>>
   Test set 1 (20 pts / 5 s) : Correct answer      (0.003s)
   Test set 2 (20 pts / 5 s) : Correct answer      (0.015s)
   Test set 3 (20 pts / 5 s) : Correct answer      (0.461s)
   Test set 4 (40 pts / 5 s) : Correct answer      (2.712s)

Total score: 100
