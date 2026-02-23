# Coin Tossing Tournament — Game Feasibility via Max Flow

## Algorithm tag

**Max Flow**

## The problem in a nutshell

There are `n` players and `m` played matches. Some match outcomes are fixed (winner known), others are undecided. Each match contributes exactly 1 point to exactly one of its two endpoints. Given target final scores `s_0, ..., s_(n-1)`, decide whether there exists an assignment of winners for the undecided matches such that every player `i` ends with exactly `s_i` total wins.

Equivalently: let `w_i` be the number of already-confirmed wins of player `i`. For each undecided match `k` between players `u_k, v_k`, choose `x_k in {u_k, v_k}` (the winner). The question is whether there exist choices such that, for all `i`,

`w_i + |{ k : x_k = i }| = s_i,`

and also `sum_i s_i = m` (since each match creates exactly one win). 

## High-level idea of the algorithm

I first account for all matches with known winners and compute each player’s already-secured score `w_i`. This immediately yields each player’s remaining required wins

`r_i := s_i - w_i.`

If any `r_i < 0`, or if `sum_i s_i != m`, the target scoreboard is impossible.

For the remaining undecided matches, I build a flow network that distributes “one unit of win” per undecided match to exactly one of its two players:

* Each undecided match is a node that receives 1 unit from the source (or more generally, its multiplicity if the same pair appears multiple times undecided).
* From a match node I connect to the two corresponding player nodes, allowing that win-unit to be assigned to either endpoint.
* Each player node sends flow to the sink with capacity `r_i`, limiting how many undecided wins that player may still receive.

If the maximum flow equals the number of undecided matches, then all undecided wins can be assigned while respecting every player’s required remainder `r_i`, so the answer is **yes**; otherwise **no**. 

## Complexity analysis

Let `U` be the number of undecided matches (counting multiplicities), and let `P <= binom(n, 2)` be the number of distinct player-pairs that appear as undecided in the aggregated `game_pairs` matrix.

* Vertices: `V = 2 + n + P` in the conceptual model (source, sink, `n` player nodes, `P` match-pair nodes).
* Edges: `E = P` (source→match) `+ 2P` (match→players) `+ n` (players→sink), i.e. `E = 3P + n` (each with a reverse edge internally in the flow implementation).

Using push–relabel, the **worst-case** running time is `O(V^3)`

## Points

```text
Testing solution >>>>
   Test set 1 (40 pts / 1 s) : Correct answer      (0.007s)
   Test set 2 (40 pts / 1 s) : Correct answer      (0.338s)
   Test set 3 (20 pts / 1 s) : Correct answer      (0.817s)

Total score: 100