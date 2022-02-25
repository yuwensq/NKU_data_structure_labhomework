#include <iostream>
#include <fstream>
#include <Windows.h>
#define CODEPATH "C:\\Users\\Lenovo\\Desktop\\BTree\\NewInstructions.txt"
using namespace std;
bool instructionSucceed();
void init();
void waitInstruction();
int main()
{
	ifstream input;
	input.open(CODEPATH);
	char ch;
	input >> ch;
	if (input.eof())
	{
		input.close();
		init();
	}
	if (input.is_open() == true)
		input.close();

	waitInstruction();

	return 0;
}

bool instructionSucceed()
{
	ifstream input;
	while (1)
	{
		Sleep(500);
		input.open(CODEPATH);
		char ch;
		input >> ch;
		if (ch == '1')
		{
			input.close();
			return true;
		}
		else if (ch == '2')
		{
			cout << "There are some entries who are not in the tree!" << endl;
			return true;
		}
		else if (ch == '3')
		{
			cout << "Repeatedly insert!" << endl;
			return true;
		}
		input.close();
	}
	if (input.is_open() == true)
		input.close();

	return false;
}

void init()
{
	ofstream output;
	output.open(CODEPATH);
	output << 'C' << ' ';

	int num, order;
	cout << "检测到还未建立B树，请先建立B树" << endl;
	cout << "请输入关键字个数：";
	cin >> num;
	cout << "请输入B树阶数：";
	cin >> order;
	output << num << ' ' << order << ' ';
	int ele;
	cout << "请输入" << num << "个关键字(空格隔开)：";
	for (int i = 0; i < num; i++)
	{
		cin >> ele;
		output << ele << ' ';
	}

	output.close();

	cout << "正在建立B树";
	if (instructionSucceed() == true)
	{
		cout << endl <<"建树成功！" << endl;
	}
}

void waitInstruction()
{
	ofstream output;
	output.open(CODEPATH);
	cout << "BTree>";
	cin.ignore(100, '\n');
	/*c = getchar();
	if (c != '\n' && c != 'e') 
	{
		output << c;
	}
	else if (c == 'e')
	{
		output.close();
		return;
	}
	else if (c == 'h')
	{
		cout << "I x -- insert x into B tree" << endl;
		cout << "D x -- remove x from B tree" << endl;
		cout << "MI x1,x2,x3,...xn# -- insert x1, x2, x3, ..., xn into B tree" << endl;
		cout << "MD x1,x2,x3,...xn# -- remove x1, x2, x3, ..., xn from B tree" << endl;
		cin.ignore(100, '\n');
	}*/
	char c;
	while (c = getchar())
	{
		if (c == '\n')
		{
			output.close();
			if (instructionSucceed() == true)
			{
				cout << "BTree>";
				output.open(CODEPATH);
			}
		}
		else if (c == 'e')
		{
			output.close();
			return;
		}
		else if (c == 'h')
		{
			cout << "I x -- insert x into B tree" << endl;
			cout << "D x -- remove x from B tree" << endl;
			cout << "MI x1,x2,x3,...xn# -- insert x1, x2, x3, ..., xn into B tree" << endl;
			cout << "MD x1,x2,x3,...xn# -- remove x1, x2, x3, ..., xn from B tree" << endl;
			cout << "BTree>";
			cin.ignore(100, '\n');
		}
		else
		{
			output << c;
		}
	}
	if (output.is_open() == true)
		output.close();
}