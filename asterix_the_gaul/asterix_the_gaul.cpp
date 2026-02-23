#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>

typedef std::pair<long, long> ll;
typedef std::vector<ll> vll;
typedef std::vector<long> vl;

void solve_task() {
  int n, m; long D, T;
  std::cin >> n >> m >> D >> T;
  vl dist(n); vl t(n);
  for(int i = 0; i < n; ++i) {
    std::cin >> dist[i] >> t[i];
  }
  vl s(m + 1, 0);
  for(int i = 1; i <= m; ++i) std::cin >> s[i];
  
  int lo = 0;
  int hi = m + 1;
  while(lo < hi) {
    int mid = lo + (hi - lo) / 2;
    
    int l = n / 2;
    int r = n - l;
    int L = 1 << l;
    int R = 1 << r;
    vll VL;
    vll VR;
    for(int maskL = 0; maskL < L; ++maskL) {
      long timeL = 0;
      long distL = 0;
      long countL = 0;
      for(int i = 0; i < l; ++i) {
        if((maskL >> i) & 1) {
          timeL += t[i];
          distL += dist[i];
          countL += 1;
        }
      }
      if(timeL < T) VL.push_back({timeL, distL + countL*s[mid]});
    }
    for(int maskR = 0; maskR < R; ++maskR) {
      long timeR = 0;
      long distR = 0;
      long countR = 0;
      for(int i = 0; i < r; ++i) {
        if((maskR >> i) & 1) {
          timeR += t[l+i];
          distR += dist[l+i];
          countR += 1;
        }
      }
      if(timeR < T) VR.push_back({timeR, distR + countR*s[mid]});
    }
    
    std::sort(VR.begin(), VR.end(), [](const ll& a, const ll& b) {
      if(a.first != b.first) return a.first < b.first;
      return a.second > b.second;
    });
    
    long bestd = std::numeric_limits<long>::min();
    vll PR;
    for(int i = 0; i < VR.size();) {
      long tR, dR;
      std::tie(tR, dR) = VR[i];
      bestd = std::max(bestd, dR);
      PR.push_back({tR, bestd});
      while(i < VR.size() && tR == VR[i].first) {
        i++;
      }
    }
    
    bool ok = false;
    for(const ll& p: VL) {
      long tL, dL;
      std::tie(tL, dL) = p;
      long rem = T - tL;
      auto it = std::lower_bound(PR.begin(), PR.end(), rem, [](const ll& a, long val) {
        return a.first < val;
      });
      long dR = (it == PR.begin() ? 0 : std::prev(it)->second);
      if(dL + dR >= D) {ok = true; break;}
    }
    
    if(ok) hi = mid;
    else lo = mid + 1;
  }
  
  if(lo == m + 1) std::cout << "Panoramix captured" << "\n"; 
  else std::cout << lo << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}