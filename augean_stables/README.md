# The Augean Stables: Binary Search on Feasibility of a Linear Program

## Algorithm tag

**LP + Binary Search on Answer**

---

## Problem in a nutshell

There are `n` constraints (stalls). Decision variables are real numbers

`h_1, h_2, h_3 in [0, 1].`

For each stall `i`, you are given integers `f_i, c_i, k_i, l_i, m_i`. If you spend `h_A in {0, ..., 24}` hours on river A and `h_B in {0, ..., 24}` hours on river B, define cumulative redirected flows

`a = sum_{t=1..h_A} a_t,   p = sum_{t=1..h_B} p_t,`

and modified coefficients

`k_i' = k_i + a^2,   l_i' = l_i + p^2,   m_i' = m_i + a*p.`

The “removed filth” is `h_1*k_i' + h_2*l_i' + h_3*m_i'`. Cleaning stall `i` requires:

`h_1*k_i' + h_2*l_i' + h_3*m_i' >= f_i - c_i   for all i.`

Goal: find the minimum total hours (`H = h_A + h_B`) such that **there exist** integers `h_A, h_B in [0, 24]` with `h_A + h_B = H` and reals `h_1, h_2, h_3 in [0, 1]` satisfying all inequalities; otherwise output “Impossible!”. 

---

## Algorithm logic

We treat the total trench time (`H = h_A + h_B`) as the outer discrete parameter and **binary search** the smallest feasible `H` (range `0` to `48`). For a candidate `H`, try all splits `(h_A, h_B)` with `h_A + h_B = H` (at most 25 options). Each split fixes `(a, p)`, hence fixes all coefficients `(k_i', l_i', m_i')`, and the question becomes pure **LP feasibility** in variables `(h_1, h_2, h_3)` with bounds `0 <= h_j <= 1` and `n` linear inequalities. Solve that feasibility LP using CGAL’s linear programming solver; if any split is feasible, then `H` is feasible and you move left in the binary search, otherwise move right. A small “upper bound prune” (your `ub < f-c`) quickly discards splits that can’t possibly work even with `h_1 = h_2 = h_3 = 1`. This pruning saves on running the LP solver allowing for faster convergence and thus passing all tests.

---

## Complexity

Let `n <= 100`.

* Prefix sums for `a_t, p_t`: `O(24)`.
* Binary search over `H in [0, 48]`: `O(log 49)` iterations (<= 6).
* For each `H`, test up to 25 splits, each building an LP with 3 variables and `n` constraints and solving it.

So total work is:

`O(log 49 * 25 * n)`

## Points

```text
Testing solution >>>>
   Test set 1        (25 pts / 1 s) : Correct answer      (0.226s)
   Test set 2        (25 pts / 1 s) : Correct answer      (0.136s)
   Test set 3        (20 pts / 1 s) : Correct answer      (0.239s)
   Test set 4        (10 pts / 1 s) : Correct answer      (0.353s)
   Hidden test set 1 (05 pts / 1 s) : Correct answer      (0.215s)
   Hidden test set 2 (05 pts / 1 s) : Correct answer      (0.136s)
   Hidden test set 3 (05 pts / 1 s) : Correct answer      (0.265s)
   Hidden test set 4 (05 pts / 1 s) : Correct answer      (0.329s)

Total score: 100