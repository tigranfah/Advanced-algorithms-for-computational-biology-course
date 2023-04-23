#include <iostream>
#include <vector>

using namespace std;

const int INF = 2 * 1e9; // naive way to define infinity, but should work for this task

bool check(const vector<char>& a, const vector<char>& b) {
	for (int i = 0; i < (int) a.size(); ++i) {
		if (a[i] != '-' and b[i] != '-' and a[i] != b[i])
			return false;
	}
	return true;
}

int main() {

	freopen("test", "r", stdin);
    freopen("output_dp", "w", stdout);

	string temp;
	vector<string> tokens;

	while (cin >> temp) {
		if (temp[0] == '>') {
			tokens.emplace_back();
			cin >> temp;
		}
		tokens.back() += temp;
	}

	string v = tokens[0], w = tokens[1];
	cout << v << " " << w << "\n";

	v = ' ' + v;
	w = ' ' + w;
	int n = (int) v.size(), m = (int) w.size();

	vector<vector<int>> dp(n, vector<int>(m, INF));
	vector<vector<int>> back(n, vector<int>(m));
	dp[0][0] = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (i == 0 && j == 0) continue;
			
			// cout << i << " " << j << "\n";
			// cout << up << " " << left << " " << diag << ", eq " << v[i] << " " << w[j] << "\n";
			if (i > 0 && j > 0 && v[i] == w[j]) {
				dp[i][j] = dp[i - 1][j - 1];
				back[i][j] = 2;
			} else {
				int up = i > 0 ? dp[i - 1][j] : INF;
				int left = j > 0 ? dp[i][j - 1] : INF;
				int diag = i > 0 && j > 0 ? dp[i - 1][j - 1] : INF;

				back[i][j] = 0;
				dp[i][j] = up;

				if (dp[i][j] > left) {
					back[i][j] = 1;
					dp[i][j] = left;
				}
				if (dp[i][j] > diag) {
					back[i][j] = 2;
					dp[i][j] = diag;
				}
				dp[i][j]++;
			}
		}
	}

	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < m; ++j) cout << dp[i][j] << " ";
	// 	cout << "\n";
	// }

	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < m; ++j) cout << back[i][j] << " ";
	// 	cout << "\n";
	// }

	vector<char> a, b;

	// int i = n - 1, j = m - 1;
	// while (i != 0 && j != 0) {
	// 	if (back[i][j] == 2) {
	// 		// cout << "pushung " << v[i] << " " << w[j] << " " << i << " " << j << "\n";
	// 		a.push_back(v[i]);
	// 		b.push_back(w[j]);
	// 		i--; j--;
	// 	} else if (back[i][j] == 1) {
	// 		a.push_back('-');
	// 		b.push_back(w[j]);
	// 		j--;
	// 	} else {
	// 		a.push_back(v[i]);
	// 		b.push_back('-');
	// 		i--;
	// 	}
	// }

	// cout << dp[n - 1][m - 1] << "\n";

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j)
			cout << dp[i][j] << " ";
		cout << "\n";
	}

	// for (int i = a.size() - 1; i >= 0; --i)
	// 	cout << a[i];
	// cout << "\n";

	// for (int i = b.size() - 1; i >= 0; --i)
	// 	cout << b[i];
	// cout << "\n";

	// cout << "check result " << check(a, b) << "\n";

	return 0;
}