#include <iostream>

using namespace std;

const int N = 1e5;
int arr[N + 5];

int search(int l, int r, int v) {
	while (r - l > 1) {
		int mid = (l + r) / 2;
		if (arr[mid] == v)
			return mid + 1;
		else if (arr[mid] > v)
			r = mid;
		else l = mid;
	}

	return -1;
}

int main() {
	int n, m, v;	
	cin >> n >> m;

	for (int i = 0; i < n; ++i)
		cin >> arr[i];

	for (int i = 0; i < m; ++i) {
		cin >> v;
		cout << search(-1, n, v) << " ";
	}
}