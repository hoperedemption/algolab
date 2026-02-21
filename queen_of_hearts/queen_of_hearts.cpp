///3
#include <iostream>
#include <vector>
#include <queue>
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
typedef std::vector<vi> vvi;
typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;

int solve_carrots(const vvi& adj, const int ni, const int ci) {
  std::queue<int> q;
  vi dist(ni, -1);
  q.push(0);
  dist[0] = 0;
  while(!q.empty()) {
    int u = q.front();
    q.pop();
    for(const int v: adj[u]) {
      if(dist[v] == -1) {
        dist[v] = dist[u] + 1;
        q.push(v);
      }
    }
  }
  if(dist[ni-1] == -1) return 0;
  return std::max(ci - dist[ni-1], 0);
}

long solve_min_cut(const vii& edges, const vi& coefs, const int r) {
  graph G(2*r + 2);
  edge_adder adder(G);
  for(int i = 1; i <= r; ++i) {
    adder.add_edge(i, r+1+i, coefs[i-1]);
  }
  int inf = std::numeric_limits<int>::max();
  for(const ii& edge: edges) {
    int u, v;
    std::tie(u, v) = edge;
    int effu = (u == 0 || u == r+1) ? u : r + 1 + u;
    adder.add_edge(effu, v, inf);
  }
  long flow = boost::push_relabel_max_flow(G, 0, r+1);
  return flow;
}

void solve_task() {
  int r, m, d;
  std::cin >> r >> m >> d;
  vi carrots(r);
  for(int i = 0; i < r; ++i) {
    int ni, mi, ci;
    std::cin >> ni >> mi >> ci;
    vvi adj(ni, vi());
    for(int j = 0; j < mi; ++j) {
      int u, v;
      std::cin >> u >> v;
      adj[u].push_back(v);
    }
    carrots[i] = solve_carrots(adj, ni, ci);
  }
  vii edges(m);
  for(int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    edges[i] = {u, v};
  }
  vi coins(r);
  for(int i = 0; i < r; ++i) coins[i] = (carrots[i] != 0 ? 1 : 0);
  
  if(d == 0) std::cout << solve_min_cut(edges, carrots, r) << " " << 0 << "\n";
  else std::cout << solve_min_cut(edges, carrots, r) << " " << solve_min_cut(edges, coins, r) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}