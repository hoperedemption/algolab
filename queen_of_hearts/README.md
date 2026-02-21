# Queen of Hearts

## Algorithm tag

**Min-Cut, Max-Flow, Node-Splitting, BFS**

---

## The problem in a nutshell

We are given a directed graph `G = (V, E)` with distinguished source `s = 0` and sink `t = r + 1`.
Each intermediate vertex `i ∈ {1..r}` has:

- an integer **budget** `w_i >= 0` (the number of carrots available at that node’s opening, computed from an internal shortest-path loss model), and
- optionally a unit **coin-cost** `c_i ∈ {0,1}` indicating whether stealing from `i` requires paying that rabbit.

Alice may **delete** (steal) any amount from node `i`, up to deleting the node completely, paying either:

- **carrot objective:** total deleted carrots `sum_{i in S} w_i`, and
- **coin objective (only if grumpy day):** total deleted coin-cost `sum_{i in S} c_i`,

where deleting node `i` means the Queen cannot use carrots at `i` to traverse outgoing edges there.

The Queen can traverse the garden as long as there exists an `s -> t` directed path whose internal vertices still provide enough “continuation tokens” (carrots). Find:

1. the minimum total carrots Alice must steal to make `t` unreachable from `s`, and
2. on grumpy days, the minimum number of rabbits Alice must pay (coins) among all stealing plans that also make `t` unreachable.

---

## High-level algorithm idea

There are two layers:

### 1) Compute available carrots per rabbit

For each rabbit network, run BFS from node `0` to find the shortest number of tunnels to the opening `n_i - 1`.
Each tunnel drops one carrot, so only `max(C_i - dist, 0)` carrots can reach the garden opening.

### 2) Turn “steal carrots to block the Queen” into an `s-t` min-cut

The decision is essentially “remove” (pay for) certain rabbit openings to break all directed paths from entrance to exit.

Model each rabbit vertex `i` as a capacity-`w_i` node via **node-splitting**:

- create `i_in` and `i_out`
- add an edge `i_in -> i_out` with capacity `w_i`

Give all garden pathways **infinite** capacity, wired so that traversing through a rabbit must cross its split edge.
Then the minimum `s-t` cut equals the minimum carrots to steal.

For the coin objective, run the same min-cut again but with node capacities `c_i ∈ {0,1}` (only for rabbits with `w_i > 0`), giving the minimum number of paid rabbits among solutions that block the exit.

---

## Complexity analysis

Let:

- `r` be the number of rabbits,
- total burrow nodes `N = sum_i n_i`,
- total burrow tunnels `M_b = sum_i m_i`,
- and garden edges `m`.

- **BFS phase:** `sum_i O(n_i + m_i) = O(N + M_b)`.
- **Min-cut phase:** build a flow network with `O(r)` vertices and `O(m + r)` edges (up to constant factors for reverse edges), then run push–relabel max-flow twice:

  - **Time:** `2 * O(V'^3)`,
  - **Space:** `O(V' + E') = O(r + m)`.

Overall per test case:

- `O(N + M_b) + O(V'^3)`