#include <iostream>
#include <vector>
#include <queue>
#include <deque>

using namespace std;

/* entry struct that shows the cost of going to the node in x, y position with cost weight */
struct entry {
    int y, x, weight;

    bool operator>(const entry& o) const {
        return weight > o.weight;
    }
};

/* node struct that stores the value of the node and the vertices that it is connected to */
struct node {
    int y, x, value;
    vector<entry> adj;
    bool used;

    node(int y, int x, int v=0) : y(y), x(x), value(v), adj(vector<entry>()), used(false) { }
};

vector<string> read_input() {
	string temp;
	vector<string> tokens;

	while (cin >> temp) {
		if (temp[0] == '>') {
			tokens.emplace_back();
			cin >> temp;
		}
		tokens.back() += temp;
	}
	return tokens;
}

vector<deque<node>> graph;
string v, w;
int n, m;

inline node* get_node(int y, int x) {
    if (graph[y].empty()) return nullptr;
    if (graph[y].front().x > x or graph[y].back().x < x) return nullptr;
    return &graph[y][x - graph[y].front().x];
}

inline bool is_node_used(int y, int x) {
    if (graph[y].empty() or graph[y].front().x > x or graph[y].back().x < x) {
        cout << "WARNING: Not appropriate node is requested " << y << " " << x << "\n";
        return false;
    }
    return graph[y][x - graph[y].front().x].used;
}

inline void set_node_used(int y, int x) {
    graph[y][x - graph[y].front().x].used = true;
}

void explore_node_connections(int y, int x) {
    node* node_ = get_node(y, x);
    if (y < n - 1)
        node_->adj.push_back({y + 1, x, 1});

    if (x < m - 1)
        node_->adj.push_back({y, x + 1, 1});

    if (y < n - 1 and x < m - 1)
        node_->adj.push_back({y + 1, x + 1, (v[y + 1] == w[x + 1] ? 0 : 1)});
}

inline void expand_node_connections(int y, int x) {
    // cout << "expand " << y << " " << x << "\n";
    if (graph[y].empty()) {
        // cout << "adding " << y << " " << x << "\n";
        graph[y].push_front(node(y, x));
        explore_node_connections(y, x);
    }

    while (graph[y].front().x > x) {
        // cout << "adding " << y << " " << graph[y].front().x - 1 << "\n";
        int new_x = graph[y].front().x - 1;
        graph[y].push_front(node(y, new_x));
        explore_node_connections(y, new_x);
    }

    while (graph[y].back().x < x) {
        // cout << "adding " << y << " " << graph[y].front().x + 1 << "\n";
        int new_x = graph[y].back().x + 1;
        graph[y].push_back(node(y, new_x));
        explore_node_connections(y, new_x);
    }

    // cout << "size of the added node " << get_node(y + 1, x + 1)->adj.size() << "\n";
}

int main() {

    freopen("test", "r", stdin);

    /* reading the input */
    vector<string> tokens = read_input();
    v = tokens[0];
    w = tokens[1];
    v = ' ' + v;
    w = ' ' + w;

    cout << v << " " << w << "\n";

    n = (int) v.size();
    m = (int) w.size();

    graph.assign(n, deque<node>());

    /* implementation of Dijkstra algorithm for finding the shortest path from (0, 0) to (n - 1, m - 1) */
    priority_queue<entry, vector<entry>, greater<entry>> pq;
    expand_node_connections(0, 0);
    pq.push({0, 0, 0});

    while (!pq.empty()) {

        entry e = pq.top();
        // cout << "popped " << e.y << " " << e.x << " " << e.weight << "\n";
        pq.pop();
        if (is_node_used(e.y, e.x)) continue;
        set_node_used(e.y, e.x);

        node& node_ = *get_node(e.y, e.x);

        node_.value = e.weight;
        if (e.y == n - 1 && e.x == m - 1)
            break;

        for (const entry& p : node_.adj) {
            expand_node_connections(p.y, p.x);
            // cout << "consider " << p.y << " " << p.x << "\n";
            // cout << "passing with " << p.y << " " << p.x << "\n";
            if (!is_node_used(p.y, p.x)) {
                // cout << "insertin " << p.y << " " << p.x << " w:" << p.weight + e.weight << "\n";
                pq.push({p.y, p.x, p.weight + e.weight});
            }
        }

        // for (int i = 0; i < n; ++i) {
        //     cout << "i: " << i << " " << graph[i].size() << "\n";
        //     for (const node& nod : graph[i])
        //         cout << nod.y << " " << nod.x << " " << nod.value << ", ";
        //     cout << "\n";
        // }
    }

    cout << get_node(n - 1, m - 1)->value << "\n";

    return 0;
}