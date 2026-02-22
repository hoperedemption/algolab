#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 Point;
typedef K::FT FT;

typedef std::vector<int> vi;
typedef std::tuple<K::FT, int, int> tii;
typedef std::vector<tii> vtii;

void solve_task() {
  int n, m; long s; int k;
  std::cin >> n >> m >> s >> k;

  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay dt;
  dt.insert(points.begin(), points.end());
  
  vtii events(m + std::distance(dt.finite_edges_begin(), dt.finite_edges_end()));
  
  for(int i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    Point bone(x, y);
    auto v = dt.nearest_vertex(bone);
    events[i] = {4 * CGAL::squared_distance(v->point(), bone), v->info(), -1};
  }
  
  int idx = m;
  for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) { 
    events[idx++] = {dt.segment(e).squared_length(), e->first->vertex((e->second+1)%3)->info(), e->first->vertex((e->second+2)%3)->info()};
  }
  
  std::sort(events.begin(), events.end(), [](const tii& a, const tii& b) {
      return std::get<0>(a) < std::get<0>(b);    
  });
  
  int a = 0;
  FT b = -1;
  vi bone_count(n, 0);
  boost::disjoint_sets_with_storage<> uf(n);
  for(const tii& event : events) {
    FT dist; int i, j;
    std::tie(dist, i, j) = event;
    int merge_bones = 0;
    if(j == -1) {
      int repi =  uf.find_set(i);
      bone_count[repi]++;
      merge_bones = bone_count[repi];
    } else {
      int repi = uf.find_set(i);
      int repj = uf.find_set(j);
      if(repi != repj) {
        int bone_counti = bone_count[repi];
        int bone_countj = bone_count[repj];
        uf.link(repi, repj);
        int repij = uf.find_set(repi);
        bone_count[repij] = bone_counti + bone_countj;
        merge_bones = bone_count[repij];
      }
    }
    
    if(dist <= s) {
      a = std::max(a, merge_bones);
    }
    if(b == -1 && merge_bones >= k) {
      b = dist;
    }
  }
  
  std::cout << a << " " << (long)b << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}