#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, weight;
};

bool compare(Edge a, Edge b) {
    return a.weight < b.weight;
}

int find(vector<int>& parent, int i) {
    if (parent[i] != i)
        parent[i] = find(parent, parent[i]);
    return parent[i];
}

void Union(vector<int>& parent, vector<int>& rank, int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    if (rank[xroot] < rank[yroot])
        parent[xroot] = yroot;
    else if (rank[xroot] > rank[yroot])
        parent[yroot] = xroot;
    else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

vector<Edge> kruskalMST(vector<Edge>& edges, int V) {
    sort(edges.begin(), edges.end(), compare);
    vector<int> parent(V), rank(V, 0);
    for (int i = 0; i < V; i++)
        parent[i] = i;

    vector<Edge> result;

    for (auto& edge : edges) {
        int x = find(parent, edge.u);
        int y = find(parent, edge.v);
        if (x != y) {
            result.push_back(edge);
            Union(parent, rank, x, y);
        }
    }
    return result;
}

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;
    vector<Edge> edges(E);

    cout << "Enter edges (u v weight):\n";
    for (int i = 0; i < E; ++i)
        cin >> edges[i].u >> edges[i].v >> edges[i].weight;

    vector<Edge> mst = kruskalMST(edges, V);
    cout << "Edges in MST:\n";
    for (auto& e : mst)
        cout << e.u << " - " << e.v << " : " << e.weight << endl;
    return 0;
}