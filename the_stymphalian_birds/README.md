### Algorithm tag

**Greedy, Tree, Scheduling, Sorting** 

---

### The problem in a nutshell

Let `n = 2^i - 1` and consider the complete binary tree on vertices `{0, ..., n-1}` with children `ch(u) = {2u+1, 2u+2}` when in range. Each vertex `u` has a deadline `t_u ∈ Z_{>= 1}`. We must choose a permutation `pi` of all vertices such that:

1. (**Precedence**) if `v ∈ ch(u)` then `v` appears before `u` in `pi` (children before parent).
2. (**Deadlines**) the vertex killed at time `tau ∈ {0, ..., n-1}` must satisfy `tau < t_{pi(tau)}`.

Decide if such a permutation exists. 

---

### High-level algorithm idea

First, enforce the precedence constraint by tightening deadlines: since a parent cannot be killed before its own deadline and must be killed **after** all descendants, every descendant must also be killed before the parent’s deadline. So we propagate along the tree:

`t_v <- min(t_v, t_parent(v))`

making each node’s effective deadline the minimum along its root-to-node path. After this, the precedence constraint is “baked into” the deadlines. Then it becomes a pure feasibility check for unit-time tasks: sort all nodes by increasing effective deadline and verify that at sorted position `tau`, the `tau`-th task still satisfies `t > tau`. If any task has deadline `<= tau`, no schedule can meet all deadlines; otherwise a schedule exists.

---

### Complexity analysis

* Deadline propagation over the array-represented tree: **O(n)**.
* Sorting `n` nodes by deadline: **O(n log n)**.
* Single pass feasibility check: **O(n)**.

**Total:** **O(n log n)** time per test case, **O(n)** memory.

## Points

```text
Testing solution >>>>
   Test set 1 (25 pts / 1 s) : Correct answer      (0.0s)
   Test set 2 (25 pts / 1 s) : Correct answer      (0.006s)
   Test set 3 (25 pts / 1 s) : Correct answer      (0.015s)
   Test set 4 (25 pts / 1 s) : Correct answer      (0.148s)

Total score: 100
