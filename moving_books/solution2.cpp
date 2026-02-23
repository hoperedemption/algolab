#include <vector>
#include <iostream>
#include <set>
#include <algorithm>

int solve_task() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> strength(n);
    for(int i = 0; i < n; ++i)
        std::cin >> strength[i];
    std::sort(strength.begin(), strength.end()); // O(n * log(n))

    std::multiset<int> weight;
    for(int j = 0; j < m; ++j) {
        int weight_j; 
        std::cin >> weight_j;
        weight.insert(weight_j);
    } // O(m * log(m))
    
    int max_weight = *weight.rbegin();
    int max_strength = *std::max_element(strength.begin(), strength.end());
    if(max_weight > max_strength) return -1;
    
    int s_start = 0;
    int s_ptr = 0;
    int cnt = 0;

    while(!weight.empty()) { // O(m)
        if(s_ptr == n) {
            s_ptr = s_start;
            cnt += 3;
        }
        int next = strength[s_ptr];
        s_ptr++;

        auto it = weight.upper_bound(next); // first value > x // O(logm)
        if(it == weight.begin()) {
            if(s_ptr < n) {
                s_start = s_ptr;
            }
            continue; // O(n)
        }
        
        it--; // last value <= x O(logm)
        weight.erase(it);
    } // since the strength array is sorted and that we remember the s_start ptr we get O((n + m) * logm)

    return cnt + 2;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    while(t--) {
        int ans = solve_task();
        if(ans == -1) {
          std::cout << "impossible" << "\n";
        } else {
          std::cout << ans << "\n";
        }
    }
    return 0;
}