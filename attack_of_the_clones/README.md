# Attack of the Clones: Maximum Set of Non-Overlapping Circular Intervals

## Algorithm tag

**Greedy + Interval Scheduling**

---

## Problem in a nutshell

There are `n` circular intervals on `{1, ..., m}`. Interval `I(a, b)` is

`I(a, b) = {a, a+1, ..., b}` if `a <= b`,  
`I(a, b) = {a, a+1, ..., m, 1, ..., b}` if `a > b`.

Compute

`max { |S| : S ⊆ {1, ..., n},  I(a_i, b_i) ∩ I(a_j, b_j) = empty  for all i != j }.`

It is guaranteed that there exists a segment `s` contained in at most 10 intervals. 

---

## Algorithm idea

The circle is cut at a carefully chosen segment `s` with minimum “coverage count”, so that only few intervals wrap around the cut. After cutting, every interval becomes an interval on a line `[1, m]`, and the maximum number of pairwise disjoint intervals on a line is solved by the classic greedy strategy: sort by right endpoint and take the next interval that starts after the current end.

The wrap-around intervals are the only complication: if a chosen set contains a wrap interval, then all other chosen intervals must lie completely inside the complementary arc. Since at most 10 intervals cover the best cut point, there are only few wrap candidates to try; for each candidate, the remaining problem reduces again to standard greedy on the line.

---

## How my code implements it

### 1) Find a good cut point

A difference-array sweep counts how many circular intervals cover each segment:

* add `(+1)` at `a`,
* add `(-1)` at `b+1` (if `b < m`) for non-wrapping part,
* if `a > b`, also add `(+1)` at 1 (because the interval covers the start of the linearized circle).

Sweeping these events yields the coverage count per segment; the segment with minimum count is chosen as `start_seg`. By the guarantee, this minimum is `<= 10`. 

### 2) Rotate all intervals so the cut becomes position 1

Each endpoint is shifted by `start_seg` modulo `m`. After rotation:

* intervals with `a <= b` become “proper” linear intervals,
* intervals with `a > b` become “wrap” intervals (they cross the cut in the rotated representation).

### 3) Solve the linear case (no wrap chosen)

Sort all **proper** intervals by increasing end `b`. Greedily count non-overlapping intervals by selecting an interval whenever its start `a` is greater than the end of the last chosen one. This gives a baseline answer.

### 4) Try solutions that include one wrap interval

If the solution includes a wrap interval `(A, B)` (meaning it covers `[A, m] ∪ [1, B]`), then every other chosen interval must lie entirely in the “gap” `(B, A)`. In rotated coordinates, a proper interval `(a, b)` fits in that gap iff:

`a > B  and  b < A.`

For each wrap interval, run the same greedy on the filtered proper intervals satisfying the condition, and add 1 for the wrap interval itself. Take the maximum over:

* baseline (no wrap),
* each wrap-based count.

---

## Complexity

Let `n` be the number of intervals.

* Building sweep events and sorting them: `O(n log n)`.
* Rotating and splitting into proper/wrap: `O(n)`.
* Sorting proper intervals by end: `O(n log n)`.
* Greedy scan of proper intervals: `O(n)`.
* Trying wrap candidates: at most the number of intervals covering the chosen cut, which is `<= 10` by construction, and each trial scans the proper list once: `O(10 * n) = O(n)`.

**Total per test case:** `O(n log n)` time, `O(n)` memory.

## Points

```text
Testing solution >>>>
   Test set 1 (30 pts / 1 s) : Correct answer      (0.005s)
   Test set 2 (30 pts / 1 s) : Correct answer      (0.403s)
   Test set 3 (40 pts / 1 s) : Correct answer      (0.37s)

Total score: 100