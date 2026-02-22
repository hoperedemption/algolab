#include <iostream>
#include <vector>
#include <limits>

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;

void solve_task() {
  int n; 
  std::cin >> n;
  vi a(n);
  for(int i = 0; i < n; ++i) std::cin >> a[i];
  vi b(n);
  for(int i = 0; i < n; ++i) std::cin >> b[i];
  
  const int inf = std::numeric_limits<int>::max();
  vvi dp(n, vi(n, inf));
  dp[0][0] = (a[0] - 1)*(b[0] - 1);
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < n; ++j) {
      if(i == 0 && j == 0) continue;
      
      int base = (a[i] - 1)*(b[j] - 1);
      if(i > 0) dp[i][j] = std::min(dp[i][j], dp[i-1][j] + base);
      if(j > 0) dp[i][j] = std::min(dp[i][j], dp[i][j-1] + base);
      if(i > 0 && j > 0) dp[i][j] = std::min(dp[i][j], dp[i-1][j-1] + base);
    }
  }
  
  std::cout << dp[n-1][n-1] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}