#include <iostream>
#include <vector>

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


void solve_task() {
  int l, p;
  std::cin >> l >> p;
  graph G(l);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  long demands = 0;
  for(int i = 0; i < l; ++i) {
    int gi, di;
    std::cin >> gi >> di;
    adder.add_edge(v_source, i, gi);
    adder.add_edge(i, v_sink, di);
    demands += di;
  }
  for(int i = 0; i < p; ++i) {
    int fj, tj, cj, Cj;
    std::cin >> fj >> tj >> cj >> Cj;
    adder.add_edge(fj, tj, Cj - cj);
    adder.add_edge(fj, v_sink, cj);
    adder.add_edge(v_source, tj, cj);
    demands += cj;
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if(flow == demands) std::cout << "yes" << "\n";
  else std::cout << "no" << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}