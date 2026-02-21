# Croquet

## Algorithm tags (Codeforces-style)
**Delaunay · BFS · Nearest-Neighbor**

---

## Problem in a nutshell

We are given:

- `n` **safe points** `P = {p0, p1, ..., p(n-1)}` in 2D
- the **target** is `t = p(n-1)` (the last safe point)
- a distance threshold `d` (given as `q = d²` in the statement)
- an integer `k`
- `m` query points `s1..sm` (these are not necessarily safe points)

Define an **undirected** graph on the safe points:

- there is an edge between `pi` and `pj` iff `dist2(pi, pj) <= q`

A query point `s` is **reachable** iff:

1. `s` is within distance `d` of *some* safe point `p`  
2. and that safe point `p` can reach the target `t` in **at most `k-1` edges** in the safe-point graph.

For each query `s`, output:

- `"y"` if reachable
- `"n"` otherwise

---

## My Solution

A direct approach would build all edges where `dist2 <= q`, but that will be too slow resulting in a graph size of O(n^2).

Instead, we combine:

- **Delaunay triangulation** to support fast nearest-neighbor searches
- **BFS** from the target to compute which safe points are within `k-1` hops of `t`
- **incremental removal** of discovered points so each safe point is processed once

The initial solution is to build a Delaunay Triangulation for each layer of the frontier 
as we run BFS which results in a partial O(knlogn) solution. That is we add points to our visited set and reconstruct a delaunay triangulation of the visited set for each distance threshold. Notice that this results in a lot of repeated work. Instead of constructing a triangulation of the visited set what if we construct a triangulation of the unvisited set and remove points from that triangulation as we visit them 
in our BFS traversal?

That is we can build a Delaunay triangulation `DT_unvisited` over all safe points except the target.

This lets us efficiently:
- query a nearest safe point to a location (a visited point)

Hence the proposed algorithm works as follows: 

We BFS from the target and discover neighbors geometrically using the above Delaunay Triangulation

We want all safe points whose graph-distance to `t` is `<= k-1`.

Run BFS starting at `t`, but **do not** explicitly enumerate all edges.

For a BFS node `u`, its graph-neighbors are exactly the safe points `v` such that:

- `dist2(u, v) <= q`

To find them efficiently, we repeatedly do:

- use nearest-neighbor query on triangulation to extract safe points within radius `d` of `u`
- **remove** each extracted point from the triangulation
- those removed points become discovered BFS nodes at distance `dist[u] + 1`

Because each safe point is removed at most once, the total work stays near `n log n`.

Stop expanding when the BFS distance reaches `k-1`. This defines our frontier (since we need one hop to get to the safe points).

At the end, we have the set `R` = safe points that can reach `t` in at most `k-1` hops.

Build a second Delaunay triangulation `DT_frontier` over only the reachable safe points `R`.

For each query point `s`:

1. Find its nearest neighbor `p` in `DT_frontier`
2. Answer `"y"` iff `dist2(s, p) <= q`, else `"n"`

This works because reachability only needs:
- being close to **some** safe point that is within `k-1` hops of `t` (the computed frontier)

---

## Complexity

Let:

- `n` = number of safe points
- `m` = number of queries

Costs:

- Build `DT_all`: `O(n log n)`
- BFS discovery:
  - each safe point is removed/discovered once
  - each extract/remove costs `O(log n)` amortized  
  ⇒ `O(n log n)`
- Build `DT_frontier`: `O(|R| log n)` which is `O(n log n)` in the worst case
- Answer queries: `m` nearest-neighbor queries ⇒ `O(m log n)`

**Total time:** `O((n + m) log n)` per test instance  
**Space:** `O(n)`
