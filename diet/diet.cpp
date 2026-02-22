#include <iostream>
#include <vector>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve_task(int n, int m) {
  std::vector<int> mini(n);
  std::vector<int> maxi(n);
  
  for(int i = 0; i < n; ++i) {
    std::cin >> mini[i] >> maxi[i];
  } 
  
  std::vector<int> p(m);
  std::vector<std::vector<int>> C(m, std::vector<int>(n));
  for(int j = 0; j < m; ++j) {
    std::cin >> p[j];
    for(int i = 0; i < n; ++i) {
      std::cin >> C[j][i];
    }
  }
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  for(int l = 0 ; l < 2*n; l += 2) {
    int i = l / 2;
    
    for(int j = 0; j < m; ++j) {
      lp.set_a(j, l, C[j][i]); 
    }
    lp.set_b(l, maxi[i]);
    for(int j = 0; j < m; ++j) {
      lp.set_a(j, l + 1, -C[j][i]);
    }
    lp.set_b(l + 1, -mini[i]);
  }
  
  for(int j = 0; j < m; ++j) {
    lp.set_c(j, p[j]);
  }
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if(s.is_infeasible()) {
    std::cout << "No such diet." << "\n";
  } else {
    ET solution = (s.objective_value_numerator() / s.objective_value_denominator());
    std::cout << (long)std::floor(solution.to_double()) << "\n";
  }
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; int m; 
  for(; std::cin >> n && std::cin >> m && n != 0 && m != 0;) {
    solve_task(n, m);
  }
  return 0;
}