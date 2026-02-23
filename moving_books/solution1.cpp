#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> vi;

void solve_task() {
  int n, m;
  std::cin >> n >> m;
  vi s(n);
  for(int i = 0; i < n; ++i) std::cin >> s[i];
  vi w(m);
  for(int i = 0; i < m; ++i) std::cin >> w[i];
  
  std::sort(s.begin(), s.end(), std::greater<int>());
  std::sort(w.begin(), w.end(), std::greater<int>());
  
  if(w[0] > s[0]) {std::cout << "impossible" << "\n"; return;}
  
  int lo = 1; int hi = m;
  while(lo < hi) {
    int mid = lo + (hi - lo)/2;
    int p = 0;
    for(int i = 0; i < n && p < m; ++i) {
      for(int t = 0; t < mid && p < m; ++t) {
        if(w[p] <= s[i]) p++;
        else break;
      }
    }
    if(p == m) hi = mid;
    else lo = mid + 1;
  }
  
  std::cout <<  3*lo-1 << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}