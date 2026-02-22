#include <iostream>
#include <vector>
#include <tuple>
#include <utility>

typedef std::tuple<int, int, long, long, long> ti;
typedef std::vector<ti> vti;
typedef std::vector<long> vl;

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve_task() {
  int n; std::cin >> n;
  
  vti coefs(n);
  for(int i = 0; i < n; ++i) {
    int f, c; long k, l, m;
    std::cin >> f >> c >> k >> l >> m;
    coefs[i] = {f, c, k, l, m};
  }
  
  vl a(25, 0);
  for(int i = 1; i <= 24; ++i) {
    long ai; std::cin >> ai;
    a[i] = ai + a[i - 1];
  }
  
  vl b(25, 0);
  for(int i = 1; i <= 24; ++i) {
    long bi; std::cin >> bi;
    b[i] = bi + b[i - 1];
  }
  
  int lo = 0;
  int hi = 49;
  while(lo < hi) {
    int midS = lo + (hi - lo)/2; // hA + hB
    
    bool ok = false;
    Program lp (CGAL::LARGER, true, 0, true, 1);
    const int h1 = 0;
    const int h2 = 1;
    const int h3 = 2;
    for(int hA = 0; hA <= std::min(midS, 24); ++hA) {
      int hB = midS - hA;
      if(hB < 0 || hB > 24) continue;
      
      bool prune = false;
      for(int i = 0; i < n; ++i) {
        long f, c, k, l, m;
        std::tie(f, c, k, l, m) = coefs[i];
        
        long kcoef = k + a[hA]*a[hA];
        long lcoef = l + b[hB]*b[hB];
        long mcoef = m + a[hA]*b[hB];
        long ub = std::max(0L, kcoef) + std::max(0L, lcoef) + std::max(0L, mcoef);
        if(ub < f - c) {
          prune = true; break;
        }
        lp.set_a(h1, i, kcoef);
        lp.set_a(h2, i, lcoef);
        lp.set_a(h3, i, mcoef);
        lp.set_b(i, f - c);
      }
      
      if(prune) continue;
      Solution sol = CGAL::solve_linear_program(lp, ET());
      if(!sol.is_infeasible()) {ok = true; break;}
    }
    
    if(ok) hi = midS;
    else lo = midS + 1;
  }
  
  if(lo == 49) std::cout << "Impossible!" << "\n";
  else std::cout << lo << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}