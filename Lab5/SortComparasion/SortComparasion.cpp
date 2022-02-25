#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
#define NUM 10000
using namespace std;
string path = "unsortedData.txt";
int* standardResult;
int* arr;

bool testSort(int* arr) {
    for (int i = 0; i < NUM; i++)
        if (arr[i] != standardResult[i])
            return false;
    return true;
}

void getData(int* arr) {
	memset(arr, 0, sizeof(arr));
	ifstream input(path);
	for (int i = 0; i < NUM; i++)
		input >> arr[i];
	input.close();
}

void printArr(int* arr) {
    for (int i = 0; i < NUM; i++)
        cout << arr[i] << ' ';
    cout << endl;
}

void bubbleSort() {
    int compareFrequency = 0;
    int swapFrequency = 0;
    getData(arr);
    for (int i = 1; i < NUM; i++) {
        for (int j = 0; j < NUM - i; j++) {
            compareFrequency++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapFrequency++;
            }
        }
    }
    if (testSort(arr)) {
        cout << "冒泡排序" << endl << "比较次数: " << compareFrequency;
        cout << endl << "数据交换次数: " << swapFrequency << endl;
    }
}

void insertionSort() {
	int compareFrequency = 0;
	int swapFrequency = 0;
    getData(arr);
    for (int i = 1; i < NUM; i++) {
        for (int j = i; j > 0; j--) {
            compareFrequency++;
            if (arr[j] < arr[j - 1]) {
                swap(arr[j], arr[j - 1]);
                swapFrequency++;
            }
            else {
                break;
            }
        }
    }
    //printArr(arr);
	if (testSort(arr)) {
		cout << "插入排序" << endl << "比较次数: " << compareFrequency;
		cout << endl << "数据交换次数: " << swapFrequency << endl;
	}
}

void selectionSort() {
	int compareFrequency = 0;
	int swapFrequency = 0;
	getData(arr);
    for (int i = 0; i < NUM - 1; i++) {
        int pos = i;
        for (int j = i + 1; j < NUM; j++) {
            compareFrequency++;
            if (arr[j] < arr[pos]) {
                pos = j;
            }
        }
        swap(arr[i], arr[pos]);
        swapFrequency++;
    }
	//printArr(arr);
	if (testSort(arr)) {
		cout << "选择排序" << endl << "比较次数: " << compareFrequency;
		cout << endl << "数据交换次数: " << swapFrequency << endl;
	}
}

void QuickSort(int* arr, int l, int r, int& compareFrequency, int& swapFrequency) {
	if (l >= r)
		return;
	int key = arr[l];
	int p1 = l;
	int p2 = r;
	while (p1 <= p2) {
        p1++;
        p2--;
		while (arr[p1] <= key && p1 <= p2) {
			p1++;
			compareFrequency++;
		}
		while (arr[p2] >= key && p1 <= p2) {
			p2--;
			compareFrequency++;
		}
		swap(arr[p1], arr[p2]);
		swapFrequency++;
	}
	swap(arr[p1], arr[p2]);
	swap(arr[p2], arr[l]);
	swapFrequency += 2;
	QuickSort(arr, l, p2, compareFrequency, swapFrequency);
	QuickSort(arr, p1, r, compareFrequency, swapFrequency);
}

void quickSort() {
	int compareFrequency = 0;
	int swapFrequency = 0;
	getData(arr);
    QuickSort(arr, 0, NUM, compareFrequency, swapFrequency);
	if (testSort(arr)) {
		cout << "快速排序" << endl << "比较次数: " << compareFrequency;
		cout << endl << "数据交换次数: " << swapFrequency << endl;
	}
}

void heapInsert(int* arr, int pos, int& compareFrequency, int& swapFrequency) {
    while (pos > 0) {
        int fatherPos = 0;
        fatherPos = (pos - 1) / 2;
        if (arr[pos] > arr[fatherPos]) {
            swap(arr[pos], arr[fatherPos]);
            swapFrequency++;
            pos = fatherPos;
        }
        else
            break;
        compareFrequency++;
    }
}

void heapify(int* arr, int pos, int size, int& compareFrequency, int& swapFrequency) {
    while (pos < size) {
        if (pos * 2 + 1 < size && pos * 2 + 2 < size) {
            compareFrequency += 2;
            if (arr[pos] < arr[pos * 2 + 1] || arr[pos] < arr[pos * 2 + 2]) {
                if (arr[pos * 2 + 1] < arr[pos * 2 + 2]) {
                    swap(arr[pos], arr[pos * 2 + 2]);
                    swapFrequency++;
                    pos = pos * 2 + 2;
                }
                else {
                    swap(arr[pos], arr[pos * 2 + 1]);
                    swapFrequency++;
                    pos = pos * 2 + 1;
                }
            }
            else
                return;
        }
        else if (pos * 2 + 1 < size) {
            compareFrequency++;
            if (arr[pos] < arr[pos * 2 + 1]) {
                swap(arr[pos], arr[pos * 2 + 1]);
                swapFrequency++;
                pos = pos * 2 + 1;
            }
            else
                return;
        }
        else
            return;
    }
}

void heapSort() {
	int compareFrequency = 0;
	int swapFrequency = 0;
	getData(arr);
    for (int i = 0; i < NUM; i++)
        heapInsert(arr, i, compareFrequency, swapFrequency);
    int size = NUM;
    swap(arr[0], arr[--size]);
    swapFrequency++;
    while (size > 0) {
        heapify(arr, 0, size, compareFrequency, swapFrequency);
        swap(arr[0], arr[--size]);
        swapFrequency++;
    }
	if (testSort(arr)) {
		cout << "堆排序" << endl << "比较次数: " << compareFrequency;
		cout << endl << "数据交换次数: " << swapFrequency << endl;
	}
}

int main()
{
    standardResult = new int[NUM];
    arr = new int[NUM];
    ofstream output("unsortedData.txt");
    srand((unsigned int)time(NULL));
    for (int i = 0; i < NUM; i++) {
        int x = rand();
        standardResult[i] = x;
        output << x;
        output << ' ';
    }
    output.close();
    sort(standardResult, standardResult + NUM);
    bubbleSort();
    insertionSort();
    selectionSort();
    quickSort();
    heapSort();
}

