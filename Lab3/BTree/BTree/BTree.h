#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <sstream>
#include <graphics.h>
using namespace std;

enum Error_cod
{
	not_present,
	overflow,
	duplicate_error,
	success
};

struct BNode {
	int count;
	int* arrData;
	BNode** arrBranch;
	BNode(int order)
	{
		count = 0;
		arrData = new int[order - 1];
		arrBranch = new BNode * [order];
		for (int i = 0; i < order; i++)
			arrBranch[i] = nullptr;
	}
};

class BTree {
private:
	int m;
	BNode* root;
	Error_cod recursiveSearch(BNode*, int);
	Error_cod searchNode(BNode*, int, int&);
	Error_cod pushDown(BNode*, int, int&, BNode*&);
	void pushIn(BNode*, int, BNode*, int);
	void splitNode(BNode*, int, BNode*, int, int&, BNode*&);
	Error_cod recursiveRemove(BNode*, int);
	void removeData(BNode*, int);
	void copyInPredecessor(BNode*, int);
	void restore(BNode*, int);
	void moveLeft(BNode*, int);
	void moveRight(BNode*, int);
	void combine(BNode*, int);
	void destoryTree(BNode*);
public:
	BTree(int);
	~BTree();
	Error_cod search(int);
	Error_cod insert(int);
	Error_cod remove(int);
	void show();
};

BTree::BTree(int order)
{
	root = nullptr;
	m = order;
}

BTree::~BTree()
{
	destoryTree(root);
}

void BTree::destoryTree(BNode* current)
{
	if (current == nullptr)
		return;
	for (int i = 0; i <= current->count; i++)
	{
		destoryTree(current->arrBranch[i]);
	}
	delete current;
	return;
}

void BTree::show()
{
	cleardevice();
	int width, length, margin;
	length = 10;
	width = 20;
	margin = 30;
	if (root == nullptr)
		return;
	queue<BNode*> qBn;
	queue<int> qLeft, qTop;
	qBn.push(root);
	qBn.push(nullptr);
	qLeft.push(0);
	qTop.push(0);
	qLeft.push(-1);
	qTop.push(-1);
	int rowLeft = 0;
	while (qBn.size() > 1)
	{
		BNode* current = qBn.front();
		int left, top;
		left = qLeft.front();
		top = qTop.front();
		qBn.pop();
		qLeft.pop();
		qTop.pop();
		if (current == nullptr)
		{
			qBn.push(nullptr);
			qLeft.push(-1);
			qTop.push(-1);
			rowLeft = 0;
			continue;
		}
		int totalLength = 0;
		int preCharRihgt = left;
		for (int i = 0; i < current->count; i++)
		{
			TCHAR s[100];
			swprintf_s(s, _T("%d"), current->arrData[i]);
			int size = 0;
			while (s[size++] != 0);
			size--;
			rectangle(preCharRihgt, top, preCharRihgt + size * length, top + width);
			outtextxy(preCharRihgt + 2, top + 2, s);
			totalLength += size * length;
			if (current->arrBranch[i] != nullptr)
			{
				qBn.push(current->arrBranch[i]);
				qLeft.push(rowLeft);
				qTop.push(top + width + margin);
				POINT pts[] = { {preCharRihgt, top + width}, {rowLeft, top + width + margin} };
				polyline(pts, 2);
				rowLeft += current->arrBranch[i]->count * 20 + margin;
			}
			preCharRihgt = preCharRihgt + size * length;
		}
		if (current->arrBranch[current->count] != nullptr)
		{
			qBn.push(current->arrBranch[current->count]);
			qLeft.push(rowLeft);
			qTop.push(top + width + margin);
			POINT pts[] = { {preCharRihgt, top + width}, {rowLeft, top + width + margin} };
			polyline(pts, 2);
			rowLeft += current->arrBranch[current->count]->count * 20 + margin;
		}
	}
}

Error_cod BTree::search(int target)
{
	return recursiveSearch(root, target);
}

