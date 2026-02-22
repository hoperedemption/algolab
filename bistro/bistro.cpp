#include <iostream>
#include <vector>
#include <utility>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

double floor_to_double(const K::FT& x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

void solve_task(int n) {
    std::vector<std::pair<P, size_t>> sites;
    sites.reserve(n);

    for (Index i = 0; i < n; ++i) {
        long x, y; std::cin >> x >> y;
        sites.emplace_back(P(x, y), i);
    }

    Delaunay t;
    t.insert(sites.begin(), sites.end()); // O(n log n)

    int m;
    std::cin >> m;

    std::vector<P> query_points(m);
    for(int i = 0; i < m; ++i) {
        long x, y;
        std::cin >> x >> y;
        query_points[i] = P(x, y);
    }

    for(int i = 0; i < m; ++i) {
        auto vh = t.nearest_vertex(query_points[i]);     // ~O(log n) on the triangulation
        const P& nearest = vh->point();

        K::FT d2 = CGAL::squared_distance(nearest, query_points[i]); 
        std::cout << (long)(floor_to_double(d2)) << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    for(int n; std::cin >> n && n != 0;) {
        solve_task(n);
    }
    return 0;
}