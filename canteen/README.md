# Canteen: Min-Cost Max-Flow on a time-expanded network 

## Algorithm tag

**Min-Cost Flow**

---

## Problem in a nutshell (math, no story)

We have `n` time steps (days). On each day `i`:

* Production: up to `a_i` units can be created at unit cost `c_i`.
* Demand: up to `s_i` units can be sold at unit price `p_i`.
* Storage from day `i` to `i+1`: at most `v_i` units can be carried over, paying unit cost `e_i`.

Choose nonnegative flows `x_i` (produced), `y_i` (sold), and carry `z_i` (stored overnight) satisfying:

`0 <= x_i <= a_i,   0 <= y_i <= s_i,   0 <= z_i <= v_i,`

and inventory conservation:

`inv_{i+1} = inv_i + x_i - y_i - z_i,   inv_{i+1} = z_i (same quantity carried),   inv_1 = 0,`

(i.e., units can either be sold that day or carried to later days). Objective is:

1. **maximize** total served students `sum_i y_i`,
2. subject to that, **maximize** profit `sum_i (p_i * y_i) - sum_i (c_i * x_i) - sum_i (e_i * z_i)`.
   Output “possible” iff `sum_i y_i = sum_i s_i`, plus the achieved served count and profit. 

---

## Algorithm logic (high level)

You encode the whole planning horizon as a **flow network** with one node per day. Sending 1 unit of flow corresponds to producing one menu and routing it forward in time (possibly through storage edges) until it is finally “consumed” by selling it on some day.

To get lexicographic optimization (serve as many as possible first, then maximize profit), you build costs so that:

* producing on day `i` costs `c_i`,
* storing overnight costs `e_i` per unit,
* selling to day `i` gives revenue `p_i`, implemented as **negative cost**, but shifted to keep all edge weights nonnegative (your `20 - p_i` trick).

Then you run **min-cost max-flow** using **successive shortest augmenting paths**: repeatedly find the cheapest way to push one more unit from source to sink until no more flow can be sent. The resulting max flow equals the maximum number of students that can be served; among those, the min-cost solution corresponds to maximum profit. Finally you convert back from shifted costs to real profit with `20*flow - find_flow_cost(G)`.

---

## Complexity

Let `V = n + 2` (days plus source/sink) and `E = O(n)` (production edges, selling edges, and `n-1` storage edges).

If `F` is the max flow value (here `F <= sum_i a_i` and also `F <= sum_i s_i`, with per-day bounds `<= 300`), SSAP with Dijkstra on nonnegative reduced costs runs in:

`O(F * E * log V) = O(F * n * log n).`

Memory: `O(V + E) = O(n)`.

## Points

```text
Testing solution >>>>
   Test set 1 (30 pts / 2 s) : Correct answer      (1.277s)
   Test set 2 (50 pts / 2 s) : Correct answer      (0.153s)
   Test set 3 (20 pts / 2 s) : Correct answer      (1.616s)

Total score: 100