#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

typedef long long ll;
const ll INF = 2e18;

struct position {
    int y, x;

    bool operator<(const position& o) const {
        if (y != o.y) return y < o.y;
        return x < o.x;
    }

    bool operator==(const position& o) const {
        return y == o.y and x == o.x;
    }
};

template<>
struct hash<position> {
    size_t operator()(const position& pos) const {
        // Szudzik pair hashing function
        return pos.x < pos.y ? pos.y * pos.y + pos.x : pos.x * pos.x + pos.x + pos.y;
    }
};

/* entry struct that shows the cost of going to the node in x, y position with cost weight */
struct entry {
    position pos;
    ll weight;

    bool operator>(const entry& o) const {
        return weight > o.weight;
    }
};

/* node struct that stores the value of the node and the vertices that it is connected to */
struct node {
    position pos;
    ll value;
    vector<entry> adj;
    bool used;

    node() = default;

    node(const position& pos, ll v) : pos(pos), value(v), adj(vector<entry>()), used(false) { }
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

unordered_map<position, node> position_to_node;
string v, w;
int n, m;

inline node* get_node(const position& pos) {
    if (position_to_node.find(pos) != position_to_node.end()) return &position_to_node[pos];
    return nullptr;
}

inline void expand_node(const position& pos) {
    if (get_node(pos) != nullptr) return;

    // cout << "expanding " << pos.y << " " << pos.x << "\n";
    node node_(pos, INF);

    if (pos.y < n - 1)
        node_.adj.push_back({position {pos.y + 1, pos.x}, 1});

    if (pos.x < m - 1)
        node_.adj.push_back({position {pos.y, pos.x + 1}, 1});

    if (pos.y < n - 1 and pos.x < m - 1)
        node_.adj.push_back({position {pos.y + 1, pos.x + 1}, (v[pos.y + 1] == w[pos.x + 1] ? 0 : 1)});

    position_to_node[pos] = node_;
}

void find_shortest_path(const position& from, const position& to) {

    /* implementation of Dijkstra algorithm for finding the shortest path from starting to target position */
    priority_queue<entry, vector<entry>, greater<entry>> pq;
    expand_node(from);
    pq.push({from, 0});

    while (!pq.empty()) {

        entry e = pq.top();
        pq.pop();
        // cout << "popped " << e.pos.y << " " << e.pos.x << " " << e.weight << "\n";
        node& node_ = *get_node(e.pos);
        if (node_.used) continue;
        node_.used = true;
        node_.value = e.weight;

        if (e.pos == to)
            break;

        // cout << node_.pos.y << " " << node_.pos.x << ": ";
        for (const entry& p : node_.adj) {
            // cout << "passing with " << p.pos.y << " " << p.pos.x << "\n";
            expand_node(p.pos);
            if (!get_node(p.pos)->used) {
                // cout << "insertin " << p.y << " " << p.x << " w:" << p.weight + e.weight << "\n";
                pq.push({p.pos, p.weight + e.weight});
            }
        }
    }
}

int main() {

    freopen("test", "r", stdin);

    /* reading the input */
    vector<string> tokens = read_input();
    v = tokens[0];
    w = tokens[1];
    v = ' ' + v;
    w = ' ' + w;

    n = (int) v.size();
    m = (int) w.size();

    cout << n << ":" << v << ", " << m << ":" << w << "\n";

    find_shortest_path(position {0, 0}, position {n - 1, m - 1});

    cout << get_node(position {n - 1, m - 1})->value << "\n";

    return 0;
}