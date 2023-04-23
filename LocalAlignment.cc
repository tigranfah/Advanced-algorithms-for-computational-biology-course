#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

const int INF = 2 * 1e9;

map<char, int> amino_map; // map data structure can easily be optimized away, it is just for convenience
vector<vector<int>> score_matrix;

enum Direction {
	Top = 0, Left = 1, TopLeft = 2, None = 3
};

void read_score_matrix(const string& name) {
	score_matrix.assign(20, vector<int>(20));

	ifstream file;
	file.open(name);

	if (file.is_open()) {
		char c;
		for (int i = 0; i < 20; ++i) {
			file >> c;
			amino_map[c] = i;
		}
		int num;
		for (int i = 0; i < 20; ++i) {
			file >> c;
			for (int j = 0; j < 20; ++j) {
				file >> num;
				score_matrix[i][j] = num;
			}
		}
	}
}

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
	read_score_matrix("BLOSUM62.txt");
	vector<string> tokens = read_input();

	string v = tokens[0], w = tokens[1];

	int n = (int) v.size(), m = (int) w.size();

	/*
		dp[i][j] shows the best score to make v(i0:i) equal to w(j0:j) (i0, i, j0, j indices are not included),
		after performing some operations, for some i0 and j0 indices in the string, which are less then i and j respectively.
	*/
	vector<vector<int>> dp(n, vector<int>(m, -INF));
	/*
		back[i][j] shows from which direction the optimal path came from to (i, j) 'cell'.
	*/
	vector<vector<Direction>> back(n, vector<Direction>(m));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			int top = i >= 1 ? dp[i - 1][j] - 5 : -INF;
			int left = j >= 1 ? dp[i][j - 1] - 5 : -INF;
			int diag = j >= 1 && i >= 1 ? dp[i - 1][j - 1] + score_matrix[amino_map[v[i]]][amino_map[w[j]]] : -INF;
			
			back[i][j] = Direction::None;
			dp[i][j] = 0;

			if (dp[i][j] < diag) {
				back[i][j] = Direction::TopLeft;
				dp[i][j] = diag;
			}

			if (dp[i][j] < left) {
				back[i][j] = Direction::Left;
				dp[i][j] = left;
			}

			if (dp[i][j] < top) {
				back[i][j] = Direction::Top;
				dp[i][j] = top;
			}

		}
	}

	int opt_i = 0, opt_j = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			// cout << dp[i][j] << " ";
			if (dp[opt_i][opt_j] < dp[i][j]) {
				opt_i = i; opt_j = j;
			}
		}
		// cout << "\n";
	}

	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < m; ++j)
	// 		cout << back[i][j] << " ";
	// 	cout << "\n";
	// }

	string sub_v, sub_w;
	int i = opt_i, j = opt_j;
	cout << dp[i][j] << "\n";

	while (back[i - 1][j - 1] != Direction::None) { // we are done if we reach -1
		if (back[i - 1][j - 1] == Direction::Left) {
			sub_v.push_back(v[i]);
			sub_w.push_back('-');
			j--;
		} else if (back[i - 1][j - 1] == Direction::Top) {
			sub_v.push_back('-');
			sub_w.push_back(w[j]);
			i--;
		} else {
			sub_v.push_back(v[i]);
			sub_w.push_back(w[j]);
			i--; j--;
		}
	}

	// reverse(sub_v.begin(), sub_v.end());
	// reverse(sub_w.begin(), sub_w.end());

	// cout << sub_v << "\n" << sub_w << "\n";
	// i += i == 0;
	// j += j == 0;
	// cout << opt_i << " " << opt_j << "\n";
	// cout << i << " " << j << "\n";

	cout << v.substr(i, opt_i - i + 1) << "\n" << w.substr(j, opt_j - j + 1) << "\n";

	return 0;
}