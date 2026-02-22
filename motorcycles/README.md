# Motorcycles: Stack-Based Elimination with Non Intersecting Rays in the Plane  

## Algorithm tag

**Greedy + Monotone Stack + Geometry**

---

## Problem in a nutshell

There are `n` rays `R_i` in `R^2`. Ray `R_i` starts at `o_i = (0, y_i)` on the `y`-axis and goes into the half-plane `x > 0` in the direction of a point `(x_i, y'_i)` with `x_i > 0`. All particles move along their rays with the same speed starting at time `0`.

Particle `i` **stops** at the first point `p` on its ray that lies on some other ray `R_j` and is reached by `j` **strictly earlier** than `i`. If two particles reach the same point at the same time, the one coming from the right continues and the one coming from the left stops. The task is to output all indices `i` that never stop (i.e., whose ray is never “hit earlier” by another ray). 

---

## Algorithm idea

Sort rays by their start height `y_i`. Then scan from bottom to top while maintaining a stack of candidate “survivor” rays. When processing a new ray `R`, only the current top of the stack can be its first relevant conflict: if `R` intersects the top ray `T`, compare which one reaches the intersection point first (equivalently compare squared distances from their origins to the intersection because all speeds are equal). If `T` reaches the intersection earlier (or ties break against `R`), then `R` is eliminated and is not pushed. Otherwise `R` eliminates `T`, pop `T` and repeat against the next stack element. If `R` does not intersect the stack top, it cannot intersect any deeper ray “in a way that matters first”, so it is safe to push. After the scan, the stack contains exactly the rays that ride forever; output their original indices sorted.

---

## Complexity

Let `n` be the number of rays.

* Sorting by `y_0`: `O(n log n)`.
* Each ray is pushed once and popped at most once: `O(n)` stack operations total.
* Each intersection test/compute is `O(1)` with exact predicates/constructions.

**Total:** `O(n log n)` time and `O(n)` memory per test case.

## Points

```text
Testing solution >>>>
   Test set 1        (20 pts / 4 s) : Correct answer      (0.089s)
   Hidden test set 1 (05 pts / 4 s) : Correct answer      (0.089s)
   Test set 2        (20 pts / 4 s) : Correct answer      (0.121s)
   Hidden test set 2 (05 pts / 4 s) : Correct answer      (0.121s)
   Test set 3        (20 pts / 4 s) : Correct answer      (0.678s)
   Hidden test set 3 (05 pts / 4 s) : Correct answer      (0.678s)
   Test set 4        (20 pts / 4 s) : Correct answer      (2.413s)
   Hidden test set 4 (05 pts / 4 s) : Correct answer      (2.413s)

Total score: 100