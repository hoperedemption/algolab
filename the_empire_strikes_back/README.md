# Empire Strikes Back - Linear Programming Feasibility with Geometric Radius Constraints (Delaunay Nearest Neighbor + LP)

### Algorithm tag

**LP, Delaunay, Nearest-Neighbor** 

---

### The problem in a nutshell

Given:

* asteroid points `A_i in R^2` with required densities `D_i > 0` for `i = 1..a`,
* shot centers `S_j in R^2` for `j = 1..s`,
* bounty hunter points `B ⊂ R^2`,
* total available energy `E >= 0`.

Each shot `j` chooses an energy `e_j >= 0` and a radius `r_j`, but to avoid detection it must satisfy

`r_j^2 <= min_{b in B} |S_j - b|^2  (and r_j^2 = +infty if B = empty).`

Asteroid `i` receives total energy

`sum over j with |A_i - S_j|^2 <= r_j^2 of  e_j / max(1, |A_i - S_j|^2).`

Decide whether there exist energies `e_1, ..., e_s` such that:

`for all i:  sum over j with |A_i - S_j|^2 <= r_j^2 of  e_j / max(1, |A_i - S_j|^2) >= D_i,`

and

`sum_{j=1..s} e_j <= E.`

---

### High-level algorithm idea

First compute, for each shot center `S_j`, the largest safe explosion radius by finding its nearest bounty hunter (if any); this is a standard nearest-neighbor query, done efficiently via a Delaunay triangulation on the bounty hunter points. With these radii fixed, the remaining decision is only the energies `e_j >= 0`. The asteroid-destruction conditions and the total-energy budget are linear inequalities in the variables `e_j`, so the question becomes pure **LP feasibility**. Build the LP with one constraint per asteroid plus the energy-budget constraint, and ask the LP solver if it’s feasible.

---

### Complexity analysis

Let `a <= 30`, `s <= 10^4`, `b <= 10^5`.

* Build Delaunay triangulation on bounty hunters: `O(b log b)`.
* Nearest bounty hunter for each shot: `O(s log b)` (amortized).
* Build LP coefficients by checking asteroid–shot distances: `O(a * s)` distance tests (here at most `3 * 10^5`).
* Solve LP: polynomial in `(s, a)` for fixed-precision arithmetic; in our case is O(max(s, a)).
Hence total runtime is `O(blogb + slogb + a*s + max(s, a))`

**Space:** `O(b + s + a * s)` in the straightforward build (effectively dominated by storing the nonzero LP coefficients you add).

## Points

```text
Testing solution >>>>
   Test set 1        (30 pts / 3 s) : Correct answer      (0.873s)
   Test set 2        (25 pts / 3 s) : Correct answer      (0.923s)
   Test set 3        (25 pts / 3 s) : Correct answer      (1.452s)
   Hidden test set 1 (10 pts / 3 s) : Correct answer      (0.873s)
   Hidden test set 2 (05 pts / 3 s) : Correct answer      (0.923s)
   Hidden test set 3 (05 pts / 3 s) : Correct answer      (1.452s)

Total score: 100