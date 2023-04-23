#include <iostream>
#include <vector>

using namespace std;

const int INF = (int) 2 * 1e9;

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

struct Entry {
	int index, value;

	friend ostream& operator<<(ostream& s, const Entry& e);
};

ostream& operator<<(ostream& s, const Entry& e) {
	s << "([" << e.index << "], " << e.value << ")";
	return s;
}

int main() {

	vector<string> tokens = read_input();

	string v = tokens[0], w = tokens[1];
	v = ' ' + v;
	w = ' ' + w;

	int n = (int) v.size(), m = (int) w.size();
	
	if (n > m) { // make sure w.size() > v.size()
		swap(v, w);
		swap(n, m);
	}

	/*
		dp is the matrix for dynamic programming,
		dp[i][j].value that shows the edit distance to make v[:i] equal to w[:j]
		dp[i][j].index shows on which index along the second axis (horisontal) the cell is positioned.
	*/
	vector<vector<Entry>> dp(n);
	/*
		How will the algo work?
		Donte v the first string, w the second string, n and m their lengths respectively.

		example: in this case the #s are forming the part of the matrix using which the edit distance will be calculated

		(grid 1)
		#.......
		.#......
		..#.....
		...#....
		....#...

		With each step expand one element from each side of every row.

		1. (step 1) ...#...
		2. compute edit distance, if satisfied finish, if not continue
		3. (step 2) ..###..
		4. ...

		For the (grid 1):
		dp[0] will have length 1: { ([0], v0) } for some v0
		dp[1] will have length 1: { ([1], v1) } for some v1
		(and so on)

		How to find dp[i - 1][j], dp[i - 1][j - 1] and dp[i][j - 1] for calculating dp[i][j]?
		Given i, j. Finding dp[i][j - 1] is easy. To find the dp[i - 1][j] (ans similarly dp[i - 1][j - 1]),
		go up i - 1 and j := j - dp[i - 1][0].index.
	*/

	// construct the initial path from (0, 0) to (n, m)
	int a = 0, b = 0;
	while (a < n && b < m) {
		if (n - a != m - b) {
			dp[a].push_back(Entry {b, 0});
			a++;
			b++;
		}
	}

	int current_edit_distance = INF;
	int band_size = 1;

	while (current_edit_distance > band_size) {

		// for (int i = 0; i < n; ++i) {
		// 	for (int j = 0; j < dp[i].front().index; ++j)
		// 		cout << ".";

		// 	for (int j = dp[i].front().index; j <= dp[i].back().index; ++j)
		// 		cout << "#";

		// 	for (int j = dp[i].back().index + 1; j < m; ++j)
		// 		cout << ".";

		// 	cout << "\n";
		// }

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < (int) dp[i].size(); ++j) {

				if (i == 0 && dp[i][j].index == 0)
					dp[i][j].value = 0;
				else if (i == 0)
					dp[i][j].value = dp[i][j].index;
				else if (dp[i][j].index == 0)
					dp[i][j].value = i;
				else {
					if (v[i] == w[dp[i][j].index]) {
						dp[i][j].value = dp[i - 1][dp[i][j].index - 1 - dp[i - 1].front().index].value;
					} else {

						int index = dp[i][j].index;

						int top_value = INF;
						int left_value = INF;

						if (dp[i - 1].front().index + (int) dp[i - 1].size() > index)
							top_value = dp[i - 1][index - (int) dp[i - 1].front().index].value + 1;

						if (j > 0)
							left_value = dp[i][j - 1].value + 1;

						int top_left_value = dp[i - 1][index - 1 - (int) dp[i - 1].front().index].value + 1;

						dp[i][j].value = min(top_value, min(left_value, top_left_value));
					}
				}
			}
		}

		// check if the alignment reach the end of both stirngs
		if (dp[n - 1].back().index == m - 1)
			current_edit_distance = dp[n - 1].back().value;

		// expanding the band
		for (int i = 0; i < n; ++i) {
			if (dp[i].front().index > 0)
				dp[i].insert(dp[i].begin(), Entry {dp[i].front().index - 1, 0}); // this is slow operation
			if (dp[i].back().index < m - 1)
				dp[i].push_back(Entry {dp[i].back().index + 1, 0});
		}
		band_size++;

	}

	cout << current_edit_distance << "\n";

	return 0;
}