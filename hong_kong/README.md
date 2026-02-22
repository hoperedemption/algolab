# Hong Kong - Widest-Path, Maximum Bottleneck Connectivity in the “inflated obstacles” free space with multiple escape cells in the Voronoi Diagram

## Algorithm tag

**Delaunay, Widest-Path, Maximum Bottleneck, Voronoi Diagram, Greedy, Disjoint Set Union, Maximin Dijkstra**

---

## Problem in a nutshell

Given **tree centers** `A = {a_i}_{i=1..n} ⊂ R^2` and tree radius `r`. For each query balloon `b = (p, s)` with center `p in R^2` and radius `s`, let

`R := r + s`.

The balloon center may move continuously in the **free space**

`F_R := R^2 \ ( union_{i=1..n} Ball(a_i, R) )`

(where balls are open; touching is allowed). Output **“y”** iff:

1. `p in F_R` (inflation feasible at the start), and
2. there exists a path in `F_R` from `p` to some position `t in F_R` such that the balloon has **takeoff clearance >= R** (equivalently `t in F_R`, and it lies in a region where the local clearance is at least `R`). 

Geometrically: decide if `p` lies in a connected component of `F_R` that contains either “infinity” (can escape arbitrarily far) **or** contains a point whose distance to the nearest tree center is at least `R`.

---

# Solution 1: Delaunay faces + offline Union-Find on descending “passage widths”

## Core geometric reduction

We build the **Delaunay triangulation** of tree centers. Work in the **dual free-space structure**:

* **Nodes = faces** of the Delaunay triangulation (equivalently they are the vertices of the Dual Voronoi Diagram), plus one special node **INF** representing the unbounded (infinite) region.
* **Adjacency = shared Delaunay edges** (two faces are neighbors if they share an edge; finite face to INF if the edge borders the infinite face).

Each adjacency (dual edge) corresponds to a “corridor” between two trees. The **maximum disk radius that can pass through** that corridor is half the distance between the two tree centers:

`corridor_radius = |a_i - a_j| / 2`

so:

`corridor_radius^2 = |a_i - a_j|^2 / 4`.

Hence we store `dt.segment(e).squared_length()/4`.

Inside a **finite face** (a Delaunay triangle), the best “resting spot” (max clearance from the 3 trees) is the triangle’s circumcenter, with clearance equal to the circumradius. Hence we store:

`face_clearance^2 = |c_f - a|^2`

for a vertex `a` of that face (all equal). 

## What a query needs

For a query with `R^2 = (r + s)^2`:

1. **Inflation feasibility at start**: if nearest tree center is closer than `R`, answer “n”.
2. Otherwise, locate which face `f` contains `p`.
3. The balloon can move through a corridor iff that corridor’s `(|a_i - a_j|^2 / 4) >= R^2`.
4. The balloon can “wait/take off” in a component if some face in that component has clearance `>= R`. Since this is related to the specific connected component it is part of, naturally this becomes a component attribute `clearance[component]`.

## The offline Union-Find

We sort:

* all dual edges by corridor width (descending),
* all queries by `R^2` (descending).

Then we sweep from large `R` to small `R` (i.e in descending order):

* When processing a query threshold `R^2`, **union** all face-adjacencies whose corridor width `>= R^2`. After these unions, two faces are connected **iff** the balloon of radius `R` can travel between them without intersecting any inflated tree-disk. Thus we connect the components under that threshold feasibility.
* We also maintain, per connected component, the **maximum face clearance** among its faces ( `clearance[root] = max(clearance[c1], clearance[c2])`) for all c1, c2 part of that root's component set.
* Finally, the query is “y” iff:

  * its face component is connected to INF (escape exists), **or** it already lies in INF; OR
  * the component’s max clearance is large enough for takeoff already.

Concretely, our final check is:

