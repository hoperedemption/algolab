# Marathon - Max Flow on Shortest-Path DAG

## Algorithm tag
**Dijkstra + MaxFlow + Shortest-Path DAG**

---

## The problem in a nutshell

Given an undirected multigraph `G = (V, E)` with `|V| = n`, start `s`, finish `f`. Each edge `e = {u, v}` has **length** `l_e > 0` and **capacity** `c_e > 0`. Let `dist(.)` be shortest-path distances from `s` w.r.t. `l`. A walk from `s` to `f` is admissible iff it is a shortest path, i.e. it has total length `dist(f)`. Runners may split across admissible shortest paths, and if `x` runners use an edge then `x <= c_e`. Compute the maximum total number of runners that can be routed from `s` to `f` using only shortest paths.

---

## High-level algorithm idea

First compute `dist(v)` for all vertices using Dijkstra on edge lengths. Then keep only edges that can lie on a shortest path and orient them “forward in distance”: for an undirected edge `{u, v}` of length `l`, add a directed arc `u -> v` with capacity `c` if `dist(u) + l = dist(v)` (and similarly `v -> u` if the reverse equality holds). This creates a shortest-path subgraph (which is a Directed Acyclic Graph) where every `s -> f` directed path corresponds to a shortest path in the original graph. The answer is exactly the value of a max flow from `s` to `f` in this directed capacity graph.

---

## Complexity analysis

Let `m = |E|`.

- **Dijkstra:** `O(m log n)`.
- **Build shortest-path digraph:** `O(m)`.
- **Max flow (push–relabel):** on `n` vertices and `O(m)` arcs, worst-case `O(n^3)`.

**Total per test case:** `O(m log n + n^3)` time, `O(n + m)` space.

## Points

```text
Testing solution >>>>
   Test set 1 (25 pts / 1 s) : Correct answer      (0.028s)
   Test set 2 (25 pts / 1 s) : Correct answer      (0.027s)
   Test set 3 (25 pts / 1 s) : Correct answer      (0.012s)
   Test set 4 (25 pts / 1 s) : Correct answer      (0.033s)

Total score: 100
