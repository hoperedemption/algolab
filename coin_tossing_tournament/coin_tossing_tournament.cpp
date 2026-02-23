///1
#include <iostream>
#include <vector>
#include <algorithm>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int solve_task() {
    int n; int m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> game_pairs(n, std::vector<int>(n, 0));
    std::vector<int> init_scoreboard(n, 0);

    int to_distribute = 0;
    for(int i = 0; i < m; ++i) {
        int a; int b; int c;
        std::cin >> a >> b >> c;

        if(c == 1) {
            init_scoreboard[a]++;
        } else if(c == 2) {
            init_scoreboard[b]++;
        } else if(c == 0) {
            if(a > b) {
                std::swap(a, b);
            }
            game_pairs[a][b]++;
            to_distribute++;
        }
    }
    
    bool impossible = 0;
    int sum_final_score = 0;
    std::vector<int> to_achieve_score(n);
    for(int i = 0; i < n; ++i) {
        int final_score;
        std::cin >> final_score;
        sum_final_score += final_score;

        if(final_score < init_scoreboard[i]) impossible = 1;
        to_achieve_score[i] = final_score - init_scoreboard[i];
    }

    if(impossible || sum_final_score != m) return 0;
    
    auto coordinates_to_int = [n](const int i, const int j) {
        return i * n + j;
    };

    graph G(n*n + n);
    edge_adder adder(G);

    // Add special vertices source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            if(game_pairs[i][j] > 0) {
                adder.add_edge(v_source, coordinates_to_int(i, j), game_pairs[i][j]);
                adder.add_edge(coordinates_to_int(i, j), n*n + i, game_pairs[i][j]);
                adder.add_edge(coordinates_to_int(i, j), n*n + j, game_pairs[i][j]);
            }
        }
    }

    for(int i = 0; i < n; ++i) {
        adder.add_edge(n*n + i, v_sink, to_achieve_score[i]);
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    return (flow == to_distribute);
}

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    std::cin >> t;
    while(t--) {
        int ans = solve_task();
        std::cout << (ans ? "yes" : "no") << "\n";
    }
}