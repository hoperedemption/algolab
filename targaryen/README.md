# King’s Landing: Multi-source Dijkstra + Edmonds Maximum Matching with matching capacities

## Algorithm tag

**Dijkstra + Edmonds Maximum Matching + Vertex Split**

---

## Problem in a nutshell

Given an undirected weighted graph `G = (V, E)` with `|V| = n`, `|E| = m`, a set of sources `A ⊆ V` (barracks), a distance budget `d`, and a subset `Q ⊆ V` (plazas). A vertex `v` is **reachable** iff

`dist(A, v) = min_{a in A} dist_G(a, v) <= d.`

We may **select** a subset of edges `S ⊆ E` such that:

* **Reachability constraint:** every selected edge `{u, v} in S` has `u, v` reachable.
* **Endpoint capacity constraint:** for each vertex `v`,

  `deg_S(v) <= 1` if `v not in Q` (regular)  
  `deg_S(v) <= 2` if `v in Q` (plaza)

* Additional guarantee: no edge has both endpoints in `Q`. 

**Goal:** maximize `|S|` (maximum number of “safe roads”).

This is a maximum subgraph selection under per-vertex degree bounds `<= 1` (regular) or `<= 2` (plaza) on the reachable-induced subgraph.

---

## Algorithm logic

1. **Compute which intersections are usable**: run **multi-source Dijkstra** from all barracks (by adding a super-source connected with 0-weight edges) to get `dist(A, v)`. Keep only vertices with distance `<= d`.
2. **Turn degree constraints into a matching problem**: build an auxiliary *unweighted* graph `F` where selecting a road corresponds to selecting an edge in a matching.

   * Regular intersection capacity 1 is the usual matching constraint.
   * Plaza capacity 2 is handled by “splitting” each plaza `v` into two capacity-1 slots via a duplicate node `v'` (your `n+v`) and connecting edges so that at most two incident roads can be chosen overall.
   * The promise “no road has both ends plazas” makes this slot-splitting clean (you never need 2-by-2 interactions on the same edge). 
3. **Run Edmonds’ maximum cardinality matching** on `F`. The resulting matching size equals the maximum number of roads you can make safe.


---

## Complexity

* Multi-source Dijkstra on `G`: `O(m log n)` time, `O(n + m)` memory.
* Build auxiliary graph `F`: `O(n + m)` vertices/edges (specifically `≈ 2N` vertices and `O(M)` edges after filtering).
* Edmonds maximum cardinality matching (general graph): `O(n*m*2*2) =  O(n*m)` in worst case.

**Total (worst-case):** `O(m log n + n*m)` time, `O(n + m)` memory (plus matching structures).

## Points

```text
Testing solution >>>>
   Test set 1 (25 pts / 5 s) : Correct answer      (0.132s)
   Test set 2 (25 pts / 5 s) : Correct answer      (0.044s)
   Test set 3 (25 pts / 5 s) : Correct answer      (0.136s)
   Test set 4 (25 pts / 5 s) : Correct answer      (0.148s)

Total score: 100