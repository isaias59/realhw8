#include "graph.h"
#include <vector>
#include <unordered_set>

void error(string msg) {
    cerr << "Error: " << msg << endl;
    exit(EXIT_FAILURE);
}

DisjointSet::DisjointSet(int numVertices) {
    subsets.resize(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        makeSet(i);
    }
}

void DisjointSet::makeSet(Vertex x) {
    subsets[x].parent = x;
}

Vertex DisjointSet::findSet(Vertex x) {
    if (subsets[x].parent != x) {
        subsets[x].parent = findSet(subsets[x].parent);
    }
    return subsets[x].parent;
}

void DisjointSet::unionSets(Vertex x, Vertex y) {
    Vertex rootX = findSet(x);
    Vertex rootY = findSet(y);
    if (rootX != rootY) {
        subsets[rootX].parent = rootY;
    }
}

Graph Graph::sort_edges() const {
    Graph sorted_graph = *this;
    sort(sorted_graph.begin(), sorted_graph.end(), [](const Edge& a, const Edge& b) {
        return a.weight < b.weight;
        });
    return sorted_graph;
}
///////////////////////////////////////////////

/*VertexList Graph::edges_from(Vertex vertex) const {
    VertexList vertices;
    for (const auto& edge : *this) {
        if (edge.u == vertex) {
            vertices.push_back(edge.v);
        }
        else if (edge.v == vertex) {
            vertices.push_back(edge.u);
        }
    }
    return vertices;
}
*/
////////////////////////////////////////////////////
VertexList Graph::edges_from(Vertex vertex) const {
    std::unordered_set<Vertex> unique_vertices;

    for (const auto& edge : *this) {
        if (edge.u == vertex) {
            unique_vertices.insert(edge.v);
        }
        else if (edge.v == vertex) {
            unique_vertices.insert(edge.u);
        }
    }

    // Convert set to sorted list (if ordering is required)
    VertexList vertices(unique_vertices.begin(), unique_vertices.end());
    std::sort(vertices.begin(), vertices.end());  // Ensures predictable output order

    return vertices;
}


EdgeList Kruskals(const Graph& G) {
    EdgeList mst;
    DisjointSet ds(G.numVertices);
    Graph sorted_edges = G.sort_edges();

    for (const auto& edge : sorted_edges) {
        if (ds.findSet(edge.u) != ds.findSet(edge.v)) {
            mst.push_back(edge);
            ds.unionSets(edge.u, edge.v);
        }
    }
    return mst;
}

int sum_weights(const EdgeList& L) {
    return accumulate(L.begin(), L.end(), 0, [](int sum, const Edge& e) {
        return sum + e.weight;
        });
}

void file_to_graph(string filename, Graph& G) {
    ifstream file(filename);
    if (!file) {
        error("File not found: " + filename);
    }
    file >> G;
}

VertexList dfs(const Graph& graph, Vertex startVertex) {
    VertexList visited;
    stack<Vertex> s;
    s.push(startVertex);

    while (!s.empty()) {
        Vertex v = s.top();
        s.pop();
        if (find(visited.begin(), visited.end(), v) == visited.end()) {
            visited.push_back(v);
            for (auto&& neighbor : graph.edges_from(v)) {
                s.push(neighbor);
            }
        }
    }
    return visited;
}
///////////////////////////////////////////////////////////////
/*VertexList bfs(const Graph& graph, Vertex startVertex) {
    VertexList visited;
    queue<Vertex> q;
    q.push(startVertex);

    while (!q.empty()) {
        Vertex v = q.front();
        q.pop();
        if (find(visited.begin(), visited.end(), v) == visited.end()) {
            visited.push_back(v);
            for (auto&& neighbor : graph.edges_from(v)) {
                q.push(neighbor);
            }
        }
    }
    return visited;
}
*/
///////////////////////////////////////////////////////////////////////
VertexList bfs(const Graph& graph, Vertex startVertex) {
    VertexList visited;
    unordered_set<Vertex> visited_set;
    queue<Vertex> q;

    q.push(startVertex);
    visited_set.insert(startVertex);

    while (!q.empty()) {
        Vertex v = q.front();
        q.pop();
        visited.push_back(v);

        VertexList neighbors = graph.edges_from(v);
        sort(neighbors.begin(), neighbors.end());  // Ensure smallest vertex is visited first

        for (const auto& neighbor : neighbors) {
            if (visited_set.find(neighbor) == visited_set.end()) {
                q.push(neighbor);
                visited_set.insert(neighbor);
            }
        }
    }
    return visited;
}

string get_arg(int argc, char* argv[], string def) {
    if (argc < 2) {
        return def;
    }
    return argv[1];
}
