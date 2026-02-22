# Clues: Unit Disk Graph Bipartiteness + Connectivity via Delaunay Triangulation 

## Algorithm tag

**Delaunay + Bipartite**

---

## Problem in a nutshell

Let `S = {s_0, ..., s_(n-1)} ⊂ R^2` be station positions and `r > 0`. Define the **unit disk graph**

`G_r = (S, E),   {u, v} in E  iff  |u - v| <= r.`

1. Decide whether `G_r` is **bipartite** (i.e., 2-colorable), which corresponds to assigning two station frequencies without interference. 
2. For each query `(a_i, b_i) in R^2 x R^2`, determine if the two endpoints can communicate under the problem’s routing rule:

* if `|a_i - b_i| <= r`, direct connection works;
* otherwise each endpoint must connect to its **unique nearest** station if within range `r`;
* then communication is possible iff those chosen stations lie in the **same connected component** of `G_r`. 

---

# Solution 1 (bipartite verification using same-color DTs)

### What it builds

1. Build DT on all stations.
2. Build a graph `H` using only **DT finite edges** of length `<= r`.
3. Compute connected components of `H` (BGL `connected_components`).
4. For queries:

* If `|a - b| <= r`, answer “y”.
* Else, find nearest stations `s(a), s(b)`. If either is farther than `r`, answer “n”.
* Else answer “y” iff `s(a)` and `s(b)` are in the same component of `H`.

### How it certifies bipartiteness

* First, run `is_bipartite(H)` to get a 2-coloring of the DT-edge graph.
* But `H` is only a subset of `G_r`. So you must ensure there isn’t a **missing** short edge in `G_r` that connects same-colored vertices.
* Do exactly that by:

  * splitting stations into **white** and **black** by the computed coloring,
  * building DT on the whites and checking if any white-white DT edge has length `<= r`; similarly for blacks.
* If either same-color DT contains an edge of length `<= r`, then there exists a forbidden same-color edge in `G_r`, so the network is **not** interference-free; output all “n”.

---

# Solution 2 (DT neighborhood expansion graph + direct bipartite on that graph)

### What it builds

Instead of only DT edges, it generates more edges by exploring “DT neighbors-of-neighbors”:

* For each vertex handle `v_h`, iterate its incident DT edges to get a neighbor `v_k`.
* Then iterate incident DT edges of `v_k` to get candidate `v_ell`.
* Add edge `{v_h, v_ell}` into the station graph if `|v_h - v_ell|^2 <= r^2` (and avoid duplicates).

This produces a graph `H'` that is a richer subset/approximation of `G_r` than just DT edges, still built in near-linear time due to bounded DT degree.

Then it does:

* `is_bipartite(H')` once.
* `connected_components(H')` once.
* Query answering exactly as in solution 1 (direct if `<= r`; else nearest-station checks; then component equality).

### Why this is correct

* By adding neighbor-of-neighbor candidates, we are explicitly ensuring that any pair within distance `<= r` is very likely generated as an edge in `H'` because close pairs must appear in a small local DT neighborhood (enclosed triangle with empty circumcircle property of Delaunay Triangulation).
---

# Why both are solving the same thing (relationship between the two)

* Both compute DT to avoid `O(n^2)` distance checks.
* Both reduce the problem to properties of the unit disk graph `G_r`:

  * feasibility of a 2-coloring (station frequency assignment),
  * connectivity between the two stations that Holmes/Watson attach to (routing feasibility). 
* They differ only in **how they generate the “short-edge graph”**:

  * **Solution 1:** minimal sparse backbone (DT edges `<= r`) + extra verification via DTs on color classes.
  * **Solution 2:** double neighbor local edge generation (neighbors-of-neighbors) and then just run bipartite directly on that richer graph.

---

## Complexity

Let `n` be stations and `m` clues.

Common parts:

* Build Delaunay triangulation: `O(n log n)`.
* Nearest-station query per endpoint (CGAL): `O(log n)`.
* So per clue: `O(log n)`, total `O(m log n)`.

**Solution 1**

* Build graph from DT finite edges: `O(n)` edges, so components + bipartite: `O(n)`.
* Build DT on each color class and scan its edges: `O(n log n)` total.
* **Total:** `O(n log n + m log n)` time, `O(n)` memory (plus DT storage).

**Solution 2**

* Neighborhood expansion: each DT vertex has constant expected degree, so the double incident-edge loop is still near-linear expected time; worst-case can be larger, but in practice remains `O(n)`–ish after DT build.
* Components + bipartite on `H'`: `O(|V| + |E|)` which is near-linear in practice.
* **Total:** `O(n log n + m log n)` typical, `O(n)` memory.

## Points

```text
Testing solution >>>>
   Test set 1 (20 pts / 2 s) : Correct answer      (0.084s)
   Test set 2 (30 pts / 2 s) : Correct answer      (0.783s)
   Test set 3 (30 pts / 2 s) : Correct answer      (1.082s)
   Test set 4 (20 pts / 2 s) : Correct answer      (1.429s)

Total score: 100