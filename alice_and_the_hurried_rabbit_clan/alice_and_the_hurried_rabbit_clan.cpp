#include <iostream>
#include <vector>
#include <limits>

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;
typedef std::vector<long> vl;
typedef std::vector<vl> vvl;

void solve_task() {
  int n, m;
  std::cin >> n >> m;
  vvi r(n, vi(m));
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < m; ++j) {
      std::cin >> r[i][j];
    }
  }
  vvl pref(n + 1, vl(m + 1, 0));
  for(int i = 1; i <= n; ++i) {
    for(int j = 1; j <= m; ++j) {
      pref[i][j] = r[i-1][j-1] + pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1];
    }
  }
  
  auto sum = [&](int i1, int j1, int i2, int j2) {
    return pref[i2+1][j2+1] - pref[i2+1][j1] - pref[i1][j2+1] + pref[i1][j1];
  };
  
  const long inf = std::numeric_limits<long>::max();
  vvl dp(n, vl(m, inf));
  dp[n-1][m-1] = 0;
  for(int i = n - 1; i >= 0; i--) {
    for(int j = m - 1; j >= 0; j--) {
      if(i == n-1 && j == m-1) continue;
      
      if(i < n - 1) {
        dp[i][j] = std::min(dp[i][j], dp[i+1][j] + sum(0, j + 1, i, m-1));
      }
      if(j < m - 1) {
        dp[i][j] = std::min(dp[i][j], dp[i][j+1] + sum(i+1, 0, n-1, j));
      }
    }
  }
  
  std::cout << dp[0][0] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}