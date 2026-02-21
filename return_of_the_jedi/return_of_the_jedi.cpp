#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <utility>
#include <limits>

struct Edge {
  int from;
  int to;
  int cost;
};

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;
typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;
typedef std::vector<vii> vvii;

void solve_task() {
  int n, i;
  std::cin >> n >> i;
  vvi w(n + 1, vi(n + 1, 0));
  for(int j = 1; j <= n - 1; ++j) {
    for(int k = 1; k <= n - j; ++k) {
      int wij; std::cin >> wij;
      w[j][j+k] = w[j+k][j] = wij;
    }
  }
  
  auto comp = [](const Edge& a, const Edge& b) {
    if(a.cost != b.cost) return a.cost > b.cost;
    if(a.to != b.to) return a.to < b.to;
    return a.from < b.from;
  };
  std::priority_queue<Edge, std::vector<Edge>, decltype(comp)> pq(comp);
  vi in_tree(n + 1, false);
  vvii tree(n + 1, vii());
  vvi tree_edge(n + 1, vi(n + 1, false));
  pq.push({-1, i, 0});
  
  long mst_cost = 0;
  int taken = 0;
  while(taken < n) {
    Edge e = pq.top();
    int u = e.from;
    int v = e.to;
    int cost = e.cost;
    pq.pop();
    
    if(in_tree[v]) continue;
    
    in_tree[v] = true;
    taken++;
    mst_cost += cost;
    if(u != -1) {
      tree_edge[u][v] = true;
      tree_edge[v][u] = true;
      tree[u].push_back({v, cost});
      tree[v].push_back({u, cost});
    }
    
    for(int t = 1; t <= n; ++t) {
      if(t == v || in_tree[t]) continue;
      pq.push({v, t, w[v][t]});
    }
  }
  
  vvi max_edge(n + 1, vi(n + 1, 0));
  for(int s = 1; s <= n; ++s) {
    std::queue<int> q;
    vi visited(n + 1, false);
    q.push(s);
    visited[s] = true;
    while(!q.empty()) {
      int u = q.front();
      q.pop();
      
      for(ii edge: tree[u]) {
        int v, cost;
        std::tie(v, cost) = edge;
        if(!visited[v]) {
          max_edge[s][v] = std::max(max_edge[s][u], cost);
          max_edge[v][s] = std::max(max_edge[u][s], cost);
          visited[v] = true;
          q.push(v);
        }
      }
    }
  }
  
  long second_best_mst_cost = std::numeric_limits<long>::max();
  for(int u = 1; u <= n; ++u) {
    for(int v = 1; v <= n; ++v) {
      if(u == v || tree_edge[u][v]) continue;
      second_best_mst_cost = std::min(second_best_mst_cost, mst_cost - max_edge[u][v] + w[u][v]);
    }
  }
  
  std::cout << second_best_mst_cost << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}