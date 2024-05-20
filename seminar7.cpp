#include <iostream>
#include <vector>
#include <queue>

struct IGraph
{
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph
{
public:
    explicit ListGraph(int n)
    {
        graph.resize(n);
    }

    ListGraph(const IGraph &);

    virtual ~ListGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) override
    {
        graph[from].push_back(to);
        // graph_reversed[to].push_back(from);
    }

    virtual int VerticesCount() const override
    {
        return graph.size();
    }

    virtual std::vector<int> GetNextVertices(int vertex) const override
    {
        std::vector<int> result;
        // std::vector<int> result(graph[vertex].begin(), graph[vertex].end());
        // std::copy(graph[vertex].begin(), graph[vertex].end(), result.begin());

        result.reserve(graph[vertex].size());
        for (int to : graph[vertex])
        {
            result.push_back(to);
        }

        return result;
    }

    virtual std::vector<int> GetPrevVertices(int vertex) const override
    {
        std::vector<int> result;
        for (int from = 0; from < graph.size(); ++from)
        {
            for (int to : graph[from])
            {
                if (to == vertex)
                {
                    result.push_back(to);
                }
            }
        }
        return result;
    }

private:
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> graph_reversed;
};

void _dfs_internal(const IGraph &graph, std::vector<bool> &visited, int v, void (*callback)(int v))
{
    visited[v] = true;
    callback(v);

    std::vector<int> children = graph.GetNextVertices(v);
    for (int child : children)
    {
        if (!visited[child])
        {
            _dfs_internal(graph, visited, child, callback);
        }
    }
}

void dfs(const IGraph &graph, void (*callback)(int v))
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int v = 0; v < graph.VerticesCount(); ++v)
    {
        if (!visited[v])
        {
            _dfs_internal(graph, visited, v, callback);
        }
    }
}

void bfs(const IGraph &graph, void (*callback)(int v))
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> q;

    for (int v = 0; v < graph.VerticesCount(); ++v)
    {
        if (!visited[v])
        {
            visited[v] = true;
            q.push(v);
            while (!q.empty())
            {
                int cur_v = q.front();
                q.pop();

                callback(cur_v);

                std::vector<int> children = graph.GetNextVertices(cur_v);
                for (int child : children)
                {
                    if (!visited[child])
                    {
                        visited[child] = true;
                        q.push(child);
                    }
                }
            }
        }
    }
}

int main()
{
    std::cout << "Begin" << std::endl;
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 0);

    graph.AddEdge(1, 2);
    graph.AddEdge(2, 1);

    graph.AddEdge(2, 3);
    graph.AddEdge(3, 2);

    graph.AddEdge(2, 4);
    graph.AddEdge(4, 2);

    graph.AddEdge(3, 4);
    graph.AddEdge(4, 3);

    graph.AddEdge(1, 5);
    graph.AddEdge(5, 1);

    // MatrixGraph mg(graph);

    dfs(graph, [](int v)
        { std::cout << "[dfs] v=" << v << std::endl; });

    std::cout << "-------------" << std::endl;
    bfs(graph, [](int v)
        { std::cout << "[bfs] v=" << v << std::endl; });
    return 0;
}
