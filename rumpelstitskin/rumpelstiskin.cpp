///3
#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

typedef std::vector<int> vi;

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
  int n, pG, pH, eG, eH, fG, fH, sG, sH;
  std::cin >> n >> pG >> pH >> eG >> eH >> fG >> fH >> sG >> sH;
  
  vi a(n);
  for(int i = 0; i < n; ++i) std::cin >> a[i];
  int amax = *std::max_element(a.begin(), a.end());
  
  graph G(2*n + pG + pH);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  adder.add_edge(v_source, 0, sG, 0);
  
  for(int i = 0; i < eG; ++i) {
    int u, v, c;
    std::cin >> u >> v >> c;
    adder.add_edge(u, v, c, 0);
  }
  
  for(int i = 0; i < eH; ++i) {
    int u, v, c;
    std::cin >> u >> v >> c;
    adder.add_edge(pG + 2*n + v, pG + 2*n + u, c, 0);
  }
  
  for(int i = 0; i < fG; ++i) {
    int u, v, c;
    std::cin >> u >> v >> c;
    adder.add_edge(u, pG + v, c, 0);
  }
  
  for(int i = 0; i < fH; ++i) {
    int u, v, c;
    std::cin >> u >> v >> c;
    adder.add_edge(pG + n + v, pG + 2*n + u, c, 0);
  }
  
  for(int i = 0; i < n; ++i) {
    adder.add_edge(pG + i, pG + n + i, 1, amax - a[i]);
  }
  
  adder.add_edge(pG + 2*n, v_sink, sH, 0);
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int mincost = boost::find_flow_cost(G);
  int t_flow = 0;
  out_edge_it e, eend;
  for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_sink,G), G); e != eend; ++e) {
    t_flow += rc_map[*e] - c_map[*e];  
  }
  std::cout << t_flow << " " << amax*t_flow - mincost << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}