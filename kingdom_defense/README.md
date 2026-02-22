# Kindom Defense - Circulation with Lower Bounds: Feasibility via s–t Max-Flow (Push–Relabel)

## Algorithm tag

**Max-Flow + Circulation Problem**

---

## Problem in a nutshell

Given a directed graph `G = (V, E)` with, for each edge `e in E`, a **lower bound** `l_e >= 0` and an **upper bound** `u_e >= l_e`. Each node has a supply and a demand associated to it. Determine whether there exists a feasible flow `f` satisfying

`l_e <= f_e <= u_e   for all e in E`

and flow conservation at every vertex `v`:

`sum_{(v,w) in E} f_vw - sum_{(u,v) in E} f_uv = 0.`

Equivalently: decide feasibility of a circulation with lower/upper bounds. Output “yes” iff feasible, else “no”.

---

## Algorithm idea

Lower bounds are removed by shifting each edge flow:

`f'_e := f_e - l_e,   so   0 <= f'_e <= u_e - l_e.`

This shift creates **imbalances** at vertices: each lower bound `l_uv` contributes `(+l_uv)` demand to `v` (inflow requirement) and `(-l_uv)` supply at `u` (outflow requirement). Let

`b(v) = sum_{(u,v)} l_uv - sum_{(v,w)} l_vw.`

A feasible circulation exists iff the remaining capacities `(u - ell)` can route flow so that every vertex with `b(v) > 0` receives `b(v)` units and every vertex with `b(v) < 0` sends `-b(v)` units. The above imbalances are solved by interpeting excess flow at a node as getting flow from the source and lack of flow at a node as demanding flow as a sink.

This is reduced to one max-flow instance by adding a super source `S` and super sink `T`:

* if `b(v) > 0`, add edge `S -> v` with capacity `b(v)`,
* if `b(v) < 0`, add edge `v -> T` with capacity `-b(v)`,
* keep original edges with capacity `u_e - l_e`.

Then run max-flow from `S` to `T`. Feasible iff all edges out of `S` are saturated, i.e. max-flow equals `sum_{b(v) > 0} b(v)`.

In my implementation, each “special constraint edge” `(f -> t)` has `l = c`, `u = C`. It is encoded as:

* capacity `C - c` on `f -> t`,
* plus the balance adjustments via edges `f -> T` (capacity `c`) and `S -> t` (capacity `c`).
  Node constraints `(g_i, d_i)` are handled as `S -> i` of capacity `g_i` and `i -> T` of capacity `d_i`, and the total required amount `sum d_i + sum c_j` is tracked as `demands`. Finally, feasibility is exactly `flow == demands`.

---

## Complexity

Let `V = l + 2` vertices (original `l` plus super source/sink) and `E = O(l + p)` edges after transformation.

Using push–relabel max-flow:

* Worst-case: `O(V^3)`,
* Memory: `O(V + E)`.

So overall per test case:

`O((l + 2)^3)` time (worst-case), and `O(l + p)` memory.

## Points

```text
Testing solution >>>>
   Test set 1        (15 pts / 5 s) : Correct answer      (2.212s)
   Hidden test set 1 (05 pts / 5 s) : Correct answer      (2.386s)
   Test set 2        (15 pts / 5 s) : Correct answer      (0.77s)
   Hidden test set 2 (05 pts / 5 s) : Correct answer      (0.819s)
   Test set 3        (25 pts / 5 s) : Correct answer      (2.511s)
   Hidden test set 3 (05 pts / 5 s) : Correct answer      (2.197s)
   Test set 4        (25 pts / 5 s) : Correct answer      (2.197s)
   Hidden test set 4 (05 pts / 5 s) : Correct answer      (2.905s)

Total score: 100