///1
#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 Point;

void solve_task() {
  int n, m; 
  std::cin >> n >> m;
  long q; 
  std::cin >> q;
  int k;
  std::cin >> k;
  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(n - 1);
  for (Index i = 0; i < n - 1; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  Delaunay dt;
  dt.insert(points.begin(), points.end());
  
  int tgtx, tgty;
  std::cin >> tgtx >> tgty;
  Point targetpoint(tgtx, tgty);
  const int targetidx = n - 1;
  std::queue<IPoint> bfsq;
  std::vector<int> distance(n, -1);
  distance[targetidx] = 0;
  bfsq.push({targetpoint, targetidx});
  std::vector<Point> frontier;
  frontier.push_back(targetpoint);
  
  while(!bfsq.empty()) {
    IPoint pointpair = bfsq.front();
    bfsq.pop();
    Point cur; int u;
    std::tie(cur, u) = pointpair;
    
    if(distance[u] >= k - 1) continue;
    
    while(dt.number_of_vertices() > 0) {
      auto closest = dt.nearest_vertex(cur);
      if(CGAL::squared_distance(closest->point(), cur) > q) break;
      int v = closest->info();
      distance[v] = distance[u] + 1;
      bfsq.push({closest->point(), v});
      frontier.push_back(closest->point());
      dt.remove(closest);
    }
  }
  
  Delaunay dtfrontier;
  dtfrontier.insert(frontier.begin(), frontier.end());
  
  for(int i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    Point query(x, y);
    auto v = dtfrontier.nearest_vertex(query);
    if(CGAL::squared_distance(v->point(), query) <= q) std::cout << "y";
    else std::cout << "n";
  }
  
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}