# Germs: Delaunay Triangulation Event Sweep

## Algorithm tag

**Geometry in 2D + Delaunay + Sort + Event Sweep**

---

## Problem in a nutshell (math, no story)

Given `n` points `p_i = (x_i, y_i)` inside an axis-aligned rectangle `[l, r] x [b, t]`. Each point represents a disk whose radius grows as

`rho(tau) = tau^2 + 1/2   (tau >= 0).`

Disk `i` “dies” at the smallest time `tau_i` such that its grown disk touches either:

* the boundary of the rectangle, or
* another disk.

In the simplified model, all disks keep growing; therefore `tau_i` depends only on the **first contact event** determined by the nearest obstacle:

Let

`delta_i = min( dist(p_i, boundary of R),  (1/2) * min_{j != i} |p_i - p_j| ).`

Then `tau_i` is the smallest `tau` with `rho(tau) = delta_i`, i.e.

`tau_i = 0                      if delta_i <= 1/2,`
`tau_i = sqrt(delta_i - 1/2)     if delta_i > 1/2.`

The required outputs are the (rounded-up-to-next-integer) times of:

* the first death,
* the moment when alive count drops below `50%`,
* the last death. 

---

## Algorithm idea

The task reduces to computing `delta_i` for every point. The boundary part is just the minimum distance to the four rectangle sides. The pairwise part needs the nearest neighbor distance `min_{j != i} |p_i - p_j|`. Computing all nearest neighbors naively is `O(n^2)`, so the Delaunay triangulation is used: for each point, its nearest neighbor is guaranteed to be among its Delaunay neighbors. Therefore:

1. Build the Delaunay triangulation of the `n` centers.
2. Initialize each `delta_i` from the rectangle boundary distance.
3. For each Delaunay edge `(i, j)`, update `delta_i` and `delta_j` with `|p_i - p_j| / 2`.
4. Convert each `delta_i` to a death time `tau_i` using `rho(tau) = tau^2 + 1/2`, then apply the required ceiling rule from the statement. 
5. Sort the `n` death times and pick the 1st, median (index `floor(n/2)` as in the code), and last.

This matches the implementation: `node_map[i]` stores squared clearance (either from boundary via `((2*dist)^2)` or from neighbor edge length squared), then after sorting it is mapped back to time via

`delta = (1/2) * sqrt(node_map),   tau = sqrt(delta - 1/2),`

and rounded up.

---

## Complexity

* Delaunay triangulation construction: `O(n log n)`.
* Scanning all incident Delaunay edges: `O(n)` edges total in planar DT, hence `O(n)` updates.
* Sorting death times: `O(n log n)`.

**Total:** `O(n log n)` time, `O(n)` memory.


## Points

```text
Testing solution >>>>
   Test set 1 (20 pts / 0.600 s) : Correct answer      (0.003s)
   Test set 2 (20 pts / 0.600 s) : Correct answer      (0.082s)
   Test set 3 (30 pts / 0.600 s) : Correct answer      (0.312s)
   Test set 4 (30 pts / 0.600 s) : Correct answer      (0.279s)

Total score: 100