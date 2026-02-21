#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <limits>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
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

typedef std::vector<int> vi;
typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;

void solve_task() {
  int n, m, k, l;
  std::cin >> n >> m >> k >> l;
  vi police(n, 0);
  for(int i = 0; i < k; ++i) {
    int policei; std::cin >> policei;
    police[policei]++;
  }
  vi photo(n, 0);
  for(int i = 0; i < l; ++i) {
    int photoi; std::cin >> photoi;
    photo[photoi]++;
  }
  vii edges(m);
  for(int i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    edges[i] = {x, y};
  }
  
  graph G(2*n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  long inf = std::numeric_limits<long>::max();
  for(const ii& edge: edges) {
    int x, y;
    std::tie(x, y) = edge;
    adder.add_edge(x, y, inf);
    adder.add_edge(n+x, n+y, 1);
  }
  
  for(int i = 0; i < n; ++i) {
    if(police[i]) {
      adder.add_edge(v_source, i, police[i]);
      adder.add_edge(n+i, v_sink, police[i]);
    }
    if(photo[i]) adder.add_edge(i, n+i, photo[i]);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}