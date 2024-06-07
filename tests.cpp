#include <iostream>
#include "graph.cpp"

void test_add_vertex() {
    Graph<int> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    try {
        graph.add_vertex(1);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Caught expected exception: " << e.what() << '\n';
    }
    std::cout << "Test add_vertex passed.\n";
}

void test_remove_vertex() {
    Graph<int> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.remove_vertex(2);
    if (!graph.has_vertex(2)) {
        std::cout << "Test remove_vertex passed.\n";
    }
    else {
        std::cout << "Test remove_vertex failed.\n";
    }
}

void test_add_edge() {
    Graph<int> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 5.0);
    if (graph.has_edge(1, 2)) {
        std::cout << "Test add_edge passed.\n";
    }
    else {
        std::cout << "Test add_edge failed.\n";
    }
}

void test_remove_edge() {
    Graph<int> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_edge(1, 2, 5.0);
    graph.remove_edge(1, 2);
    if (!graph.has_edge(1, 2)) {
        std::cout << "Test remove_edge passed.\n";
    }
    else {
        std::cout << "Test remove_edge failed.\n";
    }
}

void test_shortest_path() {
    Graph<int> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1.0);
    graph.add_edge(2, 3, 1.0);
    graph.add_edge(1, 3, 2.5);
    std::vector<Graph<int>::Edge> path = graph.shortest_path(1, 3);
    if (path.size() == 2 && path[0].from == 1 && path[0].to == 2 && path[1].from == 2 && path[1].to == 3) {
        std::cout << "Test shortest_path passed.\n";
    }
    else {
        std::cout << "Test shortest_path failed.\n";
    }
}

void test_walk() {
    Graph<int> graph;
    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_edge(1, 2, 1.0);
    graph.add_edge(2, 3, 1.0);
    std::vector<int> visited;
    graph.walk(1, [&visited](const int& v) { visited.push_back(v); std::cout << v << " "; });
    if (visited.size() == 3 && visited[0] == 1 && visited[1] == 2 && visited[2] == 3) {
        std::cout << "\nTest walk passed.\n";
    }
    else {
        std::cout << "\nTest walk failed.\n";
    }
}

int main() {
    test_add_vertex();
    test_remove_vertex();
    test_add_edge();
    test_remove_edge();
    test_shortest_path();
    test_walk();
    return 0;
}