#include <iostream> 
#include <vector>
#include <cmath>

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

void solve_task(int n) {
  int d; std::cin >> d;
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  // r >= 0
  const int r = d;
  lp.set_l(r, true, 0);   
  
  for(int i = 0; i < n; ++i) {
    long sum2 = 0;
    
    std::vector<int> ai(d);
    for(int j = 0; j < d; ++j) {
      std::cin >> ai[j];
      lp.set_a(j, i, ai[j]);
      sum2 += ai[j] * ai[j];
    }
    long normai = (long)std::sqrt(sum2);
    lp.set_a(r, i, normai);
    
    int bi; std::cin >> bi;
    lp.set_b(i, bi);
  }
  
  // maximize the radius r
  lp.set_c(r, -1); 
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  if(s.is_unbounded()) {
    std::cout << "inf" << "\n";
  } else if(s.is_infeasible()) {
    std::cout << "none" << "\n";
  } else {
    ET solution = -(s.objective_value_numerator() / s.objective_value_denominator());
    std::cout << (long)std::floor(solution.to_double()) << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  for(int n; std::cin >> n && n != 0;) {
    solve_task(n);
  }
  return 0;
}