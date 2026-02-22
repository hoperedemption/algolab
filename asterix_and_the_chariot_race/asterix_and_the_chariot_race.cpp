#include <vector>
#include <iostream>
#include <limits>

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;

void dfs(int u, vi& visited, vvi& dp, const vvi& adj, const vi& cost) {
  dp[u][0] = 0;
  dp[u][1] = cost[u];
  dp[u][2] = std::numeric_limits<int>::max();
  
  for(const int v: adj[u]) {
    if(!visited[v]) {
      dfs(v, visited, dp, adj, cost);
    }
  }
  
  for(const int w: adj[u]) {
    dp[u][2] = std::min(dp[u][2], dp[w][1] - std::min(dp[w][1], dp[w][2]));
  }
  for(const int v: adj[u]) {
    dp[u][0] += std::min(dp[v][1], dp[v][2]);
    dp[u][1] += std::min(dp[v][0], dp[v][1]);
    dp[u][2] += std::min(dp[v][1], dp[v][2]);
  }
}

void solve_task() {
  int n; std::cin >> n;
  vvi adj(n, vi());
  for(int i = 0; i < n - 1; ++i) {
    int u, v;
    std::cin >> u >> v;
    adj[u].push_back(v);
  }
  vi c(n);
  for(int i = 0; i < n; ++i) std::cin >> c[i];
  vi visited(n, false);
  vvi dp(n, vi(3));
  dfs(0, visited, dp, adj, c);
  std::cout << std::min(dp[0][1], dp[0][2]) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}