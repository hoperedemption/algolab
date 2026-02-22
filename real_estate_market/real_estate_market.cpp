#include <vector>
#include <iostream>
  
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator


// custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void solve_task() {
  int N, M, S; std::cin >> N >> M >> S;
  
  std::vector<int> l(S);
  for(int i = 0; i < S; ++i) std::cin >> l[i];
  
  std::vector<int> s(M);
  for(int i = 0; i < M; ++i) std::cin >> s[i];
  
  graph G(N + M + S);
  edge_adder adder(G);
  
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for(int i = 0; i < N; ++i) {
    adder.add_edge(v_source, i, 1, 0);
  }
  
  for(int i = 0; i < N; ++i) {
    for(int j = 0; j < M; ++j) {
      int bidij; std::cin >> bidij;
      adder.add_edge(i, N + j, 1, 100 - bidij);
    }
  }
  
  for(int j = 0; j < M; ++j) {
    adder.add_edge(N + j, N + M + s[j] - 1, 1, 0);
  }
  
  for(int k = 0; k < S; ++k) {
    adder.add_edge(N + M + k, v_sink, l[k], 0);
  }
  
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost = 100 * flow - boost::find_flow_cost(G);

  std::cout << flow << " " << cost << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}