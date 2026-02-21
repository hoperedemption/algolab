# Rapunzel — Sliding Window on a Root-to-Node DFS Path

## Algorithm tags

**Rooted arborescence (directed tree), DFS, sliding window on a root-to-node path, balanced BST / multiset (maintain min & max).**

---

## Problem (clean math statement)

You are given a directed graph on vertices $V=\{0,\dots,n-1\}$ with brightness values
$h:V\to\mathbb{Z}_{\ge 0}$. It satisfies:

- For every vertex $u$, there is **exactly one** directed path starting at $0$ and ending at $u$.

So the graph is effectively a **rooted directed tree (arborescence)** oriented outward from the root $0$.

For each vertex $s$, let

$$
P_s = (s = v_0 \to v_1 \to \cdots \to v_{m-1})
$$

be the unique directed path of **length $m$** starting at $s$ (if it exists). Define the **contrast** of a length-$m$ rope as:

$$
\mathrm{contrast}(P_s) \;=\; \max_{0\le i<m} h(v_i) \;-\; \min_{0\le i<m} h(v_i).
$$

**Goal.** Output all vertices $s$ such that a length-$m$ path starting at $s$ exists and

$$
\mathrm{contrast}(P_s)\le k.
$$

If none exist, print **`Abort mission`**.

---

## Key observation

Because there is **exactly one** path from $0$ to any node, a DFS from $0$ always maintains a single current root-to-current-node path:

$$
\text{path} = (0 \to \cdots \to u).
$$

Every possible rope of length $m$ is just a **contiguous block of $m$ consecutive vertices** somewhere inside that current DFS path.

So the entire problem becomes:

> While walking down the tree, keep a **sliding window** of the last $m$ vertices on the current DFS path, and quickly test whether  
> $$
> \max(\text{window}) - \min(\text{window}) \le k.
> $$
> If yes, mark the **start vertex** of that window as a valid answer.

---

## Algorithm (what the code is doing)

Maintain during DFS:

- `path`: the current vertices on the DFS stack (root-to-current).
- `wdw`: a multiset / balanced BST containing brightness values of the **last $m$** vertices of `path`
  (or fewer if depth $< m$).

At node $u$:

1. Push $u$ into `path` and insert $h[u]$ into `wdw`.
2. If `wdw.size() > m`, remove the brightness of the vertex that just fell out of the last-$m$ suffix:
   - That vertex is `path[path.size() - 1 - m]`.
3. If `wdw.size() == m`, compute:
   - $\min = *\text{wdw.begin()}$
   - $\max = *\text{wdw.rbegin()}$
   - If $\max-\min \le k$, then the **start of the window** is `path[path.size() - m]`. Mark it.
4. Recurse to children.
5. Backtrack: undo the insertions/deletions so the parent call sees the correct window.

This is exactly the **sliding window over a DFS path** idea.

---

## Why it’s correct (short but solid)

### Invariant

At every DFS call at node $u$:

1. `path` equals the unique directed path $(0 \to \cdots \to u)$.
2. `wdw` contains exactly the multiset of brightness values of the last $\min(m, |path|)$ vertices of `path`.

This holds because:

- You insert $h[u]$ on entry.
- If the size exceeds $m$, you delete exactly the element corresponding to the vertex now $m+1$ steps behind the end.
- On return, you undo exactly those changes.

### Correct marking

When `wdw.size() == m`, it corresponds to the unique length-$m$ rope ending at the current node $u$, i.e. the suffix:

$$
(v_{|path|-m} \to \cdots \to v_{|path|-1}=u).
$$

Its start is `path[path.size() - m]`. If its contrast is $\le k$, you mark precisely that start vertex—exactly what the problem asks for.

### Completeness

Every length-$m$ directed rope appears as the last $m$ vertices of `path` at the moment DFS visits its endpoint, so it will be checked.

---

## Complexity

Let $n$ be the number of nodes.

- Each node causes $O(1)$ multiset inserts/erases.
- Each multiset operation costs $O(\log m)$ (the window never exceeds $m$).

So per test case:

- **Time:** $O(n\log m)$
- **Extra memory:** $O(m)$ for the window/path suffix (plus adjacency storage)