```cpp
if (component == INF_component || clearance[rep] >= 4*R^2) -> 'y'
````

Note that the `4*R^2` is purely a **unit normalization** artifact: the corridor weights were stored as `(|e|^2 / 4)` but face clearance was stored as `(|.|^2)` (no `/4`), so we MUST multiply the query threshold by 4 to compare apples-to-apples (same units).

## High-level idea summarized

 Turn motion planning into **connectivity in the voronoi graph**, where an edge exists only if the balloon fits through the narrowest gap between two trees. By sorting gaps from widest to narrowest and unioning regions, we can answer each balloon query at the moment the balloon radius becomes feasible, in near-linear time.

## Complexity

Let `n` be trees, `m` balloons.

* Delaunay triangulation: `O(n log n)`
* Number of Delaunay edges/faces: `O(n)`
* Sorting edges: `O(n log n)`
* Sorting queries: `O(m log m)`
* Union-Find sweep: `O((n + m) * alpha(n))`
* Each query also does `nearest_vertex` and `locate`: `O(log n)` each.

**Total:** `O(n log n + m log m + m log n + (n + m) * alpha(n)) = O((n+m)logn + mlogm)` time, `O(n + m)` memory.

---

# Solution 2: Same graph, but compute widest path via “maximin Dijkstra”

## Same underlying Voronoi graph, different way to compute reachability

Again use Delaunay faces + INF as nodes. Edge weights are still “how big a balloon can pass”:

* between neighboring faces: `|e|^2 / 4`
* from a face to INF: also `|e|^2 / 4` for boundary edges
* plus a “takeoff-in-this-face” capacity: add an edge from the face to INF with weight `circumradius^2 / 4` (this is multiple sources/targets trick in a graph)

So every edge weight is now consistently “radius^2 capacity”.

## The key algorithmic pattern: widest path

For each face `f`, define:

`D[f] := max over paths P from f to INF of  min_{e in P} w(e).`

This is the **widest-path / maximum bottleneck path** value: the largest radius^2 that can be guaranteed along some route from `f` to INF.

Compute `D[.]` with a Dijkstra-like algorithm, but with:

* **combine** = `min` (path capacity is bottleneck),
* **relax** = `max` (we want best bottleneck),
* ordering by larger is better.

That’s exactly what `distance_compare(std::greater<FT>())` and `distance_combine(min)` implement.

## Answering a query

For balloon radius `R`:

1. If nearest tree is within distance `R`: “n”.
2. If located face is infinite: “y” (already outside).
3. Else, let `f` be the containing face; answer “y” iff:

`D[f] >= R^2.`

Because `D[f]` already encodes the best achievable bottleneck to escape **and** includes the face’s own takeoff feasibility via the circumradius edges.

## High-level idea summarized

Precompute, for every region of space, the **largest balloon size** that can safely reach open space (INF) without ever squeezing through a too-narrow gap. Then each query is a single threshold check against that precomputed “escape capacity”.

## Complexity

* Delaunay triangulation: `O(n log n)`
* Graph has `O(n)` vertices and edges.
* Widest-path Dijkstra: `O(n log n)`
* Each query does nearest + locate: about `O(log n)`.

**Total:** `O(n log n + m log n)=O((n+m)logn)` time, `O(n)` memory for the graph + distances.

---

# How the two solutions are the same (and why both get full points)

Both solutions solve the **same decision problem** on the **same capacitated dual Voronoi graph**:

* Nodes = Delaunay faces + INF
* Edge capacity = maximum balloon radius that can cross that boundary (derived from half-edge lengths; face capacity from circumradius)

They only differ in *how* they compute the set of faces reachable by a given radius `R`:

* **Solution 1 (Union-Find sweep):** answers many thresholds offline by adding edges in decreasing capacity order.
* **Solution 2 (Widest-path):** computes, once, the maximum feasible radius for each face to reach INF or its a node with better takeoff clearance, then answers queries online by comparison.

Mathematically, both are exploiting the same equivalence:

`exists a path from face f to INF using only edges with w(e) >= R^2  <=>  D[f] >= R^2.`

---

In short: both of solutions are two faces of the same coin—**maximum bottleneck reachability** in a planar, Euclidean-derived graph; Union-Find is the Kruskal/MaxST offline view, and modified Dijkstra is the widest-path online-precompute view.

## Points

```text
Testing solution >>>>
   Test set 1 (30 pts / 2 s) : Correct answer      (1.163s)
   Test set 2 (30 pts / 2 s) : Correct answer      (0.51s)
   Test set 3 (20 pts / 2 s) : Correct answer      (0.886s)
   Test set 4 (20 pts / 2 s) : Correct answer      (1.158s)

Total score: 100


