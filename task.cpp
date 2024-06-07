#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include "graph.cpp"

template<typename Vertex, typename Distance>
Distance max_distance_from_vertex(const Graph<Vertex, Distance>& graph, const Vertex& vertex) {
    Distance max_distance = 0;
    for (const auto& target : graph.vertices()) {
        if (vertex != target) {
            std::vector<typename Graph<Vertex, Distance>::Edge> path = graph.shortest_path(vertex, target);
            Distance total_distance = 0;
            for (const auto& edge : path) {
                total_distance += edge.distance;
            }
            if (total_distance > max_distance) {
                max_distance = total_distance;
            }
        }
    }
    return max_distance;
}

template<typename Vertex, typename Distance>
Vertex find_optimal_warehouse(const Graph<Vertex, Distance>& graph) {
    Vertex optimal_vertex;
    Distance minimal_max_distance = std::numeric_limits<Distance>::infinity();

    for (const auto& vertex : graph.vertices()) {
        Distance max_distance = max_distance_from_vertex(graph, vertex);
        if (max_distance < minimal_max_distance) {
            minimal_max_distance = max_distance;
            optimal_vertex = vertex;
        }
    }

    return optimal_vertex;
}

int main() {
    Graph<int, double> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_vertex(5);

    graph.add_edge(1, 2, 1.0);
    graph.add_edge(2, 3, 1.0);
    graph.add_edge(3, 4, 1.0);
    graph.add_edge(4, 5, 1.0);
    graph.add_edge(1, 5, 2.5);

    int optimal_warehouse = find_optimal_warehouse(graph);
    std::cout << "Optimal warehouse location: " << optimal_warehouse << std::endl;

    return 0;
}