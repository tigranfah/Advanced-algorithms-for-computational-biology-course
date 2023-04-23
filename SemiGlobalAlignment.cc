#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

const int INF = 2 * 1e9;

enum Direction {
	Top = 0, Left = 1, TopLeft = 2, None = 3
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

	vector<string> tokens = read_input();

	string w = tokens[0], v = tokens[1];

	int n = (int) v.size(), m = (int) w.size();

	/*
		dp[i][j] shows the minimal score for the alignment between substring v[0:i) and w[j0:j) (0 and j0 included, but i, j not included)
		for some value j0, which is the starting index of the alignment in the string w.
	*/
	vector<vector<int>> dp(n, vector<int>(m, -INF));
	/*
		back[i][j] shows from which direction the optimal path came from to (i, j) 'cell'.
	*/
	vector<vector<Direction>> back(n, vector<Direction>(m, Direction::None));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			int top = i >= 1 ? dp[i - 1][j] - 1 : -INF;
			int left = j >= 1 ? dp[i][j - 1] - 1 : -INF;
			int diag = j >= 1 && i >= 1 ? dp[i - 1][j - 1] + (v[i] == w[j] ? 1 : -1): -INF;

			if (i == 0) {
				back[i][j] = Direction::None;
				dp[i][j] = v[i] == w[j] ? 1 : -1;
			}

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

	int opt_i = n - 1, opt_j = 0;
	for (int j = 0; j < m; ++j) {
		if (dp[opt_i][opt_j] < dp[opt_i][j])
			opt_j = j;
	}

	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < m; ++j) {
	// 		cout << dp[i][j] << " ";
	// 	}
	// 	cout << "\n";
	// }

	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < m; ++j)
	// 		cout << back[i][j] << " ";
	// 	cout << "\n";
	// }

	string sub_v, sub_w;
	int i = opt_i, j = opt_j;
	// cout << opt_i << " " << opt_j << "\n";
	cout << dp[i][j] << "\n";

	while (i >= 0 && j >= 0) {
		// cout << i << " " << j << ": ";
		if (back[i][j] == Direction::Left) {
			sub_v.push_back('-');
			sub_w.push_back(w[j]);
			j--;
		} else if (back[i][j] == Direction::Top) {
			sub_v.push_back(v[i]);
			sub_w.push_back('-');
			i--;
		} else {
			// cout << "pushing v " << v[i] << ", w " << w[j] << "\n";
			sub_v.push_back(v[i]);
			sub_w.push_back(w[j]);
			i--; j--;
		}
	}

	reverse(sub_v.begin(), sub_v.end());
	reverse(sub_w.begin(), sub_w.end());

	cout << sub_w << "\n" << sub_v << "\n";

	int match = 0, n_match = 0;
	for (int i = 0; i < (int) sub_v.size(); ++i) {
		match += sub_v[i] == sub_w[i];
		n_match += sub_v[i] != sub_w[i];
	}
	cout << "score is " << match - n_match << "\n";
	
	return 0;
}