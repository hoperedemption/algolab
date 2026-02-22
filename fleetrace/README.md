# Fleetrace: Min-Cost Max-Flow for Maximum-Weight Bipartite Matching

## Algorithm tag

**Min-Cost Max Flow**

---

## Problem in a nutshell

Given a bipartite graph `G = (B ∪ S, E)` with boats `B` and sailors `S`. Each edge `(b, s) in E` has a profit `c_bs in [1, 50]`. Find a matching `M ⊆ E` (each vertex incident to at most one chosen edge) maximizing total profit(but not necessarilly maximizing matching size):

`max over matchings M of  sum_{(b,s) in M} c_bs.`

Output the maximum value.

---

## Algorithm logic

Reduce maximum-profit matching to **min-cost flow**. Create a source connected to each boat with capacity 1, connect each sailor to the sink with capacity 1, and for each compatible pair `(b, s)` add an edge of capacity 1 whose **cost** encodes negative profit (you use `50 - c_bs` to keep costs nonnegative). To allow “not matching a boat”, also add a direct boat→sink edge with fixed cost `50`, ensuring every unit of flow from a boat either chooses a sailor (paying `50 - c`) or opts out (paying `50`). Then run **Successive Shortest Augmenting Path with nonnegative weights** to send as much beneficial flow as possible while minimizing cost. Finally we must convert back from cost to profit using

`profit = 50 * flow - mincost.`

---

## Complexity

Let `V = b + s + 2` and let `p` be the number of compatible pairs (pair edges). The network has:

- pair edges: `p`
- source→boat edges: `b`
- sailor→sink edges: `s`
- boat→sink edges: `b`

So total edges `E = O(p + b + s)`. The flow value is at most `min(b, s) <= 500`.

With SSP min cost max flow, worst-case is:

`O(F * E * log V)`, where `F <= min(b, s).`

So overall:

`O(min(b, s) * (p + b + s) * log(b + s)).`

Memory: `O(V + E)`.

## Points

```text
Testing solution >>>>
   Test set 1 (25 pts / 5 s) : Correct answer      (0.036s)
   Test set 2 (25 pts / 5 s) : Correct answer      (0.013s)
   Test set 3 (25 pts / 5 s) : Correct answer      (0.054s)
   Test set 4 (25 pts / 5 s) : Correct answer      (1.319s)

Total score: 100