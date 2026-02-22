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

void solve_task(int p) {
  int a, b; std::cin >> a >> b;
  
  if(p == 1) {
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    // set the coefficients of A and b
    const int X = 0; 
    const int Y = 1;
    
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  //  x + y  <= 4
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  // 4x + 2y <= a*b
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  // 4x + 2y <= a*b
    
    // objective function
    lp.set_c(X, a);                                        // a*x
    lp.set_c(Y, -b);                                       // -b*y
    
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    // output solution
    if(s.is_unbounded()) {
      std::cout << "unbounded" << "\n";
    } else if(s.is_infeasible()) {
      std::cout << "no" << "\n";
    } else {
      ET solution = - (s.objective_value_numerator() / s.objective_value_denominator());
      std::cout << std::floor(solution.to_double()) << std::endl;
    }
  } else if (p == 2) {
    // create an LP with Ax <= b, upper bound 0 and no lower bounds
    Program lp (CGAL::SMALLER, false, 0, true, 0); 
    
    // set the coefficients of A and b
    const int X = 0; 
    const int Y = 1;
    const int Z = 2;
    
    lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);
    lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b);
    lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1);
    
    // objective function
    lp.set_c(X, a);
    lp.set_c(Y, b);
    lp.set_c(Z, 1);
    
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
     // output solution
    if(s.is_unbounded()) {
      std::cout << "unbounded" << "\n";
    } else if(s.is_infeasible()) {
      std::cout << "no" << "\n";
    } else {
      ET solution = (s.objective_value_numerator() / s.objective_value_denominator());
      std::cout << (long)std::floor(solution.to_double()) << std::endl;
    }
  }
}

int main() {
  for(int p; std::cin >> p && p != 0;) {
    solve_task(p);
  }
  return 0;
}