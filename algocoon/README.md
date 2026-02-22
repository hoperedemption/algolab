# Algocoön Group: Global Minimum s–t Cut 

## Algorithm tag

**Max-Flow Min-Cut**

---

## Problem in a nutshell

Given a directed graph `G = (V, E)` with nonnegative edge weights/capacities `c : E -> R_{>= 0}`. Find a **nontrivial** partition `V = V_1 ∪ V_2` with `V_1 != empty`, `V_2 != empty` minimizing the cut capacity

`C(V_1, V_2) = sum over edges (u,v) in E with u in V_1 and v in V_2 of c(u,v).`

Output `min over (V_1, V_2) of C(V_1, V_2)`. 

---

## Algorithm logic 

The key fact is **Max-Flow Min-Cut**: for any choice of source `s` and sink `t`, the minimum `s-t` cut value equals the maximum flow value from `s` to `t`. My solution searches for the *global* minimum cut by fixing one reference vertex `0` (could be in either S or T) and computing:

* `min over t != 0 of max-flow(0, t)` via max-flow from `0` to each `t`,
* `min over s != 0 of max-flow(s, 0)` via max-flow from each `s` to `0`,

and taking the minimum over all these `n-1 + n-1` max-flow computations. This works because some global minimum cut separates vertex `0` from at least one vertex on the other side—so it shows up as an `s-t` min-cut for either `s = 0` or `t = 0` (depending on which side contains 0). Each max-flow is computed with **Push–Relabel**, and the best (smallest) cut value seen is the answer.

---

## Complexity

Let `n = |V|`, `m = |E|`.

* We run up to `2(n-1)` max-flow computations.
* Push–relabel worst-case time is `O(n^3)` on general graphs.

**Total worst-case:**

`O(n * n^3) = O(n^4)`
 
**Memory:** `O(n + m)` for the flow network.

## Points

```text
Testing solution >>>>
   Test set 1 (20 pts / 3 s) : Correct answer      (1.214s)
   Test set 2 (20 pts / 3 s) : Correct answer      (1.058s)
   Test set 3 (20 pts / 3 s) : Correct answer      (0.07s)
   Test set 4 (20 pts / 3 s) : Correct answer      (1.169s)
   Test set 5 (20 pts / 3 s) : Correct answer      (0.92s)

Total score: 100