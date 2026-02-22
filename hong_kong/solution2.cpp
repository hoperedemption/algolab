#include <iostream>
#include <vector>
#include <limits>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef std::size_t                                            Index;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_circulator Face_circulator;
typedef K::Point_2 Point;
typedef K::FT FT;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, FT> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void solve_task() {
  int n, m;
  std::cin >> n >> m;
  unsigned long r;
  std::cin >> r;
  
  std::vector<Point> pts;
  pts.reserve(n);
  for(int i = 0; i < n; ++i) {
    long x, y;
    std::cin >> x >> y;
    pts.push_back(Point(x, y));
  }
  
  Triangulation dt;
  dt.insert(pts.begin(), pts.end());
  
  int num_faces = std::distance(dt.finite_faces_begin(), dt.finite_faces_end());
  int f_index = 0;
  for(auto it = dt.finite_faces_begin(); it != dt.finite_faces_end(); ++it) it->info() = f_index++;
  
  weighted_graph G(num_faces + 1);
  const int source = num_faces;
  weight_map weights = boost::get(boost::edge_weight, G);
  
  for(auto it = dt.finite_faces_begin(); it != dt.finite_faces_end(); ++it) {
    auto cc = dt.circumcenter(it);
    FT curr2 = CGAL::squared_distance(it->vertex(0)->point(), cc);
    // curr2 is the current sq radius which serves as our base value for Dijkstra
    edge_desc e;
    e = boost::add_edge(it->info(), source, G).first;
    weights[e] = curr2 / FT(4.0); // divide by 4 to have comparable equations
    
    for(int k = 0; k < 3; ++k) {
      FT dist2 = CGAL::squared_distance(it->vertex((k + 1) % 3)->point(), it->vertex((k + 2) % 3)->point()) / FT(4.0);

      if(!dt.is_infinite(it->neighbor(k))) {
        edge_desc e;
        e = boost::add_edge(it->info(), it->neighbor(k)->info(), G).first; 
        weights[e]=dist2;
      } else {
        edge_desc e;
        e = boost::add_edge(it->info(), source, G).first; 
        weights[e]=dist2;
      }
    }
  }
  
  std::vector<FT> dist_map(num_faces + 1, FT(0.0));
  boost::dijkstra_shortest_paths(G, source,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G)))
    .weight_map(weights)
    .distance_compare(std::greater<FT>())
    .distance_combine([](const FT a, const FT b){return std::min(a, b);})
    .distance_inf(FT(0.0))
    .distance_zero(FT(std::numeric_limits<double>::infinity()))
  );
  
  for(int j = 0; j < m; ++j) {
    long x, y;
    std::cin >> x >> y;
    unsigned long s;
    std::cin >> s;
    
    FT rs = FT(r + s);

    Point p(x, y);
    auto q = dt.nearest_vertex(p)->point();
    auto nearest_distance = CGAL::squared_distance(p, q);
    
    if(nearest_distance < rs * rs) {std::cout << "n"; continue;}
    
    auto f = dt.locate(p);
    if(dt.is_infinite(f)) {std::cout << "y"; continue;}
    std::cout << ((dist_map[f->info()] >= rs * rs) ? "y" : "n");
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