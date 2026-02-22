#include <iostream>
#include <vector>
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
typedef std::vector<vi> vvi;

void solve_task() {
  int n, b, k, m;
  std::cin >> n >> b >> k >> m;
  int utopia = 0;
  vi a(n);
  for(int i = 0; i < n; ++i) {std::cin >> a[i]; utopia += std::abs(a[i]);}
  vvi f(n, vi(n));
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      std::cin >> f[i][j];
      if(i < j) utopia += f[i][j];
    }
  }
  
  long mincut = std::numeric_limits<long>::max();
  for(int choice = 0; choice < 2; ++choice) {
    graph G(n);
    edge_adder adder(G);
    for(int i = 0; i < n; ++i) {
      for(int j = 0; j < n; ++j) {
        if(i == j || f[i][j] == 0) continue;
        adder.add_edge(i, j, f[i][j]);
      }
    }
    
    if(choice == 0) { // k is in Team A and m is in Team B
      for(int i = 0; i < n; ++i) {
        if(i != k && a[i] > 0) {
          adder.add_edge(k, i, a[i]);
          adder.add_edge(i, k, a[i]);
        } else if(i != m && a[i] < 0) {
          adder.add_edge(m, i, -a[i]);
          adder.add_edge(i, m, -a[i]);
        }
      }
    } else if(choice == 1) { // k is in Team B and m is in Team A
      for(int i = 0; i < n; ++i) {
        if(i != m && a[i] > 0) {
          adder.add_edge(m, i, a[i]);
          adder.add_edge(i, m, a[i]);
        } else if(i != k && a[i] < 0) {
          adder.add_edge(k, i, -a[i]);
          adder.add_edge(i, k, -a[i]);
        }
      }
    }
    
    long flow = boost::push_relabel_max_flow(G, k, m);
    mincut = std::min(mincut, flow);
  }
  
  if(b == 1) std::cout << mincut << "\n";
  else std::cout << utopia - 2*mincut << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}