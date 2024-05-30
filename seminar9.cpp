#include <vector>
#include <iostream>

class DSU {
public:
    explicit DSU(size_t size) {
        rank.resize(size, 1);
        parent.resize(size, 1);
        for (size_t i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }

    int find_set(int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set (parent[v]);
    }

    void union_set(int v, int u) {
        int v_set = find_set(v);
        int u_set = find_set(u);
        if (v_set == u_set) {
            return;
        }
        if (rank[v_set] > rank[u_set]) {
            parent[u_set] = v_set;
            rank[u_set] += rank[v_set];
        } else {
            parent[v_set] = u_set;
            rank[v_set] += rank[u_set];
        }
    }
private:
    std::vector<int> parent;
    std::vector<int> rank;
};

struct Edge
{
    int from;
    int to;
    int weight;
};

void kruskal(std::vector <Edge>& edges, size_t graph_size, void (*callback)(const Edge&)) {
    DSU dsu(graph_size);

    std::sort(edges.begin(), edges.end(), [](const Edge& l, const Edge& r) {
        return l.weight < r.weight;
    });

    for (const Edge& edge : edges) {
        int v = dsu.find_set(edge.from);
        int u = dsu.find_set(edge.to);

        if (v == u) {
            continue;
        }

        dsu.union_set(v, u);
        callback(edge);
    }
}

int main() {
    std::vector <Edge> edges = {
        { 2, 3, 3 },
        { 1, 6, 4 },
        { 5, 7, 5 },
        { 6, 8, 5 },
        { 7, 8, 5 },
        { 2, 4, 1 },
        { 3, 4, 1 },
        { 1, 3, 2 },
        { 0, 3, 6 },
        { 4, 7, 6 },
        { 3, 6, 7 },
        { 0, 1, 9 },
        { 3, 5, 9 },
        { 5, 6, 1 },
        { 5, 8, 1 },
        { 0, 2, 2 }
    };
    kruskal(edges, 10, [](const Edge& e) {
        std::cout << "(" << e.from << ", " << e.to << ")" << std::endl;
    });
}
