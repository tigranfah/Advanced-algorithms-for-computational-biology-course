#include <iostream>
#include <vector>

using namespace std;

/* entry struct that shows the cost of going to the node in x, y position with cost weight */
struct entry {
    int x, y, weight;

    bool operator<(const entry& o) const {
        return weight < o.weight;
    }
};

/* node struct that stores the value of the node and the vertices that it is connected to */
struct node {
    int value;
    vector<entry> adj;
};

/* PriorityQueue class, standart implementation of priority queue using heap */
template<typename T>
class PriorityQueue {
private:
    vector<T> m_Heap;

    void upheap(int index) {
        while (index != 0 && m_Heap[index] < m_Heap[index / 2]) {
            swap(m_Heap[index], m_Heap[index / 2]);
            index /= 2;
        }
    }

    void downheap(int index) {
        while (index < (int) m_Heap.size()) {
            int min_index = index;
            int left_child_index = index * 2;
            int right_child_index = index * 2 + 1;

            if (left_child_index < (int) m_Heap.size() && m_Heap[left_child_index] < m_Heap[min_index])
                min_index = left_child_index;

            if (right_child_index < (int) m_Heap.size() && m_Heap[right_child_index] < m_Heap[min_index])
                min_index = right_child_index;

            if (min_index == index) break;
            swap(m_Heap[index], m_Heap[min_index]);
            index = min_index;
        }
    }

public:

    int size() const { return (int) m_Heap.size(); }

    bool empty() const { return m_Heap.empty(); }

    void insert(T elem) {
        m_Heap.push_back(elem);
        upheap((int) m_Heap.size() - 1);
    }

    T min() {
        if (empty()) {
            cout << "WARNING: the priority queue is empty and cannot return min value.\n";
            return T {};
        }
        return m_Heap[0];
    }

    T pop_min() {
        if (empty()) {
            cout << "WARNING: the priority queue is empty and cannot return min value.\n";
            return T {};
        }
        T temp = m_Heap[0];
        swap(m_Heap[0], m_Heap[(int) m_Heap.size() - 1]);
        m_Heap.pop_back();
        downheap(0);
        return temp;
    }
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

int main() {

    /* reading the input */
    vector<string> tokens = read_input();
    string v = tokens[0], w = tokens[1];
    v += ' ';
    w += ' ';

    int n = (int) v.size(), m = (int) w.size();

    /* create the grpah with n * m nodes */
    vector<vector<node>> graph(n, vector<node>(m));

    /* build the adjecency of the graph */
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            graph[i][j] = node {0, {}};
            if (i < n - 1) graph[i][j].adj.push_back({j, i + 1, 1});
            if (j < m - 1) graph[i][j].adj.push_back({j + 1, i, 1});
            if (j < m - 1 && i < n - 1) graph[i][j].adj.push_back({j + 1, i + 1, v[i] != w[j]});
        }
    }

    /* implementation of Dijkstra algorithm for finding the shortest path from (0, 0) to (n - 1, m - 1) */
    PriorityQueue<entry> pq;
    pq.insert({0, 0, 0});

    /* keeps track of the nodes that have already been pulled out of the priority queue to deal with pushing the same vertex multiple times */
    vector<vector<bool>> used(n, vector<bool>(m, false));

    while (!pq.empty()) {
        entry e = pq.pop_min();
        if (used[e.y][e.x]) continue;
        used[e.y][e.x] = true;

        graph[e.y][e.x].value = e.weight;
        if (e.y == n - 1 && e.x == m - 1)
            break;

        for (entry p : graph[e.y][e.x].adj) {
            if (!used[p.y][p.x])
                pq.insert({p.x, p.y, p.weight + e.weight});
        }
    }

    cout << graph[n - 1][m - 1].value << "\n";

    return 0;
}