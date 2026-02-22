#include <vector>
#include <iostream>
#include <utility>
#include <tuple>
#include <algorithm>
#include <stack>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;

struct biker {
  long y0;
  Point origin;
  Ray ray;
};

void solve_task() {
  int n;
  std::cin >> n;
  std::vector<std::pair<biker, int>> bikers(n);
  for(int i = 0; i < n; ++i) {
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;
    bikers[i] = {{y0, Point(0, y0), Ray(Point(0, y0), Point(x1, y1))}, i};
  }
  std::sort(bikers.begin(), bikers.end(), [](const auto a, const auto b) {
    return a.first.y0 < b.first.y0;
  });
  std::stack<std::pair<biker, int>> st;
  st.push(bikers[0]);
  
  for(int i = 1; i < n; ++i) {
    biker cur; int curidx;
    std::tie(cur, curidx) = bikers[i];
    
    bool dead = false;
    
    while(!st.empty()) {
      biker top; int topidx;
      std::tie(top, topidx) = st.top();
      
      if(!CGAL::do_intersect(cur.ray, top.ray)) break;
      
      auto o = CGAL::intersection(cur.ray,top.ray);
      Point inter = *boost::get<Point>(&*o);
      
      if(CGAL::squared_distance(top.origin, inter) <= CGAL::squared_distance(cur.origin, inter)) {
        dead = true; break;
      } else {
        st.pop();
      }
    }
    
    if(dead) continue;
    
    st.push({cur, curidx});
  }
  
  std::vector<int> rideforever;
  while(!st.empty()) {
    int idx = st.top().second;
    st.pop();
    rideforever.push_back(idx);
  }
  std::sort(rideforever.begin(), rideforever.end());
  
  for(const int idx: rideforever) std::cout << idx << " ";
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}