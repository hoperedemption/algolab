# Inball: Largest Inscribed Ball in a Polyhedron

## Algorithm tag

**Linear Programming + Maximin**

---

## Problem in a nutshell (math, no story)

Given `n` halfspaces in `R^d`,

`C = { x in R^d | a_i^T x <= b_i,  i = 1..n },`

find the maximum radius `r >= 0` such that there exists a center `x in R^d` with the closed Euclidean ball `B(x, r) ⊆ C`. Output the maximum **integral** radius `floor(r*)`. If `C = empty` output **none**; if `r* = +infty` output **inf**. 

---

## Algorithm idea (high level)

The largest inscribed ball can be expressed as a linear program by enforcing that the center stays at distance at least `r` from every bounding hyperplane. For each inequality `a_i^T x <= b_i`, the condition “ball of radius `r` fits” is

`a_i^T x + ||a_i||_2 * r <= b_i,`

because moving `x` by distance `r` in the direction of `a_i` must still respect the halfspace. This yields an LP in variables `(x_1, ..., x_d, r)` with constraint `r >= 0`, maximizing `r`. Solving the LP gives:

* infeasible (=> cave empty => **none**),
* unbounded (=> arbitrarily large ball fits => **inf**),
* otherwise optimal `r*` (=> print `floor(r*)`).

---

## Complexity

Let `D = d + 1` be the number of variables and `n` the number of constraints.

* Building constraints: `O(n d)`.
* Solving an LP in fixed small dimension (`d <= 10`) is `O(n)`
* Memory: `O(n d)` for storing the constraint matrix (plus solver overhead).

Overall: **polynomial in `n`** with **constant dimension** (`d <= 10`).

## Points

```text
Testing solution >>>>
   Test set 1 (35 pts / 2 s) : Correct answer      (0.011s)
   Test set 2 (35 pts / 2 s) : Correct answer      (0.105s)
   Test set 3 (30 pts / 2 s) : Correct answer      (0.903s)

Total score: 100