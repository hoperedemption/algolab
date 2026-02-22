#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef Delaunay::Finite_vertex_handles Finite_vertex_handles;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef Delaunay::Edge_circulator Edge_circulator;
typedef K::Point_2 Point;
typedef K::FT FT;

double ceil_to_double(double x) {
    double a = std::ceil(x);
    while (a >= x + 1) a -= 1;
    while (a < x) a += 1;
    return a;
}

void solve_task(size_t n) {
    int l, b, r, t;
    std::cin >> l >> b >> r >> t;

    std::vector<std::pair<Point,Index>> points;
    points.reserve(n);
    std::vector<FT> node_map(n);

    for (Index i = 0; i < n; ++i) {
        long x, y;
        std::cin >> x >> y;
        points.emplace_back(K::Point_2(x, y), i);

        long min_coord = 2 * std::min({
            x - l, 
            r - x, 
            y - b, 
            t - y
        }); // mapping preserves monotonicity

        node_map[i] = FT(min_coord * min_coord);
    }
    // construct triangulation in O(n log n)
    Delaunay dt;
    dt.insert(points.begin(), points.end());

    // for each vertex construct a map index -> min(coords, min(edge_weight))
    // store that min as a squared distance
    for(auto vh: dt.finite_vertex_handles()) {
        Index i = vh->info();
        auto c = dt.incident_edges(vh);
        if(c != nullptr) {
            do {
                const auto e = (*c);
                if(!dt.is_infinite(e)) {
                    FT w2 = dt.segment(e).squared_length();
                    if(w2 < node_map[i]) {
                        node_map[i] = w2;
                    }
                }
            } while(++c != dt.incident_edges(vh));
        }
    }

    std::sort(node_map.begin(), node_map.end());

    int first = -1;
    int median = -1;
    int last = -1;

    for(int i = 0; i < n; ++i) {
        double radius = std::sqrt(CGAL::to_double(node_map[i])) / 2;
        int t = (int)ceil_to_double(std::sqrt(radius - 0.5));
        
        if(i == 0) first = t;
        if(i == n/2) median = t;
        if(i == n - 1) last = t;
    }
    std::cout << first << " " << median << " " << last << "\n";
}

int main () {
    for(size_t n; std::cin >> n && n != 0;) {
        solve_task(n);
    }
    return 0;
}