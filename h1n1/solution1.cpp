///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef std::size_t Index;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 Point;
typedef K::FT FT;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, FT> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void solve_task(int n) {
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  Triangulation dt;
  dt.insert(pts.begin(), pts.end());
  
  const int num_faces = std::distance(dt.finite_faces_begin(), dt.finite_faces_end());
  std::vector<FT> face_radius(num_faces);
  
  int face_idx = 0;
  for (auto f = dt.finite_faces_begin(); f != dt.finite_faces_end(); ++f) f->info() = face_idx++;
  weighted_graph G(num_faces + 1);
  weight_map weights = boost::get(boost::edge_weight, G);
  vertex_desc source = num_faces;
  
  for (auto f = dt.finite_faces_begin(); f != dt.finite_faces_end(); ++f) { 
    for(int i = 0; i < 3; ++i) {
      FT edge_radius = CGAL::squared_distance(f->vertex((i + 1) % 3)->point(), f->vertex((i + 2) % 3)->point()) / FT(4.0);
      if(!dt.is_infinite(f->neighbor(i))) {
        edge_desc e;
        e = boost::add_edge(f->info(), f->neighbor(i)->info(), G).first; weights[e]=edge_radius;
      } else {
        edge_desc e;
        e = boost::add_edge(f->info(), source, G).first; weights[e]=edge_radius;
      }
    }
  }
  
  std::vector<FT> dist_map(boost::num_vertices(G));
  boost::dijkstra_shortest_paths(G, source,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G)))
    .weight_map(weights)
    .distance_compare([](const FT& a, const FT &b) {return a > b;})
    .distance_combine([](const FT& a, const FT &b) {return std::min(a, b);})
    .distance_inf(FT(0.0))
    .distance_zero(std::numeric_limits<FT>::infinity())
  );
  
  int m;
  std::cin >> m;
  for(int i = 0; i < m; ++i) {
    int x, y; long d;
    std::cin >> x >> y >> d;
    FT d_ft = FT(d);
    Point q(x, y);
    auto v = dt.nearest_vertex(q);
    if(CGAL::squared_distance(q, v->point()) < d) {std::cout << "n"; continue;}
    auto f = dt.locate(q);
    if(dt.is_infinite(f)) {std::cout << "y"; continue;}
    if(dist_map[f->info()] >= d) {std::cout << "y";}
    else {std::cout << "n";}
  }
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  for(int n; std::cin >> n && n != 0;) {
    solve_task(n);
  }
}