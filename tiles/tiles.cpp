#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;

class edge_adder {
  graph &G;
 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long cap) {
    auto c = boost::get(boost::edge_capacity, G);
    auto r = boost::get(boost::edge_reverse, G);
    auto e = boost::add_edge(from, to, G).first;
    auto rev = boost::add_edge(to, from, G).first;
    c[e] = cap; c[rev] = 0;
    r[e] = rev; r[rev] = e;
  }
};

bool solve() {
  int W, H; std::cin >> W >> H;
  std::vector<std::vector<char>> grid(H, std::vector<char>(W));
  std::vector<int> black, white;
  int free_cells = 0;

  auto id = [W](int i, int j) { return i * W + j; };

  graph G(W * H);
  edge_adder add(G);

  for (int i = 0; i < H; ++i) {
    for (int j = 0; j < W; ++j) {
      std::cin >> grid[i][j];
      if (grid[i][j] != '.') continue;
      ++free_cells;

      int u = id(i, j);
      if ((i + j) % 2 == 0) black.push_back(u);
      else white.push_back(u);

      if (i > 0 && grid[i - 1][j] == '.') {
        int v = id(i - 1, j);
        if ((i + j) % 2 == 0) add.add_edge(u, v, 1);
        else add.add_edge(v, u, 1);
      }
      if (j > 0 && grid[i][j - 1] == '.') {
        int v = id(i, j - 1);
        if ((i + j) % 2 == 0) add.add_edge(u, v, 1);
        else add.add_edge(v, u, 1);
      }
    }
  }

  if (free_cells % 2 != 0) return false;

  vertex_desc S = boost::add_vertex(G), T = boost::add_vertex(G);
  for (int v : black) add.add_edge(S, v, 1);
  for (int v : white) add.add_edge(v, T, 1);

  long flow = boost::push_relabel_max_flow(G, S, T);
  return flow == free_cells / 2;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int tc; std::cin >> tc;
  while (tc--) std::cout << (solve() ? "yes\n" : "no\n");
}
