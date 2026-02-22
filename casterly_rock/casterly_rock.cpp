#include <iostream>
#include <vector>
#include <utility>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit --> HERE LONG)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


long ceil_to_double(const CGAL::Quotient<ET>& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a >= x + 1) a -= 1; 
    while (x > a) a += 1; 
    return (long)a;
}

void solve_task() {
  int n, m;
  long s;
  std::cin >> n >> m >> s;
  
  long sumnoblex = 0;
  long sumnobley = 0;
  long sumcommonx = 0;
  long sumcommony = 0;
  
  std::vector<std::pair<int, int>> noble(n);
  
  for(int i = 0; i < n; ++i) {
    std::cin >> noble[i].first >> noble[i].second;
    sumnoblex += noble[i].first;
    sumnobley += noble[i].second;
  }
  
  std::vector<std::pair<int, int>> common(m);
  for(int j = 0; j < m; ++j) {
    std::cin >> common[j].first >> common[j].second;
    sumcommonx += common[j].first;
    sumcommony += common[j].second;
  }
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  const int B = 0;
  const int C = 1;
  
  int count_id = 0;
  
  for(int i = 0; i < n; ++i) {
    lp.set_a(B, count_id, noble[i].second);
    lp.set_a(C, count_id, 1);
    lp.set_b(count_id, -noble[i].first);
    count_id++;
  }
  
  for(int j = 0; j < m; ++j) {
    lp.set_a(B, count_id, -common[j].second);
    lp.set_a(C, count_id, -1);
    lp.set_b(count_id, common[j].first);
    count_id++;
  }
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  assert(sol.solves_linear_program(lp));
  
  if(sol.is_infeasible()) {
    std::cout << "Y" << "\n";
    return;
  } 
  
  if(s == 0) {
    std::cout << "B" << "\n";
    return;
  }
  
  if(s != -1) {
    lp.set_a(B, count_id, sumcommony - sumnobley);
    lp.set_a(C, count_id, m - n);
    lp.set_b(count_id, s + sumnoblex - sumcommonx);
    count_id++;
    
    Solution sol2 = CGAL::solve_linear_program(lp, ET());
    assert(sol2.solves_linear_program(lp));
    
    if(sol2.is_infeasible()) {
      std::cout << "B" << "\n";
      return;
    }
  }
  
  const int D = 2;
  const int d = 3;
  lp.set_l(d, true, 0);   
  
  for(int i = 0; i < n; ++i) {
    lp.set_a(B, count_id, noble[i].first);
    lp.set_a(D, count_id, -1);
    lp.set_a(d, count_id, -1);
    lp.set_b(count_id, noble[i].second);
    count_id++;
  }
  
  for(int i = 0; i < n; ++i) {
    lp.set_a(B, count_id, -noble[i].first);
    lp.set_a(D, count_id, 1);
    lp.set_a(d, count_id, -1);
    lp.set_b(count_id, -noble[i].second);
    count_id++;
  }
  
  for(int j = 0; j < m; ++j) {
    lp.set_a(B, count_id, common[j].first);
    lp.set_a(D, count_id, -1);
    lp.set_a(d, count_id, -1);
    lp.set_b(count_id, common[j].second);
    count_id++;
  }
  
  for(int j = 0; j < m; ++j) {
    lp.set_a(B, count_id, -common[j].first);
    lp.set_a(D, count_id, 1);
    lp.set_a(d, count_id, -1);
    lp.set_b(count_id, -common[j].second);
    count_id++;
  }
  
  lp.set_c(d, 1);
  
  Solution sol3 = CGAL::solve_linear_program(lp, ET());
  assert(sol3.solves_linear_program(lp));
  
  auto solution = sol3.objective_value();
  std::cout << ceil_to_double(solution) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}