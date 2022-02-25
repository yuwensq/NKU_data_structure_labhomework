#include <iostream>
#include <fstream>
using namespace std;
class MeltModel {
private:
	int** matrix;
	bool unable;
	int n;
	int mov[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };	//位移数组
	bool isInBounds(int nowX, int nowY) {
		if (nowX < 0 || nowY < 0 || nowX > n + 1 || nowY > n + 1)
			return false;
		return true;
	}
	void initMatrix(ifstream& input) {
		matrix = new int* [n + 2];
		for (int i = 0; i < n + 2; i++) {
			matrix[i] = new int [n + 2];
			for (int j = 0; j < n + 2; j++) {
				matrix[i][j] = 0;
			}
		}
		char c;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				input >> c;
				if (c == '_')
					matrix[i][j] = 0;
				else if (c == '#')
					matrix[i][j] = 1;
				else {
					cout << "Input format error!";
					exit(0);
				}
			}
		}
		coloring(0, 0);
		return;
	}
	void printMatrix() {
		for (int i = 0; i <= n + 1; i++) {
			for (int j = 0; j <= n + 1; j++) {
				if (matrix[i][j] == 1)
					cout << '#';
				else if (matrix[i][j] == 0 || matrix[i][j] == 2)
					cout << '_';
			}
			cout << endl;
		}
	}
	void coloring(int nowX, int nowY) {
		if (matrix[nowX][nowY] == 1 || matrix[nowX][nowY] == 2 || matrix[nowX][nowY] == -1)
			return;
		else if (nowX == 0 || nowX == n + 1 || nowY == 0 || nowY == n + 1)
			matrix[nowX][nowY] = -1;
		else if (matrix[nowX][nowY] == 0)
			matrix[nowX][nowY] = 2;
		for (int i = 0; i < 4; i++) {
			int nextX = nowX + mov[i][0];
			int nextY = nowY + mov[i][1];
			if (!isInBounds(nextX, nextY))
				continue;
			coloring(nextX, nextY);
		}
	}
	bool totallyMelted() {
		for (int i = 1; i <= n; i++)
			for (int j = 1; j <= n; j++)
				if (matrix[i][j] == 1)
					return false;
		return true;
	}
	void melt(int nowX, int nowY, int** vised, int** willBeMelted) {
		if (vised[nowX][nowY])
			return;
		vised[nowX][nowY] = 1;
		if (matrix[nowX][nowY] == 1) {
			int contactNumbers = 0;
			for (int i = 0; i < 4; i++) {
				int nextX = nowX + mov[i][0];
				int nextY = nowY + mov[i][1];
				if (!isInBounds(nextX, nextY))
					continue;
				if (matrix[nextX][nextY] == 2)
					contactNumbers++;
			}
			if (contactNumbers >= 2) {
				willBeMelted[nowX][nowY] = 2;
				unable = false;
			}
		}
		else if (matrix[nowX][nowY] == 2 || matrix[nowX][nowY] == -1) {
			for (int i = 0; i < 4; i++) {
				int nextX = nowX + mov[i][0];
				int nextY = nowY + mov[i][1];
				if (!isInBounds(nextX, nextY))
					continue;
				melt(nextX, nextY, vised, willBeMelted);
			}
		}
	}
	void spread(int nowX, int nowY, int** vised) {
		if (vised[nowX][nowY])
			return;
		if (matrix[nowX][nowY] == 1)
			return;
		if (matrix[nowX][nowY] != -1)
			matrix[nowX][nowY] = 2;
		vised[nowX][nowY] = 1;
		for (int i = 0; i < 4; i++) {
			int nextX = nowX + mov[i][0];
			int nextY = nowY + mov[i][1];
			if (!isInBounds(nextX, nextY))
				continue;
			spread(nextX, nextY, vised);
		}
	}
	void doMelt() {
		int** willBeMelted;
		willBeMelted = new int* [n + 1];
		for (int i = 0; i < n + 1; i++) {
			willBeMelted[i] = new int[n + 1];
			for (int j = 0; j < n + 1; j++) {
				willBeMelted[i][j] = 0;
			}
		}
		int** vised;	//记录当前结点是否访问过
		vised = new int* [n + 2];
		for (int i = 0; i < n + 2; i++) {
			vised[i] = new int [n + 2];
			for (int j = 0; j < n + 2; j++) {
				vised[i][j] = 0;
			}
		}
		melt(0, 0, vised, willBeMelted);
		for (int i = 0; i < n + 2; i++) {
			for (int j = 0; j < n + 2; j++) {
				vised[i][j] = 0;
			}
		}
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (willBeMelted[i][j])
					matrix[i][j] = 2;
			}
			delete[] willBeMelted[i];
		}
		delete[] willBeMelted;
		spread(0, 0, vised);
		for (int i = 0; i < n + 2; i++)
			delete[] vised[i];
		delete[] vised;
	}
public:
	MeltModel(string path) {
		unable = true;
		ifstream input(path);
		input >> n;
		initMatrix(input);
		cout << "Initial matrix is: " << endl;
		printMatrix();
	}
	void stimulatMelting() {
		int t = 0;
		while (!totallyMelted()) {
			doMelt();
			if (unable == true) {
				cout << "This metal can't be melted!";
				return;
			}
			unable = true;
			t++;
			cout << "t = " << t << "s, " << "the matrix is: " << endl;
			printMatrix();
		}
		cout << "Total time is: " << t << "s";
	}
};
int main() {
	MeltModel mm("inputMatrix.txt");
	mm.stimulatMelting();
	return 0;
}