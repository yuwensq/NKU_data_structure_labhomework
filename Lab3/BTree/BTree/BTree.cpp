#include <iostream>
#include <fstream>
#include <Windows.h>
#include <graphics.h>
#include <conio.h>
#include "BTree.h"
#define CODEPATH "C:\\Users\\Lenovo\\Desktop\\BTree\\NewInstructions.txt"
using namespace std;

void init();
void solve();
void messageResponse(BTree&);
void drawBTree(BTree&);

int main()
{
	init();
	solve();
	closegraph();
	return 0;
}

void init()
{
	initgraph(800, 450);
	ofstream output;
	output.open(CODEPATH);
	output.clear();
	output.close();
	::ShellExecuteA(NULL, "open", "C:\\Users\\Lenovo\\Desktop\\DriveBTree\\Debug\\DriveBTree.exe", NULL, NULL, SW_SHOW);
}

void solve()
{
	ifstream input;
	while (1)
	{
		Sleep(500);
		input.open(CODEPATH);
		char ch;
		input >> ch;
		if (input.eof())
		{
			input.close();
			continue;
		}
		else if (ch == 'C')
		{
			int num, order;
			input >> num >> order;
			BTree bTree(order);
			for (int i = 0; i < num; i++)
			{
				int a;
				input >> a;
				bTree.insert(a);
			}
			input.close();
			ofstream output(CODEPATH);
			output << 1;
			output.close();
			drawBTree(bTree);
			messageResponse(bTree);
		}
		else
		{
			input.close();
		}
	}
}

void messageResponse(BTree& bTree)
{
	ifstream input;
	ofstream output;
	Error_cod result = success;
	char c;
	int a;
	while (1)
	{
		Sleep(500);
		result = success;
		input.open(CODEPATH);
		input >> c;
		if (c == 'I')
		{
			input >> a;
			result = bTree.insert(a);
			output.open(CODEPATH);
			if (result == success)
				output << 1;
			output.close();
		}
		else if (c == 'D')
		{
			input >> a;
			result = bTree.remove(a);
			output.open(CODEPATH);
			if (result == success)
				output << 1;
			output.close();
		}
		else if (c == 'M')
		{
			input >> c;
			if (c == 'I')
			{
				while (c != '#')
				{
					input >> a;
					if (result != success)
						bTree.insert(a);
					else
						result = bTree.insert(a);
					while (input >> c && c != ',' && c != '#');
				}
			}
			else if (c == 'D')
			{
				while (c != '#')
				{
					input >> a;
					if (result != success)
						bTree.insert(a);
					else
						result = bTree.remove(a);
					while (input >> c && c != ',' && c != '#');
				}
			}
		}
		else
		{
			input.close();
			continue;
		}
		output.open(CODEPATH);
		if (result == success)
			output << 1;
		else if (result == not_present)
			output << 2;
		else if (result == duplicate_error)
			output << 3;
		output.close();
		drawBTree(bTree);
		if (input.is_open())
			input.close();
	}
}

void drawBTree(BTree& bTree)
{
	bTree.show();
}