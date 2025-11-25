/*
 * File: Cafeteria-Management-System-Simulation.cpp
 * Project: Cafeteria Management System Simulation
 * Description: 
 * A C++ console application simulating a priority-based cafeteria self-service system.
 * Implements various data structures (Priority Queue for priority seating, BST for tray tracking)
 * and Dijkstra's Algorithm for path optimization. Features robust input handling and 
 * real-time service metrics (moving average wait time).
 * * Author: Vansh Arora
 * Date: 25 Oct 25
 */

#include <bits/stdc++.h>
using namespace std;

int readInt(const string &prompt, int lo = INT_MIN, int hi = INT_MAX) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);

        try {
            int val = stoi(s);
            if (val < lo || val > hi) {
                cout << " Please enter a number between " << lo << " and " << hi << ".\n";
                continue;
            }
            return val;
        } catch (...) {
            cout << " Invalid input, try again.\n";
        }
    }
}

string readString(const string &prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        if (s.empty()) {
            cout << " Input cannot be empty.\n";
        } else {
            return s;
        }
    }
}

int CLOCK_TIME = 0;   
int NEXT_TOKEN = 1;  

struct Customer {
    int token;
    string name;
    bool isFaculty;
    int priority;      
    int arrivalTime;

    Customer() {}
    Customer(string n, bool fac, int pr = 0) {
        token = NEXT_TOKEN++;
        name = n;
        isFaculty = fac;
        priority = pr;
        arrivalTime = ++CLOCK_TIME;
    }
};

struct ServedRecord {
    int token;
    string name;
    bool isFaculty;
    int priority;
    int arrivedAt;
    int servedAt;
};

queue<Customer> studentQueue;
priority_queue<Customer, vector<Customer>, function<bool(const Customer&, const Customer&)>> facultyQueue(
    [](const Customer &a, const Customer &b) {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.arrivalTime > b.arrivalTime; 
    }
);

deque<Customer> flexibleQueue;
stack<int> trayStack;
vector<ServedRecord> serviceHistory;
vector<int> lastWaitTimes;

int trayIDCounter = 100;
int TRAYS_AVAILABLE = 30;
int TRAY_CAPACITY = 50;

struct TrayNode {
    int id;
    TrayNode *left, *right;
    TrayNode(int v): id(v), left(nullptr), right(nullptr) {}
};
TrayNode *trayRoot = nullptr;

void insertTray(TrayNode *&root, int id) {
    if (!root) root = new TrayNode(id);
    else if (id < root->id) insertTray(root->left, id);
    else insertTray(root->right, id);
}

bool searchTray(TrayNode *root, int id) {
    if (!root) return false;
    if (root->id == id) return true;
    return id < root->id ? searchTray(root->left, id) : searchTray(root->right, id);
}

void inorderTray(TrayNode *root) {
    if (!root) return;
    inorderTray(root->left);
    cout << root->id << " ";
    inorderTray(root->right);
}

struct Edge { int to, w; };
struct Graph {
    int V;
    vector<vector<Edge>> adj;

    Graph(int v = 0): V(v), adj(v) {}

    void resize(int n) { V = n; adj.assign(n, {}); }

    void addEdge(int u, int v, int w) {
        if (u<0 || u>=V || v<0 || v>=V) return;
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }

