#include <iostream>
#include <vector>
#include <queue>
#include <stack>


using namespace std;

// Structure to represent a node in the grid
struct Node {
    int x, y;
    int g, h, f;
    Node* parent;

    Node(int x, int y, int g = 0, int h = 0, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

// Comparison operator for the priority queue
struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

// Directions for movement (up, down, left, right)
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

// Function to calculate Manhattan distance
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Function to print the open list
void printOpenList(priority_queue<Node*, vector<Node*>, CompareNode> openList) {
    cout << "\n Open List:\n";
    while (!openList.empty()) {
        Node* node = openList.top();
        openList.pop();
        cout << "(" << node->x << ", " << node->y << ") f=" << node->f << " ";
    }
    cout << "\n";
}

// Function to print the closed list
void printClosedList(const vector<vector<bool>>& closedList) {
    cout << " Closed List:\n";
    for (const auto& row : closedList) {
        for (bool val : row) {
            cout << (val ? "X " : ". ");
        }
        cout << endl;
    }
}

// A* algorithm function
bool aStar(vector<vector<char>>& grid, pair<int, int> start, pair<int, int> goal) {
    int rows = grid.size();
    int cols = grid[0].size();

    priority_queue<Node*, vector<Node*>, CompareNode> openList;
    vector<vector<bool>> closedList(rows, vector<bool>(cols, false));

    Node* startNode = new Node(start.first, start.second, 0, heuristic(start.first, start.second, goal.first, goal.second));
    openList.push(startNode);

    while (!openList.empty()) {
        printOpenList(openList);  // Print open list

        Node* current = openList.top();
        openList.pop();

        if (closedList[current->x][current->y]) continue;

        // Mark the node as visited
        closedList[current->x][current->y] = true;
        printClosedList(closedList); // Print closed list

        // Check if the goal is reached
        if (current->x == goal.first && current->y == goal.second) {
            // Trace the path
            Node* temp = current;
            while (temp != nullptr) {
                if (grid[temp->x][temp->y] != 'S' && grid[temp->x][temp->y] != 'G')
                    grid[temp->x][temp->y] = '*';
                temp = temp->parent;
            }
            return true;
        }

        // Explore neighbors
        for (auto dir : directions) {
            int newX = current->x + dir.first;
            int newY = current->y + dir.second;

            // Check boundaries and obstacles
            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols &&
                grid[newX][newY] != '#' && !closedList[newX][newY]) {
                
                int g = current->g + 1;
                int h = heuristic(newX, newY, goal.first, goal.second);
                Node* neighbor = new Node(newX, newY, g, h, current);
                openList.push(neighbor);
            }
        }
    }

    return false; // No path found
}

// Function to print the grid
void printGrid(const vector<vector<char>>& grid) {
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

int main() {
    int rows, cols;
    cout << "Enter number of rows and columns: ";
    cin >> rows >> cols;

    // Initialize the grid with '.'
    vector<vector<char>> grid(rows, vector<char>(cols, '.'));

    // Get start position
    pair<int, int> start;
    cout << "Enter start coordinates (row and column): ";
    cin >> start.first >> start.second;
    grid[start.first][start.second] = 'S';

    // Get goal position
    pair<int, int> goal;
    cout << "Enter goal coordinates (row and column): ";
    cin >> goal.first >> goal.second;
    grid[goal.first][goal.second] = 'G';

    // Get number of obstacles
    int numObstacles;
    cout << "Enter number of obstacles: ";
    cin >> numObstacles;

    cout << "Enter obstacle coordinates (row and column) one by one:\n";
    for (int i = 0; i < numObstacles; ++i) {
        int x, y;
        cin >> x >> y;
        if ((x == start.first && y == start.second) || (x == goal.first && y == goal.second)) {
            cout << "Cannot place obstacle on start or goal.\n";
            continue;
        }
        grid[x][y] = '#';
    }

    cout << "\nInitial Grid:\n";
    printGrid(grid);

    // Run the A* algorithm
    if (aStar(grid, start, goal)) {
        cout << "\nPath Found:\n";
        printGrid(grid);
    } else {
        cout << "\nNo Path Found!\n";
    }

    return 0;
}
