# Asterix and the Chariot Race - Minimum-Weight Dominating Set on a Tree 

### Algorithm tag 

**Tree DP, Dominating Set, DFS** 

---

### The problem in a nutshell

We are given a rooted tree `T = (V, E)` with root `0`. Each node `v in V` has a nonnegative cost `c_v`. Choose a set `S ⊆ V` minimizing `sum_{v in S} c_v` such that every node is **dominated**:

For all `u in V`:  
`u in S` **or** there exists `v in S` with `{u, v} in E`.

Output the minimum total cost. 

---

### High-level algorithm idea

Run one DFS from the root and compute a 3-state DP per node that captures whether the node is saved, or if not, where its “saving neighbor” must come from (parent vs. children). This turns the global “every node must be saved by itself or a neighbor” constraint into local constraints that combine cleanly over subtrees. The only subtle case is when a node is not repaired and also cannot rely on its parent; then at least one child must be repaired, enforced by a small “force-one-child” correction.

---

### Recurrences (a bit more detail)

Let children of `u` be `C(u)`. Define:

* `dp[u][1]`: min cost in subtree of `u` **if `u` is repaired** (`u in S`).
* `dp[u][0]`: min cost in subtree of `u` **if `u` is not repaired but is already saved by its parent** (so parent `in S`).
* `dp[u][2]`: min cost in subtree of `u` **if `u` is not repaired and not saved by its parent**, so it must be saved by **at least one repaired child**.

Then:

1. **`u` repaired**

   `dp[u][1] = c_u + sum_{v in C(u)} min(dp[v][0], dp[v][1]).`

   Because if `u` is repaired, each child `v` is automatically saved by its parent, so `v` can either be unrepaired-and-saved-by-parent (state 0) or repaired (state 1).

2. **`u` not repaired, saved by parent**

   `dp[u][0] = sum_{v in C(u)} min(dp[v][1], dp[v][2]).`

   Because `u` is not repaired, children cannot rely on `u` to be saved; each child `v` must be either repaired (1) or saved by its own children (2).

3. **`u` not repaired, not saved by parent ⇒ must be saved by a child**

   Start with the same baseline as state 0, then force at least one child to be repaired:

   `base = sum_{v in C(u)} min(dp[v][1], dp[v][2])`

   `extra = min_{w in C(u)} ( dp[w][1] - min(dp[w][1], dp[w][2]) )`

   `dp[u][2] = base + extra.`

   The “extra” is the minimal additional cost to flip one child `w` from its cheapest option (`min(dp[w][1], dp[w][2])`) to the specifically-required “repaired” option (`dp[w][1]`), guaranteeing `u` is dominated by a child. Write the general quadratic complexity formula first and then you can easily arrive to this linear complexity formula.

Finally, the root has no parent, so it cannot be in state 0:

`answer = min(dp[0][1], dp[0][2]).`

---

### Complexity analysis

Each node is processed once and each edge contributes `O(1)` work to the DP transitions.

* **Time:** `O(n)` per test case
* **Space:** `O(n)` (adjacency + DP; recursion stack `O(n)` worst-case)  

## Points

```text
Testing solution >>>>
   Test set 1 (25 pts / 3 s) : Correct answer      (0.1s)
   Test set 2 (25 pts / 3 s) : Correct answer      (0.107s)
   Test set 3 (25 pts / 3 s) : Correct answer      (0.107s)
   Test set 4 (25 pts / 3 s) : Correct answer      (1.622s)

Total score: 100