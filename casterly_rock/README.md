# Casterly Rock: Linear Programming Feasibility + Minimax Optimization for a Separating Line under a Budget Constraint

## Algorithm tag

**LP + Geometry in the Plane**

---

## Problem in a nutshell

You are given two point sets in `R^2`: nobles `N = {(x_i, y_i)}_{i=1..n}` and commons `C = {(x'_j, y'_j)}_{j=1..m}`, and a threshold `s in {-1, 0, 1, ...}` (where `-1` means `+infty`). Find two lines:

* a **sewer line** `L_s: x = B y + C` with `B, C in R` such that all nobles are on the **left** and all commons on the **right** (points on the line allowed), i.e.

  `for all (x,y) in N:  x - B*y - C <= 0,`  
  `for all (x,y) in C:  x - B*y - C >= 0.`

* a **water line** `L_w` that is perpendicular to `L_s` (so its slope is `-1/B`, with `B != 0`).

Additionally, the total horizontal distance from each point to the sewer line (sum of sewage pipe lengths) must satisfy

`sum_{(x,y) in N} (B*y + C - x) + sum_{(x,y) in C} (x - B*y - C) <= s   (if s != -1).`

Among all feasible choices, minimize the maximum vertical distance from any point to the water line (i.e., minimize the longest fresh-water pipe length):

`min d  s.t.  for all points (x,y):  | y - (D*x + E) | <= d,`

with `(D, E)` representing the water line (and perpendicularity enforced implicitly by the way the LP is set up). Output:

* **“Y”** if no separating sewer line exists,
* **“B”** if separation exists but the budget constraint fails,
* otherwise `ceil(d*)`. 

---

## Algorithm logic

The solution is a sequence of LPs:

1. **Check separability (“Yuck!” test):** set up a feasibility LP in variables `(B, C)` that enforces “nobles left, commons right” of the sewer line `x = B y + C`. If infeasible → print **Y**.

2. **Check the sewage budget (“Bankrupt” test):** if `s != -1`, add one more linear inequality that exactly encodes the sum of horizontal distances (pipe lengths) to the sewer line being `<= s`. If the augmented LP becomes infeasible → print **B**. (If `s = 0`, short-circuit to **B** if separable, matching the special subtask behavior.)

3. **Optimize longest water pipe:** introduce variables representing the water line and a nonnegative variable `d`. Add constraints that bound every point’s vertical distance to the water line by `d` (done by two inequalities per point). Set the LP objective to minimize `d`. Solve and output `ceil(d)`.

So the whole task is: feasibility → feasibility-with-budget → minimization LP.

---

## Complexity

Let `N = n + m` points, with `n, m <= 500`.

* LP1: `N` constraints, 2 variables `(B, C)`.
* LP2: adds 1 constraint.
* LP3: adds `2N` constraints and a few extra variables (still constant count).

Overall each test case solves a constant number of small-dimensional LPs (dimension `O(1)`, constraints `O(N)`)

`time = O(N) with constant dimension,   memory = O(N).`

## Points

```text
Testing solution >>>>
   Test set 1 (30 pts / 2 s) : Correct answer      (0.052s)
   Test set 2 (30 pts / 2 s) : Correct answer      (0.721s)
   Test set 3 (20 pts / 2 s) : Correct answer      (0.208s)
   Test set 4 (20 pts / 2 s) : Correct answer      (0.556s)

Total score: 100
