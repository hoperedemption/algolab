#include <iostream>
#include <vector>
#include <tuple>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef size_t Index;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::FT FT;
typedef K::Point_2 Point;

typedef std::tuple<Index,Index, FT> tii;
typedef std::vector<tii> vtii;

void solve_task(int n) {
  std::vector<K::Point_2> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y));
  }
  Triangulation t;
  t.insert(points.begin(), points.end());
  
  int face_idx = 0;
  for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) { f->info() = face_idx++;}
  const int num_faces = face_idx;
  const int INF = num_faces;
  
  vtii edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    int f = t.is_infinite(e->first) ? INF : e->first->info();
    int i = e->second;
    int g = t.is_infinite(e->first->neighbor(i)) ? INF : e->first->neighbor(i)->info();
    
    edges.emplace_back(f, g, t.segment(e).squared_length() / FT(4.0));
  }
  std::sort(edges.begin(), edges.end(),
      [](const tii& e1, const tii& e2) -> bool {
        return std::get<2>(e1) > std::get<2>(e2);});
  
  int m;
  std::cin >> m;
  std::vector<std::tuple<int, int, int, FT>> queries(m);
  for(int i = 0; i < m; ++i) {
    int x, y; long d;
    std::cin >> x >> y >> d;
    FT d_ft = FT(d);
    queries[i] = {i, x, y, d_ft};
  }
  
  std::vector<char> ans(m, 'n');
  std::sort(queries.begin(), queries.end(), [](const std::tuple<int, int, int, FT>& a, const std::tuple<int, int, int, FT>& b) {
    return std::get<3>(a) > std::get<3>(b);
  });
  boost::disjoint_sets_with_storage<> uf(num_faces + 1);
  int e_ptr = 0;
  for(const auto& query : queries) {
    int i, x, y; FT d;
    std::tie(i, x, y, d) = query;
    Point q(x, y);
    auto v = t.nearest_vertex(q);
    if(CGAL::squared_distance(q, v->point()) < d) {
      ans[i] = 'n';
      continue;
    }
    auto face = t.locate(q);
    if(t.is_infinite(face)) {
      ans[i] = 'y';
      continue;
    }
    while(e_ptr != edges.size() && std::get<2>(edges[e_ptr]) >= d) {
      int f, g; FT dist;
      std::tie(f, g, dist) = edges[e_ptr];
      Index c1 = uf.find_set(f);
      Index c2 = uf.find_set(g);
      if (c1 != c2) {
        uf.link(c1, c2);
      }
      e_ptr++;
    }
    
    if(uf.find_set(face->info()) == uf.find_set(INF)) {
      ans[i] = 'y';  
    } else {
      ans[i] = 'n';
    }
  }
  for(int i = 0; i < m; ++i) {std::cout << ans[i];}
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  for(int n; std::cin >> n && n != 0;) {
    solve_task(n);
  }
}