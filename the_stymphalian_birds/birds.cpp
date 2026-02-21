#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;
typedef std::vector<int> vi;

void solve_task() {
  int n; std::cin >> n;
  vi d(n);
  for(int i = 0; i < n; ++i) std::cin >> d[i];
  for(int i = 0; i < n; ++i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left < n) d[left] = std::min(d[i], d[left]);
    if(right < n) d[right] = std::min(d[i], d[right]);
  }
  vii kll(n);
  for(int i = 0; i < n; ++i) {
    kll[i] = {d[i], i};
  }
  std::sort(kll.begin(), kll.end(), [](const ii& a, const ii& b) {
    if(a.first == b.first) return a.second > b.second;
    return a.first < b.first;
  });
  bool fail = false;
  for(int t = 0; t < n; ++t) {
    if(kll[t].first <= t) {
      fail = true;
      break;
    }
  }
  if(fail) std::cout << "no" << "\n";
  else std::cout << "yes" << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}