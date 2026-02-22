///1
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

typedef std::vector<int> vi;

void solve_task() {
  int n, m, b, p, d;
  std::cin >> n >> m >> b >> p >> d;
  vi a(b);
  for(int i = 0; i < b; ++i) std::cin >> a[i];
  vi q(n, false);
  for(int i = 0; i < p; ++i) {
    int qi; std::cin >> qi;
    q[qi] = true;
  }
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  const vertex_desc v_start = boost::add_vertex(G);
  
  for(int i = 0; i < m; ++i) {
    int x, y; long l;
    std::cin >> x >> y >> l;
    edge_desc e;
    e = boost::add_edge(x, y, G).first; weights[e]=l;
  }
  
  for(int i = 0; i < b; ++i) {
    edge_desc e;
    e = boost::add_edge(v_start, a[i], G).first; weights[e] = 0;
  }
  
  std::vector<long> dist_map(boost::num_vertices(G));
  boost::dijkstra_shortest_paths(G, v_start,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  
  graph F(2*n);
  edge_it e_beg, e_end;
  for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg) { 
    vertex_desc u = boost::source(*e_beg, G);
    vertex_desc v = boost::target(*e_beg, G);
    if(u == v_start || v == v_start) continue;
    if(dist_map[u] <= d && dist_map[v] <= d) {
      boost::add_edge(u, v, F);
      if(q[u]) {
        boost::add_edge(n+u, v, F);
      } 
      if(q[v]) {
        boost::add_edge(u, n+v, F);
      }
    }
  }
  
  std::vector<vertex_desc> mate_map(boost::num_vertices(F));  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

  boost::edmonds_maximum_cardinality_matching(F,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, F)));
  int matching_size = boost::matching_size(F,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, F)));
  
  std::cout << matching_size << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}