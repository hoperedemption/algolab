#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

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
  int n; std::cin >> n;
  std::vector<std::vector<int>> chessboard(n, std::vector<int>(n, 0));
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      std::cin >> chessboard[i][j];
    }
  }
  
  graph G(n*n);
  edge_adder adder(G);
  
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  int n_vertex = n*n;
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      if(chessboard[i][j] == 0) { n_vertex--; continue;}
      
      if((i + j) % 2 == 1) {adder.add_edge(i*n + j, v_sink, 1); continue;}
      
      adder.add_edge(v_source, i*n + j, 1);
      const int di[8] = {-1, -1, 1, 1, -2, -2, 2, 2};
      const int dj[8] = {-2, 2, -2, 2, -1, 1, -1, 1};
      
      for(int k = 0; k < 8; ++k) {
        int ik = i + di[k];
        int jk = j + dj[k];
        if(0 <= ik && ik < n && 0 <= jk && jk < n) {
          if(chessboard[ik][jk] == 1) adder.add_edge(i*n + j, ik*n + jk, 1);
        }
      }
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  long mis = n_vertex - flow;
  std::cout << mis << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}