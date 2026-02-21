# Rapunzel

**Algorithm Tags:** Tree / Rooted Arborescence, DFS, Sliding Window on Root-to-Node Path, Balanced BST / Multiset

---

##  The Problem in a Nutshell

You are given a directed graph on vertices $V=\{0,\dots,n-1\}$ with brightness values $h:V\to\mathbb{Z}_{\ge 0}$, satisfying the following property:

For every vertex u, there is **exactly one** directed path (rope) starting at 0 and ending at u. Thus, the graph is effectively a **rooted directed tree (arborescence)** outward from root 0.

For each vertex s, let $P_s = (s = v_0 \to v_1 \to \cdots \to v_{m-1})$ be the unique directed path of length m starting at s (if it exists). Define the **contrast** of a length-m rope as:

$$\text{contrast}(P_s) = \max_{0\le i<m} h(v_i) - \min_{0\le i<m} h(v_i)$$

**Goal:** Output all vertices s such that a length-m path starting at s exists and $\text{contrast}(P_s) \le k$. If no such vertices exist, print "Abort mission".