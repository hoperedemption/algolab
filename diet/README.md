# Diet: LP Optimization

## Algorithm tag

**LP**

---

## Problem in a nutshell

There are `n` nutrients and `m` foods. Let `x_j >= 0` be the (possibly fractional) amount of food `j`. Each food has price `p_j`, and contributes `C_{j,i}` units of nutrient `i` per unit. For each nutrient `i`, the daily intake must satisfy:

`min_i <= sum_{j=1..m} C_{j,i} * x_j <= max_i.`

Minimize total cost:

`min  sum_{j=1..m} p_j * x_j.`

If infeasible, output **"No such diet."**; otherwise output `floor(opt)`. 

---

## Algorithm idea

This is a direct linear program. Each nutrient constraint is split into two linear inequalities:

`sum_j C_{j,i} * x_j <= max_i,`

`-sum_j C_{j,i} * x_j <= -min_i.`

All variables `x_j` have lower bound `0`. The objective coefficients are exactly the food prices `p_j`. Solving the LP yields either infeasibility (no diet meets all bounds) or an optimal fractional diet; only the optimal value is needed, rounded down.

---

## Complexity

Let `m` be the number of variables and `2n` the number of constraints.

* Building the LP: `O(n*m)` coefficients.
* Solving: polynomial-time in `(n, m)`: `O(max(n, m))` as seen in the course. 
* Memory: `O(n*m)` to store the constraint matrix (plus solver overhead).

## Points

```text
Testing solution >>>>
   Test set 1 (35 pts / 0.200 s) : Correct answer      (0.001s)
   Test set 2 (35 pts / 0.200 s) : Correct answer      (0.006s)
   Test set 3 (30 pts / 0.200 s) : Correct answer      (0.073s)

Total score: 100