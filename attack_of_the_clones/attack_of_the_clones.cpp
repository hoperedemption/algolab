#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>
#include <limits>

typedef std::pair<int, int> ii;
typedef std::vector<ii> vii;

void solve_task() {
  int n, m;
  std::cin >> n >> m;
  vii segments(n);
  for(int i = 0; i < n; ++i) {
    int a, b;
    std::cin >> a >> b;
    segments[i] = {a, b};
  }
  
  vii lin;
  for(int i = 0; i < n; ++i) {
    int a, b;
    std::tie(a, b) = segments[i];
    
    lin.push_back({a, +1});
      
    if(b < m) lin.push_back({b+1, -1});
  
    if(a > b) lin.push_back({1, +1});
  }
  
  std::sort(lin.begin(), lin.end(), [](const auto a, const auto b) {
    return a.first < b.first;
  });
  
  int min_cnt = std::numeric_limits<int>::max();
  int start_seg = 1;
  
  int count = 0;
  for(int i = 0; i < lin.size();) {
    int p, delta;
    std::tie(p, delta) = lin[i];
    
    while(i < lin.size() && p == lin[i].first) {
      count += lin[i].second;
      i++;
    }
    
    if(count < min_cnt) {
      min_cnt = count;
      start_seg = p;
    }
  }
  
  vii wrap;
  vii proper;
  
  for(int i = 0; i < n; ++i) {
    int a, b;
    std::tie(a, b) = segments[i];
    
    int new_a = a - start_seg;
    int new_b = b - start_seg;
    new_a %= m; if(new_a < 0) new_a += m;
    new_b %= m; if(new_b < 0) new_b += m;
    
    new_a += 1;
    new_b += 1;
    
    if(new_a > new_b) wrap.push_back({new_a, new_b});
    else proper.push_back({new_a, new_b});
  }

  std::sort(proper.begin(), proper.end(), [](const auto a, const auto b){
    return a.second < b.second;
  });
  
  int end = std::numeric_limits<int>::min();
  int count_non_overlap = 0;
  for(const ii& inter: proper) {
    int a, b;
    std::tie(a, b) = inter;
    
    if(a > end) {
      end = b;
      count_non_overlap++;
    }
  }
  
  int max_non_over_lap = count_non_overlap;
  for(const ii& noninter: wrap) {
    int A, B;
    std::tie(A, B) = noninter;
    end = std::numeric_limits<int>::min();
    count_non_overlap = 1;
    
    for(const ii& inter: proper) {
      int a, b;
      std::tie(a, b) = inter;
      
      if(a > B && b < A && a > end) {
        end = b;
        count_non_overlap++;
      }
    }
    
    max_non_over_lap = std::max(max_non_over_lap, count_non_overlap);
  }
  
  std::cout << max_non_over_lap << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();  
  }
}