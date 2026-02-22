# Tiles - Domino Tiling as Bipartite Perfect Matching via Max-Flow

## Algorithm tag

**Max-Flow + Bipartite Matching**

---

## Problem in a nutshell

Given a grid graph `G = (V, E)` induced by all free cells (marked ‘.’) of a `W x H` board, with edges between horizontally/vertically adjacent free cells. Decide whether `V` can be partitioned into `|V|/2` disjoint adjacent pairs, i.e. whether `G` has a **perfect matching**:

`exists M ⊆ E such that every v in V is incident to exactly one edge in M.`

Output “yes” iff such a matching exists, else “no”. 

---

## Algorithm idea 

The grid graph is bipartite under chessboard coloring: `V = V_black ∪ V_white` where `(i + j) mod 2` determines the side, and every adjacency goes across the partition. A domino tiling is exactly a perfect matching in this bipartite graph. This is checked by building an `s-t` flow network:

* add `S -> b` with capacity 1 for each black cell `b`,
* add `b -> w` with capacity 1 for each adjacent black–white pair,
* add `w -> T` with capacity 1 for each white cell `w`.

Then the maximum flow value equals the maximum matching size. A tiling exists iff:

1. the number of free cells is even, and
2. `maxflow = |V|/2` (every free cell is matched once).

---

## Complexity

Let `F = |V|` be the number of free cells. The grid has at most `2F` adjacency edges, so the flow network has:

* `V' = F + 2` vertices,
* `E' = O(F)` edges.

Using push–relabel max-flow:

* worst-case time `O((V')^3) = O(F^3)` (safe bound),
* memory `O(V' + E') = O(F)`.

## Points

```text
Testing solution >>>>
   Test set 1 (40 pts / 0.200 s) : Correct answer      (0.001s)
   Test set 2 (30 pts / 0.200 s) : Correct answer      (0.014s)
   Test set 3 (20 pts / 0.200 s) : Correct answer      (0.052s)
   Test set 4 (10 pts / 0.200 s) : Correct answer      (0.006s)

Total score: 100