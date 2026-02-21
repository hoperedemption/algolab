# Rapunzel

## Algorithm tags
**Backtracking · DFS · Sliding Window · Multiset**

---

## Problem in a nutshell

We are given a **rooted directed tree** `T = (V, E)` with root `0`.  
For every node `u ∈ V`, there is exactly one directed path from `0` to `u`.

Each vertex `i` has a **brightness** value `h[i] ≥ 0`.

Given integers:

- `m ≥ 2` (required rope length in vertices)
- `k ≥ 0` (maximum allowed contrast)

Find all vertices `s ∈ V` such that there exists a directed path (a “rope”)

- `s = v0 → v1 → … → v(m-1)`

containing **exactly `m` vertices**, where the **contrast** along that rope is:

- `max(h[vj]) - min(h[vj]) ≤ k` for `j = 0..m-1`

Output all such starting vertices `s` in increasing order, or print **"Abort mission"** if none exist.

---

## My Solution Idea

Run a DFS from the root while maintaining the current **root-to-current-node path**.

Along that path, maintain a **sliding window of the last `m` vertices** (once the path is at least `m` long).  
For this window, we need to quickly know:

- the minimum brightness in the window
- the maximum brightness in the window

To do that efficiently, keep a **multiset (balanced BST)** of the `h[]` values currently in the window.

### What happens during DFS?

We enter a node `u`:

1. Append `u` to the current path.
2. Insert `h[u]` into the multiset (conceptually, into the window we carry around).
3. If the path length is now greater than `m`, remove the brightness of the element that fell out of the window (the node `m` steps behind the end).
4. If the window size is exactly `m`:
   - Let `minVal = smallest element in multiset`
   - Let `maxVal = largest element in multiset`
   - If `maxVal - minVal <= k`, then the **start of the window** is a valid starting vertex:
     - that start node is the ancestor `m-1` steps above `u` on the current path

When we leave a node `u` (backtracking):

- Undo exactly what we changed when we entered:
  - remove `h[u]` from the multiset/window
  - if we previously removed an old element because the path exceeded `m`, we MUST re-insert it
  - pop `u` from the path

This ensures the things we carry around: path/window/multiset always reflect the current DFS state.

---

## Complexity

Let `n = |V|`.

- Each node is inserted into / removed from the multiset at most twice.
- Each multiset operation costs `O(log m)` because the window never exceeds size `m`.

So:

- **Time:** `O(n log m)` per test case  
- **Space:** `O(n + m)` for the tree + DFS path + window multiset + recursion stack  
