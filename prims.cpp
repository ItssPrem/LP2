#include <bits/stdc++.h>
using namespace std;

int spanningTreeByPrims(int V, vector<pair<int, int>> adj[]) {
    int ans = 0;
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq; // {wt, {parent, node}}
    vector<int> vis(V, 0);
    vector<pair<int, int>> mst;
    pq.push({0, {-1, 0}});

    while (!pq.empty()) {
        auto it = pq.top();
        pq.pop();
        int node = it.second.second;
        int wt = it.first;
        int par = it.second.first;
        if (vis[node])
            continue;
        ans += wt;
        vis[node] = 1;
        if (par != -1) {
            mst.push_back({par, node});
        }

        for (auto it : adj[node]) {
            int adjNode = it.first;
            int weight = it.second;
            if (!vis[adjNode]) {
                pq.push({weight, {node, adjNode}});
            }
        }
    }

    cout << "Graph Edges (MST):" << endl;
    for (auto i : mst) {
        cout << i.first << " --> " << i.second << endl;
    }
    cout << endl;
    return ans;
}

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;

    vector<pair<int, int>> adj[V];

    cout << "Enter edges in the format (u v wt):" << endl;
    for (int i = 0; i < E; ++i) {
        int u, v, wt;
        cin >> u >> v >> wt;
        adj[u].push_back({v, wt});
        adj[v].push_back({u, wt}); // Because the graph is undirected
    }

    int totalWeight = spanningTreeByPrims(V, adj);
    cout << "Total weight of Minimum Spanning Tree: " << totalWeight << endl;

    return 0;
}
