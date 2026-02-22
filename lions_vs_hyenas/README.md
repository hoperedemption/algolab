# Lions vs Hyenas: Global Minimum Cut With Assignment Preferences

## Algorithm tag

**Min-Cut + Max Flow**

---

## Problem in a nutshell

Given `n` vertices `V = {0, ..., n-1}` to be partitioned into two nonempty sets `A` and `B`, with two distinguished vertices `k in A`, `m in B` (or the opposite assignment), and:

* a signed preference value `a_i in Z` for each vertex `i`,
* a symmetric nonnegative “same-side bonus” `f_ij = f_ji >= 0` for each unordered pair `{i, j}`,

define the **happiness** of a partition `(A, B)` as

`H(A, B) = sum_{i in A} a_i - sum_{i in B} a_i + sum_{ {i,j} : i,j on same side } f_ij - sum_{ {i,j} : i,j on different sides } f_ij.`

Equivalently, up to an additive constant `U`, maximizing happiness is the same as minimizing a cut expression of the form

`min over A containing s and B containing t of  sum_{(u,v) in E with u in A and v in B} c(u,v),`

i.e., an **s–t minimum cut** (directed, capacities `c >= 0`), with `s = k`, `t = m` (or swapped), and then (if asked) converting the min-cut value back to the requested objective (max happiness or min dissatisfaction). 

---

## Algorithm logic

Reduce the team assignment objective to an **s–t min-cut** problem.

* Friendship terms `f_ij` are encoded as directed capacities between `i` and `j` so that separating `i` and `j` forces a cut penalty proportional to `f_ij`.
* Preference terms `a_i` are encoded by wiring each vertex to one of the two forced terminals (either `k` or `m`), so that putting a vertex on the “wrong” side relative to its sign incurs a cut penalty `|a_i|`. We must try the two consistent cases:

  1. `k in A, m in B`
  2. `k in B, m in A`
     and take the better outcome.

Then compute the min-cut value as a max-flow (by **Push–Relabel**), and:

* if the task is **min dissatisfaction** (`b = 1`), the min-cut value itself is the answer;
* if the task is **max happiness** (`b = 0`), you precompute the “utopia” constant `U` (happiness if every term were satisfied) and use

  `H_max = global satisfaction - global dissatisfcation = (U - mincut) - mincut = U - 2 * mincut,`

---

## Complexity

Let `n <= 100`. The graph we build has:

* `V = n` vertices,
* `E = O(n^2)` directed edges from friendships plus `O(n)` from preferences.

Run max-flow twice (two terminal assignments). With Push–Relabel, a safe bound is:

`O(2 * V^3) = O(n^3)`
