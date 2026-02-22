# Placing Knights: Maximum Independent Set in a Bipartite Graph via Konig’s Theorem (Max-Flow / Min-Cut)

## Algorithm tag

**Max-Flow Min-Cut, Konig’s Theorem , Maximum Independent Set**

---

## Problem in a nutshell

Let `B` be an `n x n` grid with some cells removed (holes). Define an undirected graph

`G = (V, E)`

where `V` is the set of present cells, and `{u, v} in E` iff a knight move connects the two cells (i.e. `(±1, ±2)` or `(±2, ±1)`). Find

`alpha(G) = max { |I| : I ⊆ V, I is independent },`

the maximum number of vertices with no edge between any pair (maximum number of non-attacking knights). 

Because knight moves always change square color, `G` is **bipartite** under the chessboard coloring.

---

## Algorithm logic

Just convert the bipartite “attack graph” into a flow network:

* Partition squares into **black** and **white** by parity `(i + j) mod 2`.
* Add edges from **source → black** (capacity 1), **black → white** for every possible knight attack (capacity 1), and **white → sink** (capacity 1).
* The **maximum matching** size in this bipartite graph equals the **minimum vertex cover** size (Kőnig’s theorem).
* The maximum independent set size is then

  `alpha(G) = |V| - tau(G) = |V| - (min vertex cover) = |V| - (max matching).`

---

## Complexity

Let `V = # present cells (<= n^2)`, and `E <= 8V` (each cell has at most 8 knight moves).

* Build graph: `O(V + E) = O(V)`.
* Max-flow on this bipartite unit-capacity network (push–relabel worst-case bound): `O(V^3)` in general

**Total:** `O(V^3)` worst case, memory `O(V + E) = O(V)`.

## Points

```text
Testing solution >>>>
   Test set 1 (30 pts / 0.500 s) : Correct answer      (0.0s)
   Test set 2 (35 pts / 0.500 s) : Correct answer      (0.048s)
   Test set 3 (35 pts / 0.500 s) : Correct answer      (0.157s)

Total score: 100