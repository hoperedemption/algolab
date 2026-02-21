#include <iostream>
#include <vector>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef std::tuple<int, int, int, int> ti;
typedef std::vector<ti> vti;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve_task() {
  int n, m, s, f;
  std::cin >> n >> m >> s >> f;
  vti edges(m);
  for(int i = 0; i < m; ++i) {
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    edges[i] = {a, b, c, d};
  }
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  for(int i = 0; i < m; ++i) {
    int a, b, c, d;
    std::tie(a, b, c, d) = edges[i];
    edge_desc e;
    e = boost::add_edge(a, b, G).first; weights[e]=d;
  }
  std::vector<int> dist_map(boost::num_vertices(G));
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G)))
  );
  
  graph F(n);
  edge_adder adder(F);
  for(int i = 0; i < m; ++i) {
    int a, b, c, d;
    std::tie(a, b, c, d) = edges[i];
    if(dist_map[a] == dist_map[b] + d) {
      adder.add_edge(b, a, c);
    } else if(dist_map[b] == dist_map[a] + d) {
      adder.add_edge(a, b, c);
    }
  }
  long flow = boost::push_relabel_max_flow(F, s, f);
  std::cout << flow << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}