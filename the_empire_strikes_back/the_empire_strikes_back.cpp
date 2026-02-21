#include <iostream>
#include <vector>
#include <tuple>
#include <limits>
#include <utility>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 Point;
typedef K::FT FT;

typedef std::pair<Point, int> t;
typedef std::vector<t> vt;

void solve_task() {
  int a, s, b;
  std::cin >> a >> s >> b;
  int e;
  std::cin >> e;
  vt asteroids(a);
  for(int i = 0; i < a; ++i) {
    int x, y, d;
    std::cin >> x >> y >> d;
    asteroids[i] = {Point(x, y), d};
  }
  std::vector<Point> shot(s);
  for(int i = 0; i < s; ++i) {
    int x, y;
    std::cin >> x >> y;
    shot[i] = Point(x, y);
  }
  std::vector<Point> bounty(b);
  for(int i = 0; i < b; ++i) {
    int x, y;
    std::cin >> x >> y;
    bounty[i] = Point(x, y);
  }
  Triangulation dt;
  dt.insert(bounty.begin(), bounty.end());
  std::vector<FT> radii(s, std::numeric_limits<FT>::infinity());
  if(b != 0) {
    for(int i = 0; i < s; ++i) {
      auto v = dt.nearest_vertex(shot[i]);
      radii[i] = CGAL::squared_distance(shot[i], v->point());
    }
  }
  Program lp (CGAL::LARGER, true, 0, false, 0); 
  for(int i = 0; i < a; ++i) {
    for(int j = 0; j < s; ++j) {
      FT dist2 = CGAL::squared_distance(asteroids[i].first, shot[j]);
      if(dist2 <= radii[j]) {
        CGAL::Gmpq coef(1, std::max(FT(1), dist2));
        lp.set_a(j, i, coef);
      }
    }
    lp.set_b(i, asteroids[i].second);
  }
  for(int j = 0; j < s; ++j) {
    lp.set_a(j, a, 1);
  }
  lp.set_b(a, e);
  lp.set_r(a, CGAL::SMALLER);
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  if(sol.is_infeasible()) std::cout << "n" << "\n";
  else std::cout << "y" << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}