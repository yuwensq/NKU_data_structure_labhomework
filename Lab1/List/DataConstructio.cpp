/*************************************************
Author:NANAXi
Date:2021-09-18
Description:Realize word frequency statistics of an
    English reading material through list
**************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;

/*
* 每个节点记录单词及词频
*/
struct Node
{
    string word;
    int frequency;
    Node* next;
	Node() {
		word = "";
		frequency = 0;
		next = nullptr;
	};
	Node(string w, int freq) {
		word = w;
		frequency = freq;
		next = nullptr;
	};
};

/*
* 构建链表类
*/
class List {
private:
    int size[26];//记录每一个小链表的长度
    Node* current[26];//每一个小链表的当前指针
    Node* head[26];//每一个小链表的表头
public:
    List();
	~List();
    void add(string, int);//向链表中添加节点
    void quickSort();//利用排序对链表各个小链表排序
	void qSort(Node*, Node*);
	void swap(Node*, Node*);
	void show();
	void moveCurrent(string);//根据单词移动current指针
	void insert(string, int);
};

void importData(map<string, int>&);//初始化单词词频对照表

List* solve(map<string, int>const);//插入并排序

bool isCharacter(char);

int main()
{
    map<string, int> wordsFrequency;//单词词频对照表
	importData(wordsFrequency);
	List* list;
	list = solve(wordsFrequency);
	list->show();
    return 0;
}


void importData(map<string, int>& wordsFrequency)
{
	/*************************************************
	Description:从txt文件中统计单词及词频
	Input:map<string, int>& wordsFrequency 用于记录结果
	Return:void
	*************************************************/
	fstream stopWords("stopwordstable.txt");
	map<string, int> hashStopWords;//停词哈希表
	hashStopWords.clear();
	string stopWord;
	while (stopWords >> stopWord)
	{
		hashStopWords.insert(pair<string, int>(stopWord, 1));
	}
	string source;
	//cin >> source;
	fstream inputText("text.txt");
	string now = "";//记录当前单词
	wordsFrequency.clear();
	char c, c0;
	inputText.get(c);
	c0 = c;
	while (!inputText.eof())
	{
		char c1;
		inputText.get(c1);
		if (c >= 'a' && c <= 'z')
		{
			now += c;
		}
		else if ((c == '\'' || c == '-' || c == '.') && isCharacter(c0) && isCharacter(c1))
		{
			now += c;
		}
		else if (c >= 'A' && c <= 'Z')
		{
			now += (char)(c + 32);
		}
		else if (now != "")
		{
			if (hashStopWords.find(now) == hashStopWords.end())
			{
				if (wordsFrequency.find(now) == wordsFrequency.end())
					wordsFrequency.insert(pair<string, int>(now, 1));
				else
					wordsFrequency[now]++;
			}
			now = "";
		}
		c0 = c;
		c = c1;
	}
	if (now != "")
	{
		if (hashStopWords.find(now) == hashStopWords.end())
		{
			if (wordsFrequency.find(now) == wordsFrequency.end())
				wordsFrequency.insert(pair<string, int>(now, 1));
			else
				wordsFrequency[now]++;
		}
		now = "";
	}
	return;
}

List* solve(map<string, int> wordsFrequency)
{
	/*************************************************
	Description:生成List对象，将结果录入到链表中，
		之后按照词频排序，返回对象指针
	Input:map<string, int> wordsFrequency 单词词频对照表
	Return:List*
	*************************************************/
	List* list = new List();
	map<string, int>::iterator it;
	for (it = wordsFrequency.begin(); it != wordsFrequency.end(); it++)
	{
		list->add(it->first, it->second);
	}
	list->quickSort();
	return list;
}

bool isCharacter(char c)
{
	if (c >= 'a' && c <= 'z')
		return true;
	if (c >= 'A' && c <= 'Z')
		return true;
	return false;
}

List::List()
{
	for (int i = 0; i < 26; i++)
	{
		size[i] = 0;
		head[i] = new Node;
		current[i] = head[i];
	}
}

List::~List()
{
	for (int i = 0; i < 26; i++)
	{
		Node* p = head[i];
		while (p->next != NULL)
		{
			Node* q = p;
			p = p->next;
			delete q;
		}
		delete p;
	}
}

void List::add(string word, int freq)
{
	/*************************************************
	Description:向链表中添加项
	Calls:moveCurren(string), insert(string, int)
	Input:string word 单词，int freq 词频
	Return:void
	*************************************************/
	moveCurrent(word);
	insert(word, freq);
}

void List::moveCurrent(string word)
{
	/*************************************************
	Description:移动链表的current指针以进行下一步插入操作
		将current移动到word的直接前驱处
	Input:string word 单词
	Return:void
	*************************************************/
	int i = word[0] - 'a';//获得索引
	if (current[i]->next != nullptr && current[i]->next->word < word)//word在current[i]指针之后
	{
		while (current[i]->next != nullptr && current[i]->next->word < word)
		{
			current[i] = current[i]->next;
		}
	}
	else//word在current[i]指针之前
	{
		current[i] = head[i];
		while (current[i]->next != nullptr && current[i]->next->word < word)
		{
			current[i] = current[i]->next;
		}
	}
	return;
}

void List::insert(string word, int freq)
{
	/*************************************************
	Description:将节点插入到链表中
	Input:string word 节点单词, int freq 节点词频
	Return:void
	*************************************************/
	Node* newNode = new Node(word, freq);
	int i = word[0] - 'a';
	newNode->next = current[i]->next;
	current[i]->next = newNode;
	size[i]++;
	return;
}

void List::quickSort()
{
	/*************************************************
	Description:使用快速排序对26个链表排序
	Calls:qSort(Node*, Node*)
	Input:NULL
	Return:void
	*************************************************/
	for (int i = 0; i < 26; i++)
	{
		Node* tail = head[i];
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		qSort(head[i]->next, tail);
	}
}

void List::qSort(Node* head, Node* tail)
{
	/*************************************************
	Description:使用快速排序对单个链表排序
	Input:Node* head, 链表头, Node* tail, 链表尾
	Return:void
	*************************************************/
	if (head == tail || head == NULL)
		return;
	Node* p, * q;
	p = q = head;
	int myKey = head->frequency;
	while (q != tail)
	{
		q = q->next;
		if (q->frequency > myKey)
		{
			p = p->next;
			swap(p, q);
		}
	}
	swap(head, p);
	qSort(head, p);
	qSort(p->next, tail);
}

void List::swap(Node* p, Node* q)
{
	/*************************************************
	Description:交换两个结点的值
	Input:Node* p, *q 两个不同结点
	Return:void
	*************************************************/
	int temp = p->frequency;
	p->frequency = q->frequency;
	q->frequency = temp;
	string tempw = p->word;
	p->word = q->word;
	q->word = tempw;
	return;
}

void List::show()
{
	for (int i = 0; i < 26; i++)
		cout << size[i] << ' ';
	cout << endl;
	for (int i = 0; i < 26; i++)
	{
		Node* p = head[i];
		while (p->next != nullptr)
		{
			cout << p->next->word << " " << p->next->frequency << endl;
			p = p->next;
		}
	}
	return;
}
