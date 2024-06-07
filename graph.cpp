#pragma once
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <stdexcept>
#include <queue>
#include <functional>
#include <iterator>

template<typename Vertex, typename Distance = double>
class Graph {
public:
    struct Edge {
        Vertex from;
        Vertex to;
        Distance distance;
        bool operator==(const Edge& other) const {
            return from == other.from && to == other.to && distance == other.distance;
    };

    bool has_vertex(const Vertex& v) const {
        return _vertices.count(v) > 0;
    }
    void add_vertex(const Vertex& v) {
        if (has_vertex(v)) 
            throw std::invalid_argument("Vertex already exists in the graph");
        _vertices.insert(v);
        _edges.emplace(v, std::vector<Edge>{});
    }
    bool remove_vertex(const Vertex& v) {
        if (!has_vertex(v)) return false;
        _vertices.erase(v);
        _edges.erase(v);
        for (auto& kv : _edges) {
            auto& list_edge = kv.second;
            list_edge.erase(std::remove_if(list_edge.begin(), list_edge.end(),
                [v](const Edge& e) { return e.to == v; }), list_edge.end());
        }
        return true;
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
        if (!has_vertex(from) || !has_vertex(to))
            throw std::out_of_range("Vertex doesn't exist in the graph");
        if(d>0)
            _edges[from].push_back({ from, to, d });
        else
            throw std::out_of_range("Distance must be positive");
    }
    bool remove_edge(const Vertex& from, const Vertex& to) {
        if (!has_vertex(from) || !has_vertex(to))
            return false;
        auto& list = _edges[from];
        auto it = std::remove_if(list.begin(), list.end(),
            [to](const Edge& e) { return e.to == to; });
        if (it != list.end()) {
            list.erase(it, list.end());
            return true;
        }
        return false;
    }
    bool remove_edge(const Edge& e) {
        if (!has_vertex(e.from) || !has_vertex(e.to))
            return false;
        auto& list = _edges[e.from];
        auto it = std::remove_if(list.begin(), list.end(),
            [e](const Edge& edge) { return edge.from == e.from && edge.to == e.to; });
        if (it != list.end()) {
            list.erase(it, list.end());
            return true;
        }
        return false;
    }
    bool has_edge(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to))
            return false;
        const auto& edges = _edges.at(from);
        return std::find_if(edges.begin(), edges.end(),
            [to](const Edge& e) { return e.to == to; }) != edges.end();
    }
    bool has_edge(const Edge& e) {
        if (!has_vertex(e.from) || !has_vertex(e.to))
            return false;
        const auto& edges = _edges.at(e.from);
        return std::find(edges.begin(), edges.end(), e) != edges.end();
    }

    size_t order() const {
        return _vertices.size();
    }
    size_t degree(const Vertex& v) const {
        if (!has_vertex(v))
            throw std::invalid_argument("Vertex doesn't exist in the graph");

        size_t degree = _edges.at(v).size();
        for (const auto& kv : _edges) {
            if (kv.first != v) {
                for (const Edge& e : kv.second) {
                    if (e.to == v)
                        degree++;
                }
            }
        }
        return degree;
    }

    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to))
            throw std::invalid_argument("Vertex does not exist in the graph");

        std::unordered_map<Vertex, Distance> distance;
        std::unordered_map<Vertex, Edge> predecessor;
        std::set<std::pair<Distance, Vertex>> pq;

        for (const Vertex& v : _vertices) {
            distance[v] = std::numeric_limits<Distance>::infinity();
            predecessor[v] = Edge();
        }

        distance[from] = Distance();
        pq.insert({ Distance(), from });

        while (!pq.empty()) {
            Vertex u = pq.begin()->second;
            pq.erase(pq.begin());

            for (const Edge& e : _edges.at(u)) {
                Vertex v = e.to;
                Distance weight = e.distance;
                if (distance[u] + weight < distance[v]) {
                    pq.erase({ distance[v], v });
                    distance[v] = distance[u] + weight;
                    predecessor[v] = e;
                    pq.insert({ distance[v], v });
                }
            }
        }

        std::vector<Edge> path;
        for (Vertex v = to; predecessor[v].from != Vertex(); v = predecessor[v].from) {
            path.push_back(predecessor[v]);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
    void walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action) {
        if (!has_vertex(start_vertex))
            return;

        std::queue<Vertex> queue;
        std::unordered_set<Vertex> visited;

        queue.push(start_vertex);
        visited.insert(start_vertex);

        while (!queue.empty()) {
            Vertex current = queue.front();
            queue.pop();
            action(current);

            for (const Edge& edge : _edges.at(current)) {
                if (visited.find(edge.to) == visited.end()) {
                    queue.push(edge.to);
                    visited.insert(edge.to);
                }
            }
        }
    }
    std::unordered_set<Vertex> vertices() const {
        return _vertices; 
    }
private:
    std::unordered_set<Vertex> _vertices;
    std::unordered_map<Vertex, std::vector<Edge>> _edges;
    std::vector<Edge> edges(const Vertex& vertex) {
        return _edges[vertex];
    }
};