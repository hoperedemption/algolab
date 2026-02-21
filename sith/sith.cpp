#include <iostream>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::FT FT;

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;

int count_components(int s, vi& visited, const vvi& adj) {
  std::queue<int> q;
  visited[s] = true;
  q.push(s);
  int count = 0;
  while(!q.empty()) {
    int u = q.front();
    q.pop();
    count++;
    for(const int v: adj[u]) {
      if(!visited[v]) {
        visited[v] = true;
        q.push(v);
      }
    }
  }
  return count;
}

void solve_task() {
  int n, r;
  std::cin >> n >> r;
  FT r2 = FT(r) * FT(r);
  
  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  int lo = 0;
  int hi = n;
  while(lo < hi) {
    int mid = lo + (hi - lo) / 2;
    
    vi visited(n, false);
    for(int i = 0; i < mid; ++i) visited[i] = true;
    
    std::vector<IPoint> curpoints;
    for(int i = mid; i < n; ++i) {
      curpoints.push_back(points[i]);
    }
    Delaunay dt;
    dt.insert(curpoints.begin(), curpoints.end());
    
    vvi adj(n, vi());
    for(auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) {
      if(dt.segment(e).squared_length() <= r2) {
        auto f = e->first;
        int i = e->second;
        int u = f->vertex((i+1)%3)->info();
        int v = f->vertex((i+2)%3)->info();
        adj[u].push_back(v);
        adj[v].push_back(u);
      }
    }
    
    bool ok = false;
    for(int u = 0; u < n; ++u) {
      if(!visited[u]) {
        int compsize = count_components(u, visited, adj);
        if(compsize >= mid) {
          ok = true; break;
        }
      }
    }
    
    if(ok) lo = mid + 1;
    else hi = mid;
  }
  
  std::cout << lo - 1 << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    solve_task();
  }
}