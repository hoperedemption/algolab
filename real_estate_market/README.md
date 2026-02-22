# Real Estate Market: Min-Cost Max-Flow for Capacitated Assignment

## Algorithm tag

**Min-Cost Flow**

---

## Problem in a nutshell

Given:

* buyers `i in {1, ..., N}`,
* sites `j in {1, ..., M}`,
* states `k in {1, ..., S}` with sale limits `l_k`,
* site-to-state map `sigma(j) in {1, ..., S}`,
* bids `b_ij in [1, 100]`.

Choose a set of assignments `x_ij in {0, 1}` maximizing lexicographically:

1. the number of sold sites (`c = sum_{i,j} x_ij`),
2. subject to that, total profit (`p = sum_{i,j} b_ij * x_ij`),

under constraints:

`sum_j x_ij <= 1  for all i   (each buyer buys at most one),`

`sum_i x_ij <= 1  for all j   (each site sold at most once),`

`sum over j with sigma(j) = k of  sum_i x_ij <= l_k  for all k   (state capacity).`

Output `(c, p)`. 

---

## Algorithm logic

Model this as a **min-cost flow** on a layered network:

* Source → each buyer with capacity 1 (enforces “buy at most one”).
* Buyer `i` → site `j` with capacity 1 and cost `100 - b_ij` (turn profit maximization into cost minimization with nonnegative costs).
* Each site `j` → its state node `sigma(j)` with capacity 1 (site sold at most once; also routes flow into the correct state bucket).
* Each state node `k` → sink with capacity `l_k` (enforces the per-state sale limit).

Sending one unit of flow corresponds to selling one site to one buyer in some state. Running **Successive Shortest Augmenting Path** finds, among all maximum flows, the one with minimum total cost, i.e. maximum total bid sum. Finally convert back:

`p = 100 * c - mincost.`

---

## Complexity

Let `V = N + M + S + 2` and `E ≈ N*M + M + N + S = O(N*M)`.

With SSAP (Dijkstra-based, nonnegative costs), if `F = c <= min(N, M)` is the flow value:

`O(F * E * log V) = O(min(N, M) * (N*M) * log(N + M + S)).`

Memory: `O(V + E) = O(N*M)`.

## Points

```text
Testing solution >>>>
   Test set 1 (20 pts / 1 s) : Correct answer      (0.011s)
   Test set 2 (20 pts / 1 s) : Correct answer      (0.019s)
   Test set 3 (20 pts / 1 s) : Correct answer      (0.013s)
   Test set 4 (20 pts / 1 s) : Correct answer      (0.016s)
   Test set 5 (20 pts / 1 s) : Correct answer      (0.094s)

Total score: 100