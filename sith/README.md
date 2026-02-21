# Revenge of the Sith - Binary search on (k) and unit-disk connectivity 

### Algorithm tag

**Delaunay, Geometry, Binary Search, BFS, Connected Components** 

---

### The problem in a nutshell

We are given points `p0, ..., p(n-1)` in a fixed order and a radius `r`. For any integer `k`, consider the set of “available” points

`S_k = {p_k, p(k+1), ..., p(n-1)}`.

Define the undirected **unit-disk graph** `G_k = (S_k, E_k)` where `{u, v} in E_k` iff `|u - v| <= r`.
A set of `k` distinct points `R ⊆ S_k` is **visitable** iff there exists an ordering `r0, ..., r(k-1)` of `R` such that for every `j >= 1` there exists `i < j` with `|r_j - r_i| <= r`.
This is equivalent to: **`R` is contained in a connected component of `G_k`**, hence such an `R` exists iff `G_k` has a connected component of size at least `k`.
Compute the maximum `k` for which `G_k` contains a connected component of size `>= k`. 

---

### High-level algorithm idea

Binary search the answer `k` since clearly the problem is monotone in `k`. For a candidate `k = mid`, build the unit-disk graph on the remaining points `S_mid`, but do it implicitly using a Delaunay triangulation: only Delaunay edges need to be checked to find all “close neighbor” connections relevant for connectivity. Note that the Delaunay Triangulation has to be constructed again for each candidate k as it induces a different set of points. Add an undirected edge between any pair of Delaunay-adjacent points whose squared distance is `<= r^2`. Then run BFS/DFS to compute connected component sizes; if any component has size `>= mid`, the candidate is feasible and you search higher, otherwise lower.

---

### Complexity analysis

Let `n` be the number of points.

* Each feasibility check for a fixed `k`:

  * Build Delaunay triangulation on `|S_k| = n - k` points: `O((n - k) log(n - k))`.
  * Scan finite Delaunay edges and keep those with length `<= r`: `O(n - k)` edges, hence `O(n - k)` work.
  * BFS over the kept adjacency: `O(n - k)`.

So one check is `O((n - k) log(n - k)) ⊆ O(n log n)`.
Binary search performs `O(log n)` checks, giving:

* **Time:** `O(n log^2 n)` per test case (as implemented by rebuilding the triangulation each mid).
* **Space:** `O(n)` for points, triangulation, adjacency, and BFS state.

## Points

```text
Test set 1 (25 pts / 2 s) : Correct answer      (0.066s)
Test set 2 (25 pts / 2 s) : Correct answer      (0.194s)
Test set 3 (25 pts / 2 s) : Correct answer      (0.62s)
Test set 4 (25 pts / 2 s) : Correct answer      (1.399s)

Total score: 100