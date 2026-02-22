///3
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 Point;
typedef K::FT FT;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits< graph > traits;
typename traits::vertex_iterator vertex_iter, vertex_end;

typedef std::vector<char> vc;

void solve_task() {
  int n, m, r;
  std::cin >> n >> m >> r;
  FT r2 = FT(r)*FT(r);
  
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
  
  graph G(n);
  for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) {
    if(dt.segment(e).squared_length() <= r2) {
      boost::add_edge(e->first->vertex((e->second+1)%3)->info(), e->first->vertex((e->second+2)%3)->info(), G);
    }
  }
  
  std::vector<int> component_map(n);  // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
  int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
  
  vc ans(m, 'n');
  vc neg_ans(m, 'n');
  for(int i = 0; i < m; ++i) {
    int xa, ya;
    std::cin >> xa >> ya;
    int xb, yb;
    std::cin >> xb >> yb;
    Point a(xa, ya);
    Point b(xb, yb);
    if(CGAL::squared_distance(a, b) <= r2) {ans[i] = 'y'; continue;}
    auto va = dt.nearest_vertex(a);
    if(CGAL::squared_distance(a, va->point()) > r2) {ans[i] = 'n'; continue;}
    auto vb = dt.nearest_vertex(b);
    if(CGAL::squared_distance(b, vb->point()) > r2) {ans[i] = 'n'; continue;}
    if(component_map[va->info()] == component_map[vb->info()]) {ans[i] = 'y';}
  }
  
  typedef std::vector< boost::default_color_type > partition_t;
  typedef
      typename boost::property_map< graph, boost::vertex_index_t >::type index_map_t;
  typedef boost::iterator_property_map< partition_t::iterator, index_map_t >
      partition_map_t;

  partition_t partition(boost::num_vertices(G));
  partition_map_t partition_map(partition.begin(), boost::get(boost::vertex_index, G));
  
  bool bipartite = boost::is_bipartite(G, boost::get(boost::vertex_index, G), partition_map);
  if(!bipartite) {
    for(int i = 0; i < m; ++i) {
      std::cout << neg_ans[i];
    } std::cout << "\n";
    return;
  }
  
  std::vector<Point> white;
  std::vector<Point> black;
  white.reserve(n);
  black.reserve(n);
  for (boost::tie(vertex_iter, vertex_end) = boost::vertices(G); vertex_iter != vertex_end; ++vertex_iter)
  {
      if(get(partition_map, *vertex_iter) == boost::color_traits<boost::default_color_type>::white()) white.push_back(points[*vertex_iter].first);
      else black.push_back(points[*vertex_iter].first);
  }
  
  Delaunay white_dt;
  white_dt.insert(white.begin(), white.end());
  
  for(auto e = white_dt.finite_edges_begin(); e != white_dt.finite_edges_end(); ++e) {
    if(dt.segment(e).squared_length() <= r2) {
      bipartite = false;
      break;
    }
  }
  
  if(!bipartite) {
    for(int i = 0; i < m; ++i) {
      std::cout << neg_ans[i];
    } std::cout << "\n";
    return;
  }
  
  Delaunay black_dt;
  black_dt.insert(black.begin(), black.end());
  
  for(auto e = black_dt.finite_edges_begin(); e != black_dt.finite_edges_end(); ++e) {
    if(dt.segment(e).squared_length() <= r2) {
      bipartite = false;
      break;
    }
  }
  
  if(!bipartite) {
    for(int i = 0; i < m; ++i) {
      std::cout << neg_ans[i];
    } std::cout << "\n";
    return;
  }
  
  for(int i = 0; i < m; ++i) {std::cout << ans[i];}
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}