    void shortestPaths(int src) {
        const int INF = 1e9;
        vector<int> dist(V, INF);
        dist[src] = 0;

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        pq.push({0, src});

        while(!pq.empty()) {
            auto [d,u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto &e: adj[u]) {
                if (dist[u] + e.w < dist[e.to]) {
                    dist[e.to] = dist[u] + e.w;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        cout << "\n Shortest distances from node " << src << ":\n";
        for (int i=0;i<V;i++) {
            cout << " → To node " << i << " = ";
            if (dist[i] == INF) cout << "Unreachable\n";
            else cout << dist[i] << "\n";
        }
    }
};
Graph cafeteriaGraph;

void recordAndServe(Customer c) {
    if (TRAYS_AVAILABLE <= 0) {
        cout << " No trays available right now. Restock first!\n";
        return;
    }

    int servedAt = ++CLOCK_TIME;
    int waitTime = servedAt - c.arrivalTime;

    int trayId = trayIDCounter++;
    insertTray(trayRoot, trayId);
    trayStack.push(trayId);

    ServedRecord rec{c.token, c.name, c.isFaculty, c.priority, c.arrivalTime, servedAt};
    serviceHistory.push_back(rec);

    TRAYS_AVAILABLE--;

    cout << "\n Served: " << c.name 
         << (c.isFaculty ? " [Faculty]" : " [Student]")
         << " | Token #" << c.token 
         << " | Tray #" << trayId
         << " | Wait Time = " << waitTime << "\n";

    lastWaitTimes.push_back(waitTime);
    if (lastWaitTimes.size() > 5) lastWaitTimes.erase(lastWaitTimes.begin());

    double avg = accumulate(lastWaitTimes.begin(), lastWaitTimes.end(), 0.0) / lastWaitTimes.size();
    cout << " Avg waiting time (last " << lastWaitTimes.size() << "): " << avg << "\n";
}

void setupSampleGraph() {
    cafeteriaGraph.resize(6);
    cafeteriaGraph.addEdge(0,1,3);
    cafeteriaGraph.addEdge(0,2,2);
    cafeteriaGraph.addEdge(1,3,4);
    cafeteriaGraph.addEdge(2,3,2);
    cafeteriaGraph.addEdge(3,4,6);
    cafeteriaGraph.addEdge(2,5,7);
}

void addStudent() {
    string name = readString("Enter student name: ");
    Customer s(name, false);
    studentQueue.push(s);
    flexibleQueue.push_back(s);
    cout << " Student added → Token #" << s.token << "\n";
}

void addFaculty() {
    string name = readString("Enter faculty name: ");
    int pr = readInt("Enter priority (1 low - 20 high): ", 1, 20);
    Customer f(name, true, pr);
    facultyQueue.push(f);
    flexibleQueue.push_front(f);
    cout << " Faculty added → Token #" << f.token << " | Priority " << pr << "\n";
}

void serveNext() {
    if (!facultyQueue.empty()) {
        Customer f = facultyQueue.top(); facultyQueue.pop();
        recordAndServe(f);
    } else if (!studentQueue.empty()) {
        Customer s = studentQueue.front(); studentQueue.pop();
        recordAndServe(s);
    } else {
        cout << " No customers waiting.\n";
    }
}

void returnTray() {
    if (trayStack.empty()) { cout << "No trays returned yet.\n"; return; }
    int id = trayStack.top(); trayStack.pop();
    TRAYS_AVAILABLE++;
    cout << "🔄 Tray #" << id << " returned. Available trays: " << TRAYS_AVAILABLE << "\n";
}

void showQueues() {
    cout << "\n--- Current Queues ---\n";
    cout << "Faculty: " << facultyQueue.size() << " | Students: " << studentQueue.size() << "\n";
}

void printMenu() {
    cout << "\n====== CAFETERIA MENU ======\n";
    cout << "1. Add Student\n";
    cout << "2. Add Faculty\n";
    cout << "3. Serve Next (faculty first)\n";
    cout << "4. Return Tray\n";
    cout << "5. Show Queues\n";
    cout << "6. Show Tray Records\n";
    cout << "7. Search Tray ID\n";
    cout << "8. Cafeteria Shortest Paths\n";
    cout << "0. Exit\n";
}

int main() {
    cout << "🍴 Welcome to Cafeteria Self-Service System!\n";
    TRAY_CAPACITY = readInt("Enter max tray capacity: ", 1, 500);
    TRAYS_AVAILABLE = readInt("Enter starting available trays: ", 0, TRAY_CAPACITY);

    setupSampleGraph();

    while (true) {
        printMenu();
        int ch = readInt("Your choice: ", 0, 8);
        switch(ch) {
            case 1: addStudent(); break;
            case 2: addFaculty(); break;
            case 3: serveNext(); break;
            case 4: returnTray(); break;
            case 5: showQueues(); break;
            case 6: inorderTray(trayRoot); cout << "\n"; break;
            case 7: {
                int id = readInt("Enter tray ID to search: ", 0, 9999);
                cout << (searchTray(trayRoot, id) ? " Found\n" : " Not found\n");
                break;
            }
            case 8: {
                int src = readInt("Enter start node (0-5): ", 0, 5);
                cafeteriaGraph.shortestPaths(src);
                break;
            }
            case 0: cout << "Exiting. Goodbye!\n"; return 0;
        }
    }
}
