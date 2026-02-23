#include <iostream>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <algorithm>

typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;

void solve_task() {
  int n; 
  std::cin >> n;
  vii boats(n);
  for(int i = 0; i < n; ++i) {
    int l, p;
    std::cin >> l >> p;
    boats[i] = {l, p};
  }
  std::sort(boats.begin(), boats.end(), [](const auto a, const auto b) {
    return a.second < b.second;
  });
  
  int neginf = std::numeric_limits<int>::min();
  int X1 = neginf;
  int X2 = neginf;
  int count = 0;
  for(int i = 0; i < n; ++i) {
    int l, p;
    std::tie(l, p) = boats[i];
    if(p < X2) {
      int newX2 = std::max(X1 + l, p);
      X2 = std::min(X2, newX2);
    } else {
      X1 = X2;
      X2 = std::max(X2 + l, p);
      count++;
    }
  }
  
  std::cout << count << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}