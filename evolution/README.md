# Evolution - Offline ancestor query processing on a rooted tree

### Algorithm tag

**DFS, Binary Search, Offline Queries, Backtracking** 

---

### The problem in a nutshell

Given a rooted tree `T = (V, E)` with root `r`. Each node `v in V` has an integer key `age(v)`, and along every root-to-leaf path ages are strictly decreasing (ancestors are older). For each query `(u, b)`, return the ancestor

`x in Anc(u) such that age(x) <= b`

with **minimum depth** (i.e., the oldest possible among those not older than `b`; equivalently: the first node on the path `r -> u` whose age is `<= b`). 

---

### High-level algorithm idea

Process all queries **offline** during one DFS from the root. Maintain the current root-to-node path as a vector of pairs `(v, age(v))`; because ages strictly decrease along the path, this vector is sorted by age in descending order. When you visit a node `u`, answer every query `(u, b)` by binary searching this path for the first element with `age <= b`, and output its vertex. Then continue DFS and pop the node when backtracking.

---

### Complexity analysis

Let `n = |V|`, `q =` number of queries.

* DFS traversal: `O(n)`
* Each query answered by binary search on the current path: `O(log n)`

**Total time:** `O(n + q log n)` per test case  
**Space:** `O(n + q)` for adjacency + stored queries + DFS path/recursion stack.

## Points

```text
Testing solution >>>>
   Test set 1 (30 pts / 1 s) : Correct answer      (0.01s)
   Test set 2 (70 pts / 3 s) : Correct answer      (0.477s)

Total score: 100
