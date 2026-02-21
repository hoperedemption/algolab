# Return of the Jedi - Second-Best Minimum Spanning Tree (via edge exchange on an MST / Prim tree)

### Algorithm tag 

**MST, Prim, Second-Best MST, Tree BFS** 

---

### The problem in a nutshell

Given a complete undirected graph `G = (V, E)` with `|V| = n` and positive edge weights `w : E -> Z_{>0}`, and a fixed start vertex `i`. Let `T` be the spanning tree produced by **Prim’s algorithm started at `i`** with deterministic tie-breaking (by vertex “importance”, i.e. index order).
Compute

`min { w(T') : T' is a spanning tree of G, T' != T }`

where `w(T') = sum_{e in T'} w(e)`. 

---

### High-level algorithm idea

First, reproduce Leia’s tree `T` using Prim with the required tie-breaking, and record its total weight. Any other spanning tree `T' != T` can be obtained from `T` by adding one non-tree edge `(u, v)` (creating a cycle) and removing exactly one edge on the unique `u -> v` path in `T`. To minimize the new total weight, for each non-tree edge `(u, v)` we should remove the **maximum-weight edge** on that path (best exchange). So we precompute, for every pair `(u, v)`, the maximum edge weight on the path between them in `T`; then scan all non-tree edges and take

`w(T) - maxEdge_T(u, v) + w(u, v)`

keeping the smallest value, which is the cheapest spanning tree that differs by at least one edge. 

---

### Complexity analysis

* Building `T` with Prim on the dense graph: `O(n^2 log n)` in this implementation .
* Computing `maxEdge_T(u, v)` for all pairs by BFS from every source on the tree: `O(n^2)` (tree has `n - 1` edges).
* Scanning all non-tree edges (complete graph): `O(n^2)`.

**Total:** `O(n^2 log n)` time per test case
**Space:** `O(n^2)` due to storing the full weight matrix (and the all-pairs max-edge table).

## Points

```text
Testing solution >>>>
   Test set 1 (40 pts / 2 s) : Correct answer      (0.007s)
   Test set 2 (40 pts / 2 s) : Correct answer      (0.444s)
   Test set 3 (20 pts / 2 s) : Correct answer      (0.792s)

Total score: 100
