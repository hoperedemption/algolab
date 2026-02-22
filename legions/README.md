# Legions - Maximin programming in an arrangement of half-planes

### Algorithm tag

**Linear Programming, Half-Planes** 

---

### The problem in a nutshell

Given a start point `s = (x_s, y_s) in R^2` and `n` lines (legions)

`l_i: a_i x + b_i y + c_i = 0`

with speed `v_i > 0`. Let `sigma_i in {+1, -1}` be chosen so that  
`sigma_i*(a_i x_s + b_i y_s + c_i) > 0` (the side of `l_i` containing the start). A point `p = (x, y)` is **reachable** iff it stays in the same cell as `s`, i.e.

`sigma_i*(a_i x + b_i y + c_i) >= 0  for all i.`

If you start at `p`, the earliest arrival time of legion `i` is

`T_i(p) = sigma_i*(a_i x + b_i y + c_i) / ( v_i * sqrt(a_i^2 + b_i^2) ),`

and the preparation time is `min_i T_i(p)`. Compute

`max over reachable (x, y) of  min_{1 <= i <= n} T_i(x, y),`

rounded down to an integer. 

---

### High-level algorithm idea

Introduce a variable `z` representing the guaranteed preparation time. For each legion `i`, require that the time until its soldiers reach `(x, y)` is at least `z`. I.e the variable represents the minimum preparation time. Multiplying out the distance formula gives a **linear** inequality:

`sigma_i(a_i x + b_i y + c_i) >= v_i * sqrt(a_i^2 + b_i^2) * z,`

plus the implicit reachability constraint is already included because `z >= 0`. So the whole problem becomes a single LP feasibility/optimization in 3 variables `(x, y, z)`: maximize `z` subject to `n` linear inequalities and `z >= 0`. The answer is `floor(z*)`. 

---

### Complexity analysis

Let `n` be the number of legions.

* LP has **3 variables** and **n** constraints.
* In fixed dimension, LP can be solved in time polynomial in `n` (often near-linear in practice); CGAL’s exact solver handles the arithmetic robustly.

**Time:** `O(n)` for a 3-variable LP.  
**Space:** `O(n)` to store the constraints.

## Points

```text
Testing solution >>>>
   Test set 1 (25 pts / 3 s) : Correct answer      (0.04s)
   Test set 2 (25 pts / 3 s) : Correct answer      (0.111s)
   Test set 3 (25 pts / 3 s) : Correct answer      (1.543s)
   Test set 4 (25 pts / 3 s) : Correct answer      (1.729s)

Total score: 100