Error_cod BTree::searchNode(BNode* current, int target, int& position)
{
	position = 0;
	while (position < current->count && target > current->arrData[position])
		position++;
	if (position < current->count && target == current->arrData[position])
		return success;
	return not_present;
}

Error_cod BTree::recursiveSearch(BNode* current, int target)
{
	Error_cod result = not_present;
	int position;
	if (current != nullptr)
	{
		result = searchNode(current, target, position);
		if (result == not_present)
			result = recursiveSearch(current->arrBranch[position], target);
	}
	return result;
}

Error_cod BTree::insert(int newEntry)
{
	int median;
	BNode* rightBranch, * newRoot;
	Error_cod result = pushDown(root, newEntry, median, rightBranch);
	if (result == overflow)
	{
		newRoot = new BNode(m);
		newRoot->count = 1;
		newRoot->arrData[0] = median;
		newRoot->arrBranch[0] = root;
		newRoot->arrBranch[1] = rightBranch;
		root = newRoot;
		result = success;
	}
	return result;
}

Error_cod BTree::pushDown(BNode* current, int newEntry, int& median, BNode*& rightBranch)
{
	Error_cod result;
	int position = 0;
	if (current == nullptr)
	{
		median = newEntry;
		rightBranch = nullptr;
		result = overflow;
	}
	else
	{
		if (searchNode(current, newEntry, position) == success)
			result = duplicate_error;
		else
		{
			int extraEntry;
			BNode* extraBranch;
			result = pushDown(current->arrBranch[position], newEntry, extraEntry, extraBranch);
			if (result == overflow)
			{
				if (current->count < m - 1)
				{
					result = success;
					pushIn(current, extraEntry, extraBranch, position);
				}
				else
					splitNode(current, extraEntry, extraBranch, position, median, rightBranch);
			}
		}
	}
	return result;
}

void BTree::pushIn(BNode* current, int entry, BNode* rightBranch, int position)
{
	for (int i = current->count; i > position; i--)
	{
		current->arrData[i] = current->arrData[i - 1];
		current->arrBranch[i + 1] = current->arrBranch[i];
	}
	current->arrData[position] = entry;
	current->arrBranch[position + 1] = rightBranch;
	current->count++;
}

void BTree::splitNode(BNode* current, int extraEntry, BNode* extraBranch, int position, int& median, BNode*& rightHalf)
{
	rightHalf = new BNode(m);
	int mid = m / 2;
	if (position <= mid)
	{
		for (int i = mid; i < m - 1; i++)
		{
			rightHalf->arrData[i - mid] = current->arrData[i];
			rightHalf->arrBranch[i + 1 - mid] = current->arrBranch[i + 1];
		}
		current->count = mid;
		rightHalf->count = m - 1 - mid;
		pushIn(current, extraEntry, extraBranch, position);
	}
	else
	{
		mid++;
		for (int i = mid; i < m - 1; i++)
		{
			rightHalf->arrData[i - mid] = current->arrData[i];
			rightHalf->arrBranch[i + 1 - mid] = current->arrBranch[i + 1];
		}
		current->count = mid;
		rightHalf->count = m - 1 - mid;
		pushIn(rightHalf, extraEntry, extraBranch, position - mid);
	}
	rightHalf->arrBranch[0] = current->arrBranch[current->count];
	median = current->arrData[current->count - 1];
	current->count--;
}

Error_cod BTree::remove(int target)
{
	Error_cod result;
	result = recursiveRemove(root, target);
	if (root != nullptr && root->count == 0)
	{
		BNode* oldRoot = root;
		root = root->arrBranch[0];
		delete oldRoot;
	}
	return result;
}

Error_cod BTree::recursiveRemove(BNode* current, int target)
{
	Error_cod result;
	int position;
	if (current == nullptr)
		result = not_present;
	else
	{
		if (searchNode(current, target, position) == success)
		{
			result = success;
			if (current->arrBranch[position] != nullptr)//不是叶子
			{
				copyInPredecessor(current, position);//找左子树里最大的顶替它
				recursiveRemove(current->arrBranch[position], current->arrData[position]);//变成了删除左子树里最大的
			}
			else//是叶子
				removeData(current, position);
		}
		else
			result = recursiveRemove(current->arrBranch[position], target);
		if (current->arrBranch[position] != nullptr && current->arrBranch[position]->count < (m - 1) / 2)
			restore(current, position);
	}
	return result;
}

