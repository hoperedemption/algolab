# Surveillance Photograph - Max Flow on Layered Graph

## Algorithm tags
**Max Flow · Graph Layering · Push–Relabel**

---

## Problem in a nutshell

Given a **directed multigraph** `G = (V, E)` with `|V| = n`.

- There are `k` policemen placed on vertices (multiple policemen may start on the same vertex).
- There are `l` photographs placed on vertices (multiple photos may be on the same vertex).
- Some vertices are **police stations** (where photos can be deposited).

A policeman may:

1. travel along directed edges to reach a vertex that has at least one photograph,
2. pick up **at most one** photograph,
3. then travel (possibly via different routes) to a police station and deposit it.

Constraints:

- Each policeman carries **at most one** photo.
- Each deposited photo occupies one “safe slot”; equivalently, the number of photos that can be deposited at a station is limited by how many policemen/safes are available there.
- **While carrying a photo**, each directed edge may be used by **at most one** policeman (capacity `1`).
- **Without carrying a photo**, edge usage is unconstrained.

**Goal:** compute the maximum number of photographs that can be collected and deposited.

---

## High-level algorithm idea

The key trick is to separate movement into two distinct phases:

- **before pickup** (not carrying a photo)
- **after pickup** (carrying a photo)

We model this using **two layers** of the graph:

- **Pre-pickup layer**: free movement (no edge congestion constraints)
- **Post-pickup layer**: congested movement (each street edge can carry at most one photo-carrier)

### 1) Duplicate the graph into two layers

For every original vertex `v`, create:

- `v_pre`  (before pickup)
- `v_post` (after pickup)

### 2) Travel edges

For each directed edge `(u → v)` in the original graph:

- Add `(u_pre → v_pre)` with **INF capacity** (unlimited travel when not carrying).
- Add `(u_post → v_post)` with capacity **1** (exclusive travel when carrying).

### 3) Pickup edges (photos)

If vertex `v` contains `photos[v]` photographs, add:

- `(v_pre → v_post)` with capacity `photos[v]`

This enforces that at most `photos[v]` policemen can transition into the “carrying a photo” state at `v`.

### 4) Add source for policemen

If vertex `v` initially has `police[v]` policemen, connect:

- `(S → v_pre)` with capacity `police[v]`

This limits how many units of flow (policemen) can start moving.

### 5) Add sink for depositing at stations

If `v` is a police station with `safe[v]` available deposit slots (often equal to the number of policemen/safes there), connect:

- `(v_post → T)` with capacity `safe[v]`

This enforces that depositing is limited by station capacity.

---

## Why max flow equals the answer

Effectively **one unit of flow** = “one policeman who successfully picks up and deposits one photo”.

- Entering the post-layer requires passing through `(v_pre → v_post)`, which consumes one photo at `v`.
- Moving in the post-layer uses capacity-1 edges, enforcing that each street is used by at most one photo-carrier (edge disjoint paths).
- Reaching `T` requires passing through a station’s `(v_post → T)`, enforcing deposit capacity.
- Since each unit of flow can only cross from pre to post once, each policeman carries **at most one** photo.

Therefore, the **maximum number of collectible photos** is exactly the **maximum `S–T` flow** in this layered network.

---

## Complexity

Let:

- original graph has `n` vertices and `m` directed edges
- flow network has:
  - `N = 2n + 2` vertices (two layers + `S` and `T`)

Complexities:

- **Time (worst-case):** `O(N^3)` for push–relabel  
  ⇒ `O((2n)^3) = O(n^3)` in theory  
- **Space:** `O(N + M) = O(n + m)` to store the graph

## Points

```text
Testing solution >>>>
   Test set 1 (50 pts / 1.500 s) : Correct answer      (0.155s)
   Test set 2 (50 pts / 1.500 s) : Correct answer      (0.248s)

Total score: 100
