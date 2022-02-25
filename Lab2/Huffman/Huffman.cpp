#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;
typedef string path;

struct Node {
	//二叉树的节点
	Node* leftChild, * rightChild;
	char c;
	int num;
	Node(int num = 0, Node* leftChild = NULL, Node* rightChild = NULL, char c = 0) {
		this->num = num;
		this->leftChild = leftChild;
		this->rightChild = rightChild;
		this->c = c;
	}
};

class Huffman{
	//封装哈夫曼工具类，实现对文章的哈夫曼编码和解码。
private:

	static Huffman* instance;
	static Node* root;//哈夫曼树的根
	static string* mapp;//字符到编码的映射

	Huffman() {};
	static void processData(path, int*);//数据处理
	static void constructTree(int*);//建立二叉树
	static void insertToRootSet(vector<Node*>&, Node*);
	static void createMapping(Node*, string);//产生字符哈夫曼编码映射
	static void saveCode(int*);//保存每个字符出现的次数及编码
	static void destoryTree(Node*);//重复生成哈夫曼树时用,释放内存时用
public:

	static Huffman* open();
	void generateCode(path);//生成哈夫曼树及编码
	void code(path);//根据生成的哈夫曼树对文件进行编码
	void decode(path);//译码函数
	void decode(path, path);
	void close();//delete
};

Huffman* Huffman::instance = NULL;
Node* Huffman::root = NULL;
string* Huffman::mapp = NULL;

Huffman* Huffman::open()
{
	if (instance == NULL)
		instance = new Huffman();
	return instance;
}

void Huffman::destoryTree(Node* nowNode = root)
{
	if (nowNode == NULL)
		return;
	destoryTree(nowNode->leftChild);
	destoryTree(nowNode->rightChild);
	delete nowNode;
}

void Huffman::generateCode(path sour)
{
	//生成编码树，并输出文件中字符出现的次数和编码
	destoryTree(root);
	root = new Node();
	if (mapp == NULL)
		mapp = new string[128];
	int arr[128];
	processData(sour, arr);
	constructTree(arr);
	createMapping(root, "");
	saveCode(arr);
	cout << "生成编码成功！字符权值及对应编码请见文件outfile1.txt" << endl;
	return;
}

void Huffman::processData(path sour, int* arr)
{
	//记录文本文件中的字母出现的次数
	for (int i = 0; i < 128; i++)
	{
		arr[i] = 0;
	}
	ifstream input;
	input.open(sour);
	char c;
	input.get(c);
	while (!input.eof())
	{
		arr[int(c)]++;
		input.get(c);
	}

	input.close();
	return;
}

void Huffman::insertToRootSet(vector<Node*>& rootSet, Node* newNode)
{
	rootSet.push_back(newNode);
	int key = rootSet[rootSet.size() - 1]->num;
	int j = 0;
	for (j = rootSet.size() - 1; j > 0; j--)
	{
		//插入排序，升序排列
		if (key < rootSet[j - 1]->num)
		{
			rootSet[j] = rootSet[j - 1];
		}
		else
		{
			break;
		}
	}
	rootSet[j] = newNode;
	return;
}

void Huffman::constructTree(int* arr)
{
	//建立二叉树
	vector<Node*> rootSet;//树根集合
	rootSet.clear();
	for (int i = 0; i < 128; i++)
	{
		Node* newNode = new Node(arr[i], NULL, NULL, char(i));
		insertToRootSet(rootSet, newNode);
	}
	if (rootSet.size() == 0)
		return;
	while (rootSet.size() > 2)
	{
		Node* leaf1, * leaf2;
		leaf1 = rootSet[0];
		leaf2 = rootSet[1];
		rootSet.erase(rootSet.begin());
		rootSet.erase(rootSet.begin());
		int num = leaf1->num + leaf2->num;
		Node* newNode = new Node(num, leaf1, leaf2);
		insertToRootSet(rootSet, newNode);
	}
	if (rootSet.size() == 1)
	{
		root->num = rootSet[0]->num;
		root->leftChild = rootSet[0];
		return;
	}
	root->num = rootSet[0]->num + rootSet[1]->num;
	root->leftChild = rootSet[0];
	root->rightChild = rootSet[1];
	return;
}

void Huffman::createMapping(Node* nowNode, string nowCode)
{
	if (nowNode == nullptr)
		return;
	if (nowNode->c != 0)
	{
		mapp[int(nowNode->c)] = nowCode;
		return;
	}
	nowCode += '0';
	createMapping(nowNode->leftChild, nowCode);
	nowCode[nowCode.size() - 1] = '1';
	createMapping(nowNode->rightChild, nowCode);
}

void Huffman::saveCode(int* arr)
{
	//输出字符出现次数和编码
	ofstream output;
	output.open("outputfile1.txt");
	output << "字符" << '\t' << "出现次数" << '\t' << '\t' << "对应编码" << endl;
	for (int i = 0; i < 128; i++)
	{
		if (arr[i] > 0)
		{
			output << char(i) << '\t' << arr[i] << '\t' << '\t' << mapp[i] << endl;
		}
	}
	output.close();
	return;
}

