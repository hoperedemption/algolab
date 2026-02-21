# Harry Potter - Feasibility of a lossy flow under total-cost budget 

### Algorithm tag

**Linear Programming, Binary Search, Bottleneck Minimization, Minimax** 

---

### The problem in a nutshell

Let `V = {1, ..., n}` be nodes with `1` (sink) and `2` (source). You are given directed edges `E` where each edge `(i, j)` has:

* suspicion coefficient `d_ij >= 0`,
* efficiency `e_ij in (0, 1]` (so sending `x` units results in `e_ij * x` units arriving).

Let decision variables `x_ij >= 0` denote amount sent on edge `(i, j)`. Define node balances with multiplicative losses as:

`sum_{(u, v) in E} (e_uv * x_uv) - sum_{(v, w) in E} x_vw`

(i.e., inflow-after-loss minus outflow).

Given `a, b, c`, ask whether there exists a feasible circulation-with-supply such that:

* total suspicion budget: `sum_{(i, j) in E} (d_ij * x_ij) <= c`,
* source can inject at most `a`: `sum_{(2, j) in E} x_2j <= a`,
* sink (node 1) receives at least `b`:
  `sum_{(i, 1) in E} (e_i1 * x_i1) - sum_{(1, j) in E} x_1j >= b`,
* all other nodes (`v >= 3`) conserve nonnegatively:
  `sum_{(i, v)} (e_iv * x_iv) - sum_{(v, j)} x_vj >= 0`.

Additionally, nodes are ordered by trust; define `k` as the smallest integer `>= 2` such that restricting to the induced subgraph on `{1, ..., k}` still makes the system feasible. For that minimum `k`, define `s` as the minimum possible **maximum per-edge suspicion**:

`s := min  max_{(i, j) in E} (d_ij * x_ij)`

over all feasible solutions using only nodes `<= k`. Output “Busted!” if infeasible even for `k = n`; else output `(k, s)` with `s` as an integer ceiling. 

---

### High-level algorithm idea

Model feasibility for a fixed prefix `{1, ..., K}` as a linear program in the `x_ij`’s: the constraints are exactly the suspicion budget, the source injection limit, the sink demand, and the (lossy) flow conservation inequalities at intermediate nodes as defined above. Then find the smallest feasible `k` by **binary searching** on `K` and, for each mid, disabling edges that touch nodes `> K` by forcing `x_ij = 0` (upper bound `0`). Once the minimum `k` is known, compute the minimum bottleneck `s` by introducing an extra variable `S` and constraints `d_ij * x_ij <= S` for every edge, and minimize `S`. Finally output `ceil(S)`. 

---

### Complexity analysis

Let `m = |E|`, `n <= 32`, `m <= 64`.

* **Binary search** over `k`: `O(log n)` LP solves.
* Each LP has `m` variables (plus one extra `S` in the second phase) and `O(n + m)` constraints.

So total time is `O(log n * (n + m))`
**Space:** `O(n + m)` to store constraints and edge lists.