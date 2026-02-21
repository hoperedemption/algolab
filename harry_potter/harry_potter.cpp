///3
#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Edge {
  int k;
  int d;
  CGAL::Gmpq e;
};

typedef std::vector<int> vi;
typedef std::vector<Edge> ve;
typedef std::vector<ve> vve;

long ceil_to_double(const CGAL::Quotient<CGAL::Gmpq>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while(a >= x + 1) a -= 1;
  while(a < x) a += 1;
  return (long)a;
}

void solve_task() {
  int n, m, a, b, c;
  std::cin >> n  >> m >> a >> b >> c;
  
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  vi max_element(m);
  ve edges(m);
  vve in(n + 1, ve());
  vve out(n + 1, ve());
  for(int k = 0; k < m; ++k) {
    int i, j, d, e1, e2;
    std::cin >> i >> j >> d >> e1 >> e2;
    edges[k] = {k, d, CGAL::Gmpq(e1, e2)};
    max_element[k] = std::max(i, j);
    in[j].push_back(edges[k]);
    out[i].push_back(edges[k]);
    lp.set_a(k, 0, d);
  }
  lp.set_b(0, c);
  
  
  for(Edge e: out[2]) {
    lp.set_a(e.k, 1, 1);
  }
  lp.set_b(1, a);
  
  for(Edge e: in[1]) {
    lp.set_a(e.k, 2, e.e);
  }
  for(Edge e: out[1]) {
    lp.set_a(e.k, 2, -1);
  }
  lp.set_b(2, b);
  lp.set_r(2, CGAL::LARGER);
  
  for(int k = 3; k <= n; ++k) {
    for(Edge e: in[k]) {
      lp.set_a(e.k, k, e.e);
    }
    for(Edge e: out[k]) {
      lp.set_a(e.k, k, -1);
    }
    lp.set_b(k, 0);
    lp.set_r(k, CGAL::LARGER);
  }
  
  int lo = 2;
  int hi = n + 1;
  while(lo < hi) {
    int mid = lo + (hi - lo) / 2;
    for(int k = 0; k < m; ++k) {
      if(max_element[k] <= mid) {
        lp.set_u(k, false, 0);
      } else {
        lp.set_u(k, true, 0);
      }
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    if(sol.is_infeasible()) lo = mid + 1;
    else hi = mid;
  }
  
  if(lo == n + 1) {std::cout << "Busted!" << "\n"; return;}
  
  for(int k = 0; k < m; ++k) {
    if(max_element[k] <= lo) {
      lp.set_u(k, false, 0);
    } else {
      lp.set_u(k, true, 0);
    }
  }
  
  const int s = m;
  for(int k = 0; k < m; ++k) {
    lp.set_a(k, n+1+k, edges[k].d);
    lp.set_a(s, n+1+k, -1);
    lp.set_b(n+1+k, 0);
  }
  lp.set_c(s, 1);
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  std::cout << lo << " " << ceil_to_double(sol.objective_value()) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}