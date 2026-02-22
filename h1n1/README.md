# H1N1 - Escape Motion Planning: Voronoi Graph + Widest-Path (Max–Min) Connectivity — via Maximin Dijkstra or Offline Kruskal/DSU

## Algorithm tag

**Geometry + Delaunay + WidestPath + Dijkstra + DSU Offline Event Sweep**

---

## Problem in a nutshell

Given `n` points `P = {p_i} ⊂ R^2`. Consider the obstacle set formed by closed disks of radius `sqrt(d)` around each `p_i`. For each query `(q, d)` (a point `q` and a threshold `d > 0`) decide whether there exists a continuous path from `q` to infinity that stays at squared distance at least `d` from every `p_i`. Equivalently:

1. **start-feasibility:** `min_i |q - p_i|^2 >= d`;
2. **escape-feasibility:** in the free space `F_d := R^2 \ ( union_i Ball(p_i, sqrt(d)) )`, the point `q` lies in a connected component that touches the unbounded region.

The output is “y” iff both hold, else “n”.

(Everything is done in squared distances; corridor capacities are `|p_i - p_j|^2 / 4` because a disk of radius `sqrt(d)` passes between two sites iff `2*sqrt(d) <= |p_i - p_j|`.)

---

## Algorithm idea

The Delaunay triangulation is used to model the free space. Each finite Delaunay face represents a region of the plane; adjacent faces share an edge, and crossing that edge corresponds to squeezing through a “corridor” whose bottleneck radius is `|e| / 2`. This yields a dual graph on faces plus one special node **INF** for the unbounded region, with edge capacity

`w(e) = |p_i - p_j|^2 / 4.`

For a query threshold `d`, escaping to infinity is possible exactly when the face containing `q` is connected to **INF** using only edges with `w(e) >= d`. This is a **widest-path / bottleneck connectivity** problem on the dual graph.

Two equivalent ways to compute the needed information are:

* **Solution 1 (maximin Dijkstra):** compute for every face `f` the maximum bottleneck value `D[f]` along a path to **INF**, then answer a query by checking `D[f] >= d`.
* **Solution 2 (offline Kruskal/DSU):** process edges in descending capacity and union faces; for each query `d`, union all edges with `w >= d` and test if `f` is in the same component as **INF**.

---

# Solution 1 — Delaunay + “maximin Dijkstra” (widest-path to INF)

### What is built

* Delaunay triangulation on `P`.
* Dual graph `G_F`: one vertex per finite face plus a source node **INF**.
* For each face-edge adjacency, add an undirected edge with weight `w = |e|^2 / 4`; if the neighboring face is infinite, connect the face to **INF** with the same weight.

### What is computed

Run a Dijkstra-like algorithm from **INF**, but with:

* path value = minimum edge weight on the path (bottleneck),
* goal = maximize that bottleneck.

This is implemented by:

* `distance_combine = min`
* `distance_compare = greater`

so the algorithm computes

`D[f] = max over paths f -> INF of  min_{e in path} w(e).`

### How queries are answered

For each `(q, d)`:

1. If `min_i |q - p_i|^2 < d` => “n”.
2. Locate the containing face `f`. If `f` is infinite => “y” (already outside).
3. Else answer “y” iff `D[f] >= d`.

### Why this is correct

Any path from `q` to infinity must cross a sequence of Delaunay edges; the tightest “gap” along that route is the minimum `|e|/2`. Therefore the maximum feasible clearance is exactly the maximum bottleneck along face-to-INF paths, i.e. `D[f]`. So the decision reduces to `D[f] >= d`.

---

# Solution 2 — Delaunay + Offline Kruskal/DSU on edge capacities

### What is built

* Same Delaunay triangulation and the same face adjacency edges with weight `w = |e|^2 / 4`.
* Store all dual edges `(f, g, w)`, where `g` may be **INF**.

### What is computed

Sort edges by decreasing `w`. Sort queries by decreasing `d`. Sweep:

* While next edge has `w >= d`, union its two endpoints in DSU.
* After unions, the query’s face `f` can reach **INF** using only edges with `w >= d` iff `find(f) == find(INF)`.

### How queries are answered

For each `(q, d)` in descending `d`:

1. Start-feasibility check via nearest vertex: if `|q - p_nearest|^2 < d` => “n”.
2. If `q` lies in an infinite face => “y”.
3. Otherwise union all edges with `w >= d` and test if `f` is connected to **INF**.

### Why this is correct

Fix `d`. Consider the subgraph containing only edges with `w >= d`. Two faces are connected in this subgraph iff there exists a route between them that never squeezes through a corridor smaller than `sqrt(d)`. DSU after processing all `w >= d` edges exactly represents connected components of that subgraph, so connectivity to **INF** is equivalent to escape feasibility.

---

# How the two solutions relate (they are the same computation in different forms)

Both methods solve the same bottleneck connectivity question on the same graph:

* Define `G_{>= d}` as the subgraph of dual edges with `w >= d`.
  Then escape for a query is: `f` and **INF** are connected in `G_{>= d}`.

Now note the equivalence:

`D[f] >= d  <=>  f is connected to INF in G_{>= d}.`

So:

* **Maximin Dijkstra** precomputes all `D[f]` in one run and answers each query by one comparison.
* **Offline DSU** answers queries by dynamically building `G_{>= d}` for decreasing `d` using Kruskal’s union process.

This is also the classic relationship between **widest paths** and **maximum spanning forests**: DSU is Kruskal on decreasing weights; the bottleneck value `D[f]` is exactly the minimum edge weight on the max-spanning-tree path from `f` to **INF**.

---

## Complexity

Let `n` be sites, `m` queries. The number of finite faces and dual edges is `O(n)`.

**Common preprocessing**

* Delaunay triangulation: `O(n log n)`.
* For each query: nearest vertex + locate: typically `O(log n)`.

**Solution 1 (Dijkstra widest-path)**

* Build dual graph: `O(n)`.
* Widest-path Dijkstra: `O(n log n)`.
* Queries: `O(m log n)`.
* **Total:** `O(n log n + m log n)`.

**Solution 2 (Offline DSU sweep)**

* Sort edges: `O(n log n)`.
* Sort queries: `O(m log m)`.
* DSU unions/finds: `O((n + m) * alpha(n))`.
* Queries still need nearest+locate: `O(m log n)`.
* **Total:** `O(n log n + m log m + m log n)`.

## Points

```text
Testing solution >>>>
   Test set 1 (20 pts / 1 s) : Correct answer      (0.006s)
   Test set 2 (20 pts / 1 s) : Correct answer      (0.048s)
   Test set 3 (20 pts / 1 s) : Correct answer      (0.361s)
   Test set 4 (20 pts / 1 s) : Correct answer      (0.558s)
   Test set 5 (20 pts / 1 s) : Correct answer      (0.493s)

Total score: 100