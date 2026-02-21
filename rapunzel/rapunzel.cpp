#include <iostream>
#include <vector>
#include <set>
#include <utility>

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;
typedef std::multiset<int> bsi;

void dfs(int u, vi& visited, vi& path, bsi& wdw, vi& ans, const vvi& adj, const vi& h, const int m, const int k) {
  visited[u] = true;
  path.push_back(u);
  wdw.insert(h[u]);
  
  bool deleted = false;
  int erased;
  if(wdw.size() > m) {
    deleted = true;
    erased = path[path.size() - 1 - m];
    wdw.erase(wdw.find(h[erased]));
  }
  
  if(wdw.size() == m) {
    if(*wdw.rbegin() - *wdw.begin() <= k) {
      int root = path[path.size() - m];
      ans[root] = true;
    }
  }
  
  for(int v: adj[u]) {
    if(!visited[v]) {
      dfs(v, visited, path, wdw, ans, adj, h, m, k);
    }
  }
  
  wdw.erase(wdw.find(h[u]));
  path.pop_back();
  if(deleted) {
    wdw.insert(h[erased]);
  }
}

void solve_task() {
  int n, m, k;
  std::cin >> n >> m >> k;
  vi h(n);
  for(int i = 0; i < n; ++i) std::cin >> h[i];
  vvi adj(n, vi());
  for(int i = 0; i < n - 1; ++i) {
    int u, v;
    std::cin >> u >> v;
    adj[u].push_back(v);
  }
  vi visited(n, false);
  vi path;
  bsi wdw;
  vi ans(n, false);
  dfs(0, visited, path, wdw, ans, adj, h, m, k);
  bool end = true;
  for(int i = 0; i < n; ++i) {
    if(ans[i]) {
      end = false;
      std::cout << i << " ";
    }
  }
  if(end) std::cout << "Abort mission";
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}