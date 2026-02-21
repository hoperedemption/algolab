#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <algorithm>

typedef std::string string;
typedef std::pair<string, int> si;
typedef std::vector<si> vsi;
typedef std::unordered_map<string, int> msi;
typedef std::vector<int> vi;
typedef std::vector<vi> vvi;
typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;
typedef std::vector<vii> vvii;

void dfs(int u, vi& visited, vii& path, vi& ans, const vvi& adj, const vvii& queries, const vi& age) {
  visited[u] = true;
  path.push_back({u, age[u]});
  
  for(const ii& query: queries[u]) {
    int qidx, b;
    std::tie(qidx, b) = query;
    auto it = std::lower_bound(path.begin(), path.end(), b, [](const ii& el, int val) {
      return el.second > val;
    });
    ans[qidx] = it->first;
  }
  
  for(int v: adj[u]) {
    if(!visited[v]) {
      dfs(v, visited, path, ans, adj, queries, age);
    }
  }
  
  path.pop_back();
}

void solve_task() {
  int n, q;
  std::cin >> n >> q;
  vsi int_to_string(n);
  vi age(n);
  for(int i = 0; i < n; ++i) {
    string s; int a;
    std::cin >> s >> a;
    int_to_string[i] = {s, i};
    age[i] = a;
  }
  msi string_to_int(int_to_string.begin(), int_to_string.end());
  vvi adj(n, vi());
  vi parent(n, -1);
  for(int i = 0; i < n - 1; ++i) {
    string s, p;
    std::cin >> s >> p;
    int si = string_to_int[s];
    int pi = string_to_int[p];
    adj[pi].push_back(si);
    parent[si] = pi;
  }
  vvii queries(n, vii());
  for(int i = 0; i < q; ++i) {
    string s; int b;
    std::cin >> s >> b;
    int si = string_to_int[s];
    queries[si].push_back({i, b});
  }
  int root;
  for(int i = 0; i < n; ++i) {
    if(parent[i] == -1) {
      root = i;
      break;
    }
  }
  vi ans(q);
  vi visited(n, false);
  vii path;
  dfs(root, visited, path, ans, adj, queries, age);
  for(int i = 0; i < q; ++i) {
    std::cout << int_to_string[ans[i]].first << " ";
  } std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}