void BTree::removeData(BNode* current, int position)
{
	for (int i = position; i < current->count - 1; i++)
		current->arrData[i] = current->arrData[i + 1];
	current->count--;
}

void BTree::copyInPredecessor(BNode* current, int position)
{
	BNode* leaf = current->arrBranch[position];
	while (leaf->arrBranch[leaf->count] != nullptr)
		leaf = leaf->arrBranch[leaf->count];
	current->arrData[position] = leaf->arrData[leaf->count - 1];
}

void BTree::restore(BNode* current, int position)
{
	//能借就借，不能借找爹
	if (position == current->count)//不能从右边借了
	{
		if (current->arrBranch[position - 1]->count > (m - 1) / 2)
			moveRight(current, position - 1);
		else
			combine(current, position);
	}
	else if (position == 0)//不能从左边借了
	{
		if (current->arrBranch[1]->count > (m - 1) / 2)
			moveLeft(current, 1);
		else
			combine(current, 1);
	}
	else//左右均可借
	{
		if (current->arrBranch[position - 1]->count > (m - 1) / 2)
			moveRight(current, position - 1);
		else if (current->arrBranch[position + 1]->count > (m - 1) / 2)
			moveLeft(current, position + 1);
		else
			combine(current, position);
	}
}

void BTree::moveLeft(BNode* current, int position)
{
	BNode* leftBranch = current->arrBranch[position - 1];
	BNode* rightBranch = current->arrBranch[position];
	leftBranch->arrData[leftBranch->count] = current->arrData[position - 1];
	leftBranch->arrBranch[++leftBranch->count] = rightBranch->arrBranch[0];
	current->arrData[position - 1] = rightBranch->arrData[0];
	rightBranch->count--;
	for (int i = 0; i < rightBranch->count; i++)
	{
		rightBranch->arrData[i] = rightBranch->arrData[i + 1];
		rightBranch->arrBranch[i] = rightBranch->arrBranch[i + 1];
	}
	rightBranch->arrBranch[rightBranch->count] = rightBranch->arrBranch[rightBranch->count + 1];
}

void BTree::moveRight(BNode* current, int position)
{
	BNode* rightBranch = current->arrBranch[position + 1];
	BNode* leftBranch = current->arrBranch[position];
	rightBranch->arrBranch[rightBranch->count + 1] = rightBranch->arrBranch[rightBranch->count];
	for (int i = rightBranch->count; i > 0; i--)
	{
		rightBranch->arrData[i] = rightBranch->arrData[i - 1];
		rightBranch->arrBranch[i] = rightBranch->arrBranch[i - 1];
	}
	rightBranch->count++;
	rightBranch->arrData[0] = current->arrData[position];
	rightBranch->arrBranch[0] = leftBranch->arrBranch[leftBranch->count--];
	current->arrData[position] = leftBranch->arrData[leftBranch->count];
}

void BTree::combine(BNode* current, int position)
{
	int i;
	BNode* leftBranch = current->arrBranch[position - 1];
	BNode* rightBranch = current->arrBranch[position];
	leftBranch->arrData[leftBranch->count] = current->arrData[position - 1];
	leftBranch->arrBranch[++leftBranch->count] = rightBranch->arrBranch[0];
	for (i = 0; i < rightBranch->count; i++)
	{
		leftBranch->arrData[leftBranch->count] = rightBranch->arrData[i];
		leftBranch->arrBranch[++leftBranch->count] = rightBranch->arrBranch[i + 1];
	}
	current->count--;
	for (i = position - 1; i < current->count; i++)
	{
		current->arrData[i] = current->arrData[i + 1];
		current->arrBranch[i + 1] = current->arrBranch[i + 2];
	}
	delete rightBranch;
}