void Huffman::code(path sour)
{
	if (root == NULL || (root->leftChild == NULL && root->rightChild == NULL))
	{
		cout << "Please generate code first!";
		return;
	}
	ifstream input;
	input.open(sour);
	ofstream output;
	output.open("outputfile.dat", ios::binary);
	int originBytes = 1;
	int codedBytes = 0;
	char ch = 0;
	int bit = 7;
	char c;
	input.get(c);
	while (!input.eof())
	{
		string charCode = mapp[int(c)];
		for (int i = 0; i < charCode.size(); i++)
		{
			ch |= ((charCode[i] - '0') << bit);
			bit--;
			if (bit < 0)
			{
				output.write((char*)&ch, sizeof(ch));
				codedBytes++;
				ch = 0;
				bit = 7;
			}
		}
		input.get(c);
		originBytes++;
	}
	if (bit != 7)
	{
		output.write((char*)&ch, sizeof(ch));
		ch = bit + 1;
		output.write((char*)&ch, sizeof(ch));//二进制文件最后一个字节存储多使用了多少位
		codedBytes += 2;
	}
	else
	{
		ch = 0;
		output.write((char*)&ch, sizeof(ch));
		codedBytes++;
	}
	cout << "编码成功，压缩比为：" << double(1.0 * originBytes / codedBytes) << endl;
	cout << "二进制代码请见文件outfile.dat";
	input.close();
	output.close();
	return;
}

void Huffman::decode(path sour)
{
	ifstream input;
	input.open(sour, ios::binary);
	ofstream output;
	output.open("outputfile2.txt");

	char c = 0;
	input.seekg(-1, ios::end);
	input.read((char*)&c, sizeof(c));
	int remaind = int(c);//得到倒数第二个字节多用了几位

	input.seekg(-2, ios::end);
	streampos endPos = input.tellg();//从开始到最后一个字节结束
	input.seekg(ios::beg);
	streampos nowPos = input.tellg();

	Node* nowNode = root;
	while (nowPos != endPos)
	{
		input.read((char*)&c, sizeof(c));
		for (int i = 7; i >= 0; i--)
		{
			int bitValue = ((c & (1 << i)) != 0 );
			if (bitValue == 1)
			{
				nowNode = nowNode->rightChild;
			}
			else
			{
				nowNode = nowNode->leftChild;
			}
			if (nowNode->c != 0)
			{
				output << nowNode->c;
				nowNode = root;
			}
		}
		nowPos = input.tellg();
	}
	input.read((char*)&c, sizeof(c));
	for (int i = 7; i >= remaind; i--)
	{
		int bitValue = ((c & (1 << i)) != 0);
		if (bitValue == 1)
		{
			nowNode = nowNode->rightChild;
		}
		else
		{
			nowNode = nowNode->leftChild;
		}
		if (nowNode->c != 0)
		{
			output << nowNode->c;
			nowNode = root;
		}
	}
	cout << "解码成功！结果请见文件outputfile2.txt";
	input.close();
	output.close();
	return;
}

void Huffman::decode(path sour1, path sour2)
{
	map<string, char> maps2c;
	maps2c.clear();
	ifstream input1;
	input1.open(sour1);
	ifstream input2;
	input2.open(sour2, ios::binary);
	ofstream output;
	output.open("task4.txt");

	int nowChar = 0;
	string nowCode = "";
	char c[100];
	input1.getline(c, 100);
	input1.getline(c, 100);
	while (input1.getline(c, 100))
	{
		bool can = false;
		nowCode = "";
		int nowChar = c[0] == '\t' ? int('\n') : int(c[0]);
		for (int i = 0; i < 100 && c[i] != '\0'; i++)
		{
			if (can == true || (c[i - 1] == '\t' && c[i - 2] == '\t'))
			{
				nowCode += c[i];
				can = true;
			}
		}
		maps2c[nowCode] = char(nowChar);
		cout << int(nowChar) << ' ' << nowCode << endl;
	}
	input2.seekg(-1, ios::end);
	streampos endPos = input2.tellg();
	input2.seekg(ios::beg);
	streampos nowPos = input2.tellg();

	nowCode = "";
	char ch = 0;
	while (nowPos != endPos)
	{
		input2.read((char*)&ch, sizeof(ch));
		for (int i = 7; i >= 0; i--)
		{
			int bitValue = ((ch & (1 << i)) != 0);
			nowCode += char(bitValue + 48);
			if (maps2c.find(nowCode) != maps2c.end())
			{
				output << maps2c[nowCode];
				nowCode = "";
			}
		}
		nowPos = input2.tellg();
	}

	cout << "解码成功！结果请见文件task4.txt";
	input1.close();
	input2.close();
	output.close();
}

void Huffman::close()
{
	if (root == NULL || mapp == NULL)
	{
		cout << "Empty, Error!";
		return;
	}
	destoryTree(root);
	delete[] mapp;
	root = NULL;
	mapp = NULL;
	return;
}

int main()
{
	cout << "请输入用于生成编码的文本文件的路径：";
	string s1;
	cin >> s1;
	Huffman* huff = Huffman::open();
	huff->generateCode(s1);
	int op;
	cout << "编码/解码1/解码2？（1/0/-1）：";
	cin >> op;
	if (op == 1)//编码
	{
		string s2;
		cout << "请输入要编码的文本文件的路径：";
		cin >> s2;
		huff->code(s2);
	}
	else if (op == 0)//解码
	{
		string s2;
		cout << "请输入要解码的二进制文件的路径：";
		cin >> s2;
		huff->decode(s2);
	}
	else if (op == -1)
	{
		string sa, sb;
		cout << "请输入编码文件及二进制文件的路径：";
		cin >> sa >> sb;
		huff->decode(sa, sb);
	}
	huff->close();
	return 0;
}