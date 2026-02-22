#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef size_t Index;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::FT FT;
typedef K::Point_2 Point;

void solve_task() {
  int n, m; long r;
  std::cin >> n >> m >> r;
  std::vector<Point> pts;
  pts.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    long x, y;
    std::cin >> x >> y;
    pts.push_back(Point(x, y));
  }
  Triangulation dt;
  dt.insert(pts.begin(), pts.end());
  
  int num_faces = std::distance(dt.finite_faces_begin(), dt.finite_faces_end());
  const int INF = num_faces;
  std::vector<FT> clearance(num_faces + 1);
  int face = 0;
  for(auto f = dt.finite_faces_begin(); f != dt.finite_faces_end(); ++f) {
    auto cc = dt.circumcenter(f);
    f->info() = face++;
    clearance[f->info()] = CGAL::squared_distance(cc, f->vertex(0)->point());
  } 
  clearance[INF] = FT(std::numeric_limits<double>::infinity());

  std::vector<std::tuple<int, int, FT>> edges;
  edges.reserve(std::distance(dt.finite_edges_begin(), dt.finite_edges_end()));
  for(auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) {
    auto f1 = e->first;
    auto f2 = e->first->neighbor(e->second);
    
    int F = dt.is_infinite(f1) ? INF : f1->info();
    int G = dt.is_infinite(f2) ? INF : f2->info();
    
    FT dist = dt.segment(e).squared_length() / FT(4.0);
    edges.push_back({F, G, dist});
  }
  std::sort(edges.begin(), edges.end(), [](const auto a, const auto b) {
    return std::get<2>(a) > std::get<2>(b);
  });
  std::vector<std::tuple<int, Point, FT>> queries(m);
  for(int i = 0; i < m; ++i) {
    long x, y, s;
    std::cin >> x >> y >> s;
    Point query(x, y);
    queries[i] = {i, query, FT(r + s) * FT(r + s)};
  }
  std::sort(queries.begin(), queries.end(), [](const auto a, const auto b) {
    return std::get<2>(a) > std::get<2>(b);
  });
  
  int eptr = 0;
  boost::disjoint_sets_with_storage<> uf(num_faces + 1);
  std::vector<char> ans(m, 'n');
  for(int i = 0; i < m; ++i) {
    int idx; Point query; FT radius2;
    std::tie(idx, query, radius2) = queries[i];
    
    auto v = dt.nearest_vertex(query);
    if(CGAL::squared_distance(v->point(), query) < radius2) {ans[idx] = 'n'; continue;}
    
    while(eptr < edges.size() && std::get<2>(edges[eptr]) >= radius2) {
      int f1, f2; FT edgeradius;
      std::tie(f1, f2, edgeradius) = edges[eptr];
      Index c1 = uf.find_set(f1);
      Index c2 = uf.find_set(f2);
      if(c1 != c2) {
        uf.link(c1, c2);
        int root = uf.find_set(c1);
        clearance[root] = std::max(clearance[c1], clearance[c2]);
      }
      eptr++;
    }
    
    auto f = dt.locate(query);
    if(dt.is_infinite(f)) {ans[idx] = 'y'; continue;}
    
    int fidx = f->info();
    int repidx = uf.find_set(fidx);
    if(uf.find_set(fidx) == uf.find_set(INF) || clearance[repidx] >= FT(4.0) * radius2) ans[idx] = 'y';
    else ans[idx] = 'n';
  }
  
  for(int i = 0; i < m; ++i) std::cout << ans[i];
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}