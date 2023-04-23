#include <iostream>
#include <vector>

using namespace std;

const int INF = 2 * 1e9; // naive way to define infinity, but should work for this task

int main() {

	string temp;
	string v, w;
	cin >> temp >> v >> temp >> w;

	v = ' ' + v;
	w = ' ' + w;
	int n = (int) v.size(), m = (int) w.size();

	vector<vector<int>> dp(n, vector<int>(m, INF));
	dp[0][0] = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (i == 0 && j == 0) continue;
			int up = i > 0 ? dp[i - 1][j] : INF;
			int left = j > 0 ? dp[i][j - 1] : INF;
			int diag = i > 0 && j > 1 ? dp[i - 1][j - 1] : INF;
			// cout << i << " " << j << "\n";
			// cout << up << " " << left << " " << diag << ", eq " << v[i] << " " << w[j] << "\n";
			dp[i][j] = min(up, min(left, diag)) + (v[i] == v[j] ? 0 : 1);
		}
	}

	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < m; ++j) cout << dp[i][j] << " ";
	// 	cout << "\n";
	// }



	cout << dp[n - 1][m - 1] << "\n";

	return 0;
}