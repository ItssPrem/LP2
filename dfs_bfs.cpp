#include <bits/stdc++.h>    
using namespace std;

void dfsIterative(const vector<vector<int>> &adj, int start) {
    vector<bool> visited(adj.size(), false);
    stack<int> st;
    vector<int> levels(adj.size(), -1);

    st.push(start);
    levels[start] = 0;

    cout << "Node (Level): ";
    while (!st.empty()) {
        int node = st.top();
        st.pop();

        if (!visited[node]) {
            visited[node] = true;
            cout << node << " (" << levels[node] << ") ";

            // Push neighbors in reverse order to mimic recursive DFS
            for (auto it = adj[node].rbegin(); it != adj[node].rend(); ++it) {
                if (!visited[*it]) {
                    st.push(*it);
                    levels[*it] = levels[node] + 1;
                }
            }
        }
    }
    cout << endl;
}

void bfsIterative(vector<vector<int>> &adj, int s) {
    queue<int> q; 
    vector<int> levels(adj.size(), -1); 
    vector<bool> visited(adj.size(), false);

    q.push(s);
    levels[s] = 0; 
    visited[s] = true;

    cout << "Node (Level): ";
    while (!q.empty()) {
        int node = q.front();
        q.pop();

        cout << node << " (" << levels[node] << ") ";

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                levels[neighbor] = levels[node] + 1; 
            }
        }
    }
    cout << endl;
}

void dfsRecursive(vector<vector<int>> &adj, vector<int> &levels, vector<bool> &visited, int node, int level) {
    visited[node] = true;
    levels[node] = level;
    cout << node << " (" << level << ") ";

    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfsRecursive(adj, levels, visited, neighbor, level + 1);
        }
    }
}

void bfsRecursive(vector<vector<int>> &adj, queue<int> &q, vector<bool> &visited, vector<int> &levels) {
    if (q.empty()) {
        return;
    }
    
    int v = q.front();
    q.pop();
    cout << v << " (Level " << levels[v] << ") ";

    for (int u : adj[v]) {
        if (!visited[u]) {
            visited[u] = true;
            levels[u] = levels[v] + 1; 
            q.push(u);
        }
    }

    bfsRecursive(adj, q, visited, levels);
}

void addEdge(vector<vector<int>> &adj, int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}


int main() {
    int V;
    cout << "Enter number of vertices (0-indexed): ";
    cin >> V;

    int e;
    cout << "Enter number of edges: ";
    cin >> e;

    vector<vector<int>> adj(V);

    cout << "Enter pairs of vertices for the edges:\n";
    int u, v;
    for (int i = 0; i < e; i++) {
        cin >> u >> v;
        addEdge(adj, u, v);
    }
    cout << "Adjacency List:\n";
    for (int i = 0; i < adj.size(); i++) {
        cout << i << " -> ";
        for (auto it : adj[i]) {
            cout << it << " ";
        }
        cout << endl;
    }

    cout << "\nChoose a traversal method:\n";
    cout << "1. Iterative DFS with Levels\n";
    cout << "2. Recursive DFS with Levels\n";
    cout << "3. Iterative BFS with Levels\n";
    cout << "4. Recursive BFS with Levels\n";
    cout << "5. Exit\n";

    int choice;
    cin >> choice;

    vector<bool> visited(V, false);
    vector<int> levels(V, -1); 
    vector<int> level(V, 0);
    queue<int> q;

    switch (choice) {
        case 1:
            cout << "Iterative DFS with Levels: ";
            dfsIterative(adj, 0);
            break;

        case 2:
            cout << "Recursive DFS with Levels: ";
            dfsRecursive(adj, levels, visited, 0, 0);
            cout << endl;
            break;

        case 3:
            cout << "Iterative BFS with Levels: ";
            bfsIterative(adj, 0);
            break;

        case 4:
            cout << "Recursive BFS: ";
            visited[0] = true;
            q.push(0);
            bfsRecursive(adj, q, visited, level);
            break;

        case 5:
            break;

        default:
            cout << "Invalid choice!" << endl;
    }

    return 0;
}