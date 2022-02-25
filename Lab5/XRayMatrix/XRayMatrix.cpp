#include <iostream>
#include <algorithm>
using namespace std;
struct Node {
	int rows;
	int zeros;
};
bool cmp(Node& a, Node& b) {
	return a.rows > b.rows;
}
int main()
{
	int n, m, k;
	cin >> n >> m >> k;
	int cnt = 0;
	Node* arrRow = new Node [n];
	int** arr = new int* [n];
	bool* wFinded = new bool[n];
	for (int i = 0; i < n; i++) {
		wFinded[i] = false;
		arr[i] = new int[m];
		for (int j = 0; j < m; j++)
			cin >> arr[i][j];
	}
	for (int i = 0; i < n; i++) {
		if (wFinded[i])
			continue;
		int rows = 1;
		int zeors = 0;
		for (int k = 0; k < m; k++) 
			if (arr[i][k] == 0)
				zeors++;
		arrRow[cnt].zeros = zeors;
		for (int j = i + 1; j < n; j++) {
			bool same = true;
			for (int k = 0; k < m; k++) {
				if (arr[i][k] != arr[j][k]) {
					same = false;
					break;
				}
			}
			if (same) {
				rows++;
				wFinded[j] = true;
			}
		}
		arrRow[cnt].rows = rows;
		cnt++;
	}
	sort(arrRow, arrRow + cnt, cmp);
	int result = 0;
	for (int i = 0; i < cnt; i++) {
		if (arrRow[i].zeros > k)
			continue;
		else if (arrRow[i].zeros % 2 == k % 2){
			result = arrRow[i].rows;
			break;
		}
	}
	cout << "最大导通行数为：" << result;
	return 0;
}
