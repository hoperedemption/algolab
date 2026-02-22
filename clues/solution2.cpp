///2
#include <iostream>
#include <vector>

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::FT FT;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef Delaunay::Edge_circulator Edge_circulator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS    // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges

void solve_task() {
    long n, m, r;
    std::cin >> n >> m >> r;

    FT r2 = FT(r * r);

    std::vector<std::pair<Point, Index> > points;
    points.reserve(n);
    for (Index i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        points.emplace_back(Point(x, y), i);
    }

    // This step takes O(n log n) time (for constructing the triangulation).
    Delaunay dt;
    dt.insert(points.begin(), points.end());

    graph G(n);

    for(auto vh: dt.finite_vertex_handles()) {
        auto ch = dt.incident_edges(vh);
        if(ch != nullptr) {
            do {
                if(dt.is_infinite(ch)) continue;

                auto vk = ch->first->vertex((ch->second + 1) % 3)->info() == vh->info() ? ch->first->vertex((ch->second + 2) % 3) : ch->first->vertex((ch->second + 1) % 3);

                auto ck = dt.incident_edges(vk);

                if(ck != nullptr) {
                    do {
                        if(dt.is_infinite(ck)) continue;

                        auto vl = ck->first->vertex((ck->second + 1) % 3)->info() == vk->info() ? ck->first->vertex((ck->second + 2) % 3) : ck->first->vertex((ck->second + 1) % 3);
                        if(vh->info() == vl->info()) continue; // no self loops

                        if(!boost::edge(vh->info(), vl->info(), G).second && CGAL::squared_distance(vh->point(), vl->point()) <= r2) {
                            boost::add_edge(vh->info(), vl->info(), G);
                        }
                    } while(++ck != dt.incident_edges(vk));
                }

                if(!boost::edge(vh->info(), vk->info(), G).second && CGAL::squared_distance(vh->point(), vk->point()) <= r2) {
                    boost::add_edge(vh->info(), vk->info(), G);
                }
            } while(++ch != dt.incident_edges(vh));
        }
    }

    bool is_bipartite = boost::is_bipartite(G);

    std::vector<int> component_map(n);  // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
    int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

    for(int i = 0; i < m; ++i) {
        int x, y, w, z;
        std::cin >> x >> y >> w >> z;
        Point a(x, y);
        Point b(w, z);

        if(!is_bipartite) {std::cout << "n"; continue;}

        FT d2 = CGAL::squared_distance(a, b);
        if(d2 <= r2) {
            std::cout << "y";
            continue;
        }

        Vertex_handle va = dt.nearest_vertex(a);
        if(CGAL::squared_distance(va->point(), a) > r2) {std::cout << "n"; continue;}
        Vertex_handle vb = dt.nearest_vertex(b);
        if(CGAL::squared_distance(vb->point(), b) > r2) {std::cout << "n"; continue;}
        std::cout << ((component_map[va->info()] == component_map[vb->info()]) ? "y" : "n");
    }
    std::cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t;
    std::cin >> t;
    while(t--) {
        solve_task();
    }
    return 0;
}