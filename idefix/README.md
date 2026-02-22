# Idéfix and the Mansions of the Gods: Delaunay Triangulation + Kruskal (Minimum Spanning Forest) with DSU Event Sweep

## Algorithm tag

**Geometry + Delaunay + DSU + Greedy + Event Sweep**

---

## Problem in a nutshell

Let `T = {t_1, ..., t_n} ⊂ R^2` be tree centers and `B = {b_1, ..., b_m} ⊂ R^2` be bone locations (bones may coincide). For a radius `r >= 0`, define the **intersection graph of disks**

`G(r) = (T, E(r)),   {t_i, t_j} in E(r)  iff  |t_i - t_j| <= 2r.`

A walk inside the union of closed disks of radius `r` is possible between two disks iff their centers lie in the same connected component of `G(r)`. A bone at position `b` is *inspectable* iff it lies in at least one disk, i.e. iff

`min_{t in T} |b - t| <= r.`

Let `C(r)` be the multiset of connected components of `G(r)`, and for each component `C in C(r)` let `#bones(C)` be the number of bones whose nearest tree lies in `C` **and** are within distance `r` of that tree (ties don’t matter for counting in a component).

Given the input parameter `s = 4r^2` and integer `k`, output:

1. `a = max_{C in C(r)} #bones(C)` for the fixed radius `r` (equivalently threshold `s`);
2. `q = 4b^2` where `b` is the smallest radius such that `max_{C in C(b)} #bones(C) >= k`. 

---

## Algorithm logic 

Treat increasing radius as a **Kruskal-like process** where components merge when the radius is large enough for two disks to touch.

Two observations make it fast:

* **Edges worth considering:** connectivity of disk intersections can be handled using only candidate edges from the **Delaunay triangulation** of the tree centers .
* **Single monotone sweep:** create an event list consisting of

  * **bone events** at value `d_b = 4 * |b - nearest tree|^2` (the smallest `4r^2` that covers that bone), and
  * **merge events** for each Delaunay edge `(i, j)` at value `d_ij = |t_i - t_j|^2` (the smallest `4r^2` for which disks of radius `r` intersect, because `|t_i - t_j| <= 2r` iff `|t_i - t_j|^2 <= 4r^2`).

Sort all events by increasing value and process them with a **DSU**:

* Bone event: add one bone to the DSU component of its nearest tree.
* Edge event: union the two tree components and sum their bone counts.

While sweeping:

* For **(a)**, track the maximum component bone count among all events with value `<= s`.
* For **(q)**, record the first event value at which some component reaches `>= k` bones (that value is exactly the minimal `q = 4b^2`).

This is Kruskal/DSU on a geometric graph, with extra “activation” events for bones.

---

## Complexity

Let `n` be trees, `m` bones.

* Delaunay triangulation: `O(n log n)`.
* Nearest-tree query per bone (via DT): `O(m log n)` typical.
* Number of Delaunay edges: `O(n)`, so total events (= `O(n + m)`).
* Sort events: `O((n + m) log(n + m))`.
* DSU operations: `O((n + m) * alpha(n))`.

**Total:** `O(n log n + m log n + (n + m) log(n + m))` time, `O(n + m)` memory.

## Points

```text
Testing solution >>>>
   Test set 1        (20 pts / 1 s) : Correct answer      (0.016s)
   Hidden test set 1 (05 pts / 1 s) : Correct answer      (0.016s)
   Test set 2        (20 pts / 1 s) : Correct answer      (0.084s)
   Hidden test set 2 (05 pts / 1 s) : Correct answer      (0.084s)
   Test set 3        (20 pts / 1 s) : Correct answer      (0.026s)
   Hidden test set 3 (05 pts / 1 s) : Correct answer      (0.026s)
   Test set 4        (20 pts / 1 s) : Correct answer      (0.257s)
   Hidden test set 4 (05 pts / 1 s) : Correct answer      (0.13s)

Total score: 100