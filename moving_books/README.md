# Moving Books: Greedy Scheduling

## Algorithm tag

**Greedy**

---

## Problem in a nutshell 

There are `n` carriers with strengths `s_1, ..., s_n` and `m` items with weights `w_1, ..., w_m`. In one *round*, each carrier can carry at most one item, and carrier `i` can carry an item only if `s_i >= w`. All carriers act in parallel each round. Determine the minimum number of rounds `R` needed to move all items, or report **impossible** if `max_j w_j > max_i s_i`. The required time is a linear function of `R` (here: `3R - 1`).

---

# Solution 1 — Sort + feasibility check with binary search on rounds

### High-level idea

After sorting strengths and weights in non-increasing order, the only remaining question is: *for a given number of rounds `R`, can all `m` items be assigned to carriers over `R` rounds respecting capacity and strength constraints?* This feasibility is monotone: if `R` rounds are enough, then any larger number of rounds is also enough. That monotonicity makes binary search natural.

### Feasibility test for a fixed `R`

Greedily simulate assignments in sorted order:

* Maintain a pointer `p` to the heaviest not-yet-assigned item.
* For each carrier `i` in descending strength:

  * try to assign up to `R` items to that carrier, each time checking whether the current heaviest remaining item `w_p` satisfies `w_p <= s_i`.
  * If it does, assign it and increment `p`; if it doesn’t, break (because all remaining items are `<= w_p`, but if even `w_p` is too heavy, no further item can be assigned to this carrier at this moment in the sorted order model).

If after processing all carriers `p = m`, then `R` rounds suffice.

### Output

Binary search the minimal `R in [1, m]` that passes the feasibility test, then output `3R - 1`. If the heaviest item is heavier than the strongest carrier (`w_1 > s_1`), output **impossible** immediately.

---

# Solution 2 — Greedy multiset packing round-by-round

### High-level idea

Instead of binary searching `R`, directly simulate the process: in each round, each carrier takes the heaviest remaining item it can carry. 

### How it works

* Sort strengths increasingly.
* Store all item weights in a multiset.
* If `max w > max s`, return **impossible**.

Simulate rounds using a pointer through the strengths:

* For the current carrier strength `s`, take from the multiset the largest weight `<= s` (via `upper_bound` then decrement iterator).
* If no such weight exists for that carrier, skip it for this round.
* After all carriers have been considered once, one round has elapsed; increase time by 3 (except the last offset handled at the end) and restart the carrier pointer.
* Continue until the multiset is empty.

The final output is `cnt + 2`, matching the timeline (each completed round adds 3 time units, with a final adjustment of +2).

The `s_start` optimization avoids repeatedly scanning carriers that are too weak to lift (essentially skip pointer).

---

# Why both solutions are correct (and how they relate)

### Shared greedy principle

Both methods rely on the same structural fact: after sorting, it is always safe to match the *heaviest remaining items* as early as possible to the *strongest available carriers*, because:

* if a heavy item cannot be carried by a given carrier, it cannot be carried by any weaker carrier either;
* delaying a heavy item never helps, since it only reduces future options.

---

## Complexity

### Solution 1

* Sorting strengths and weights: `O(n log n + m log m)`
* Feasibility test for a fixed `R`: `O(n + m)` (single pass with pointer)
* Binary search over `R in [1, m]`: `O(log m)` tests

**Total:** `O(n log n + m log m + (n + m) log m)` time, `O(1)` extra memory (besides arrays).

### Solution 2

* Sort strengths: `O(n log n)`
* Multiset build: `O(m log m)`
* Each item removed once with `upper_bound`: `m` operations at `O(log m)`

**Total:** `O(n log n + m log m)` time, `O(m)` memory for the multiset.