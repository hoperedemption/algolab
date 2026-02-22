# Alice and the Hurried Rabbit Clan: 2D Prefix Sums + Dynamic Programming on a Grid

## Algorithm tag

**DP + PrefixSum**

---

## Problem in a nutshell

Given an `n x m` grid with nonnegative weights `r_{i,j}`. Consider monotone paths

`P: (0,0) -> (n-1,m-1)`

that move only **south** `(i+1, j)` or **east** `(i, j+1)`.

For a cell `u = (i, j)`, define the directed (monotone) distance to the path

`d(u, P) = min over v in P with v >= u of ( (v_i - i) + (v_j - j) ),`

where `v >= u` means `v_i >= i` and `v_j >= j` (reachable by only moving south/east).

Objective:

`min over P of  sum_{i=0..n-1} sum_{j=0..m-1} r_{i,j} * d((i,j), P).`

Output the minimum value. 

---

## Algorithm logic

The key trick is to compute the objective **incrementally** while constructing `P` from bottom-right back to top-left. When you decide that the path “steps” through a boundary between `(i,j)` and its next cell, you determine which grid cells will have their nearest contact with the path **shift by +1** in distance (because the earliest path cell they can reach moves one step further). Those “pay +1” regions are rectangles:

* If you go **down** from `(i,j)` to `(i+1,j)`, then all cells in rows `0..i` and columns `j+1..m-1` become one step farther ⇒ add `sum r` over that rectangle.
* If you go **right** from `(i,j)` to `(i,j+1)`, then all cells in rows `i+1..n-1` and columns `0..j` become one step farther ⇒ add `sum r` over that rectangle.

To make these rectangle sums `O(1)`, we must precompute a **2D prefix-sum** array `pref`, and use it to query any sub-rectangle in constant time.

Then run a DP from `(n-1,m-1)` backwards:

* `dp[i][j]` = minimum total cost for completing a monotone path from `(i,j)` to `(n-1,m-1)`,
* transition to either `(i+1,j)` or `(i,j+1)` adding the appropriate rectangle sum cost.

So the whole problem becomes a standard grid DP with fast rectangle-sum transitions.

---

## Complexity

* Build 2D prefix sums: `O(nm)`
* DP over all states with `O(1)` transitions: `O(nm)`
* Memory: `O(nm)` for `pref` and `dp`.

**Total:** `O(nm)` time, `O(nm)` memory.

## Points

```text
Testing solution >>>>
   Test set 1        (20 pts / 4 s) : Correct answer      (0.001s)
   Test set 2        (20 pts / 4 s) : Correct answer      (0.001s)
   Test set 3        (20 pts / 4 s) : Correct answer      (0.029s)
   Test set 4        (20 pts / 4 s) : Correct answer      (2.015s)
   Hidden test set 1 (05 pts / 4 s) : Correct answer      (0.001s)
   Hidden test set 2 (05 pts / 4 s) : Correct answer      (0.001s)
   Hidden test set 3 (05 pts / 4 s) : Correct answer      (0.029s)
   Hidden test set 4 (05 pts / 4 s) : Correct answer      (2.015s)

Total score: 100