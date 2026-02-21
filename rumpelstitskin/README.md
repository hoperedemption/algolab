# Rumpelstitskin

## Algorithm tag
**Min-Cost Max-Flow · Successive SP · Intersection of Flow Networks**

---

## The problem in a nutshell

Given two disjoint directed capacity networks `G` and `H` with sources `g0` and `h0`, and `n` intermediate “selection” nodes (islands)  
`I = {0, 1, ..., n-1}`, each usable at most once with profit `a_i`.

There are:

- directed capacity edges inside `G` and inside `H`,
- directed capacity edges from cities of `G` to islands and from cities of `H` to islands,
- supplies `s_G` at `g0` and `s_H` at `h0`.

A unit of production flow corresponds to a cobination **one unit of flow** received from `G` and **one unit of flow** received from `H` , all while respecting all capacities, and the fact that each island can produce one unit of flow at most.

Compute:

1. `y`: the maximum number of units that can be produced, and
2. `s`: among all flows achieving `y`, the maximum total profit `sum(a_i)` of used islands.

---

## High-level algorithm idea

Turn it into a **min-cost max-flow** where “using an island” gives profit.

1. Compute `A = max_i a_i`.
2. For each island, put the “use at most once” bottleneck on a single edge (the island-selection edge) with capacity `1`.
3. Assign that island edge a cost of `A - a_i`.

This trick allows to use the min cost solver for a maximization of cost problem given that all costs
have to be non-negative.

Build one combined flow network that routes:

`super-source -> G -> island-selection edge -> reversed(H) -> super-sink`

while keeping all original capacities. Reversing the edges from the islands to H and reversing the edges within H itself allows to solve the conjunction of flow problem correctly. This is because we do not introduce any disontinuities. The flow is continuous. 

Then run **successive shortest augmenting paths** (nonnegative costs) to obtain a min-cost flow. This procedure also pushes as much flow as possible given the supplies/capacities, so you get the maximum `flow`.

Finally, recover the maximum profit as:

`profit = A * flow - min_cost`

---

## Complexity analysis

Let `V` and `E` be the vertex/edge counts of the constructed network:

- `V = Theta(p_G + p_H + n)`
- `E = Theta(e_G + e_H + f_G + f_H + n)`  
  (up to constant factors for reverse edges)

Let `F = y` be the max flow value.

Using **successive shortest path with nonnegative costs**:

- **Time:** `O(F * (E log V))` with Dijkstra-based successive shortest paths algorithm 
- **Space:** `O(V + E)`.
