#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <cmath>
#include <map>
#include <unordered_set>

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

// template<>
// struct hash<position> {
//     size_t operator()(const position& pos) const {
//         // TODO: Bad hashing function, change it
//         return pos.x < pos.y ? pos.y * pos.y + pos.x : pos.x * pos.x + pos.x + pos.y;
//     }
// };

/* entry struct that shows the cost of going to the node in x, y position with cost weight */
struct entry {
    position pos;
    ll g_value, h_value;

    bool operator>(const entry& o) const {
        return g_value + h_value > o.g_value + o.h_value;
    }
};

struct edge {
    position pos;
    ll weight;
};

/* node struct that stores the value of the node and the vertices that it is connected to */
struct node {
    position pos;
    ll value;
    vector<edge> adj;

    node() = default;

    node(const position& pos, ll v) : pos(pos), value(v), adj(vector<edge>()) { }
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

map<position, node> position_to_node;
vector<ll> suffix_non_matches_along_x, suffix_non_matches_along_y;
string v, w;
int n, m;

inline ll h_function(const position& pos) {
    ll value = 0;
    if (pos.x + 1 < m)
        value = max(value, suffix_non_matches_along_x[pos.x + 1]);
    
    if (pos.y + 1 < n)
        value = max(value, suffix_non_matches_along_y[pos.y + 1]);

    // value = max(value, (ll) abs(pos.x - pos.y));
        
    return value;
}

vector<ll> compute_suffix_non_matches(const string& str1, const string& str2, int k) {
    int len1 = (int) str1.size(), len2 = (int) str2.size();
    unordered_set<string> kmers;
    // generate all kmers of v
    for (int i = 0; i < len1; ++i) {
        for (int j = 1; j <= len1 - i; ++j) {
            kmers.insert(str1.substr(i, j));
            // cout << str1.substr(i, j) << "\n";
        }
    }

    vector<ll> non_matches(len2, 0), suffix_non_matches(len2, 0);
    for (int i = 0; i <= len2 - k; i += k) {
        // cout << "checking " << str2.substr(i, k) << " ";
        if (kmers.find(str2.substr(i, k)) == kmers.end()) {
            non_matches[i] = 1;
            // cout << "not found";
        }
        // cout << "\n";
    }
    suffix_non_matches[len2 - 1] = 0;
    for (int i = len2 - 2; i >= 0; --i) {
        suffix_non_matches[i] = suffix_non_matches[i + 1] + non_matches[i];
    }

    // for (int i = 0; i < len2; ++i)
    //     cout << suffix_non_matches[i] << " ";
    // cout << "\n";

    return suffix_non_matches;
}

inline node* get_node(const position& pos) {
    if (position_to_node.find(pos) != position_to_node.end()) return &position_to_node[pos];
    return nullptr;
}

inline void explore_node(const position& pos) {
    if (get_node(pos) != nullptr) return;

    node node_(pos, INF);

    if (pos.y < n - 1)
        node_.adj.push_back({position {pos.y + 1, pos.x}, 1});

    if (pos.x < m - 1)
        node_.adj.push_back({position {pos.y, pos.x + 1}, 1});

    if (pos.y < n - 1 and pos.x < m - 1)
        node_.adj.push_back({position {pos.y + 1, pos.x + 1}, (v[pos.y + 1] == w[pos.x + 1] ? 0 : 1)});

    position_to_node[pos] = node_;
}

void find_shortest_path(const position& from, const position& to, function<ll(const position&)> heuristic) {

    /* implementation of A* algorithm for finding the shortest path from starting to target position */
    priority_queue<entry, vector<entry>, greater<entry>> pq;
    explore_node(from);
    pq.push({from, 0, heuristic(position {0, 0})});

    while (!pq.empty()) {

        entry e = pq.top();
        pq.pop();
        assert(e.pos.x < m and e.pos.y < n);
        // cout << "popped " << e.pos.y << " " << e.pos.x << " " << e.weight << "\n";
        // cout << "popped " << e.pos.y << " " << e.pos.x << " g:" << e.g_value << ", h:" << e.h_value << "\n";
        node& node_ = *get_node(e.pos);
        if (e.g_value >= node_.value) continue;
        node_.value = e.g_value;

        if (e.pos == to)
            break;

        for (const edge& p : node_.adj) {
            // cout << "insert " << p.pos.y << " " << p.pos.x << ", g:" << e.g_value + p.weight << ", h:" << heuristic(p.pos) << "\n";
            explore_node(p.pos);
            pq.push({p.pos, e.g_value + p.weight, heuristic(p.pos)});
            // }
        }
    }
}

void align_with_heuristic() {
    freopen("output_a", "w", stdout);
    position_to_node.clear();

    cout << v << " " << w << "\n";

    suffix_non_matches_along_x = compute_suffix_non_matches(v, w, max(1, (int) log(m)));
    suffix_non_matches_along_y = compute_suffix_non_matches(w, v, max(1, (int) log(n)));

    find_shortest_path(position {0, 0}, position {n - 1, m - 1}, h_function);
}

void align_without_heuristic() {
    freopen("output_dij", "w", stdout);
    position_to_node.clear();

    cout << v << " " << w << "\n";

    find_shortest_path(position {0, 0}, position {n - 1, m - 1}, [] (const position& pos) { return 0; } );
}

int main() {

    freopen("test", "r", stdin);
    // freopen("output_a*", "w", stdout);
    

    /* reading the input */
    vector<string> tokens = read_input();
    v = tokens[0];
    w = tokens[1];
    v = ' ' + v;
    w = ' ' + w;

    n = (int) v.size();
    m = (int) w.size();

    align_with_heuristic();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            const node* node_ = get_node(position {i, j});
            if (node_ && node_->value != INF) cout << node_->value << " ";
            else cout << "- ";
        }
        cout << "\n";
    }

    align_without_heuristic();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            const node* node_ = get_node(position {i, j});
            if (node_ && node_->value != INF) cout << node_->value << " ";
            else cout << "- ";
        }
        cout << "\n";
    }

    return 0;
}