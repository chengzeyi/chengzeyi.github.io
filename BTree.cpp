#include <iostream>
#include <string>
#include <stack>
#include <queue>

using namespace std;

struct BTNode
{
	char data;
	BTNode *lChild, *rChild;
	BTNode(const char &data)
	{
		this->data = data;
		this->lChild = NULL;
		this->rChild = NULL;
	}
	BTNode()
	{
		this->lChild = NULL;
		this->rChild = NULL;
	}
};

class BTree
{
private:
	BTNode head; // 仅左子树有效

	int GetHeight(BTNode *ptr)const
	{
		if (ptr == NULL)
			return 0;
		else
			return GetHeight(ptr->lChild) > GetHeight(ptr->rChild) ? GetHeight(ptr->lChild) + 1 : GetHeight(ptr->rChild) + 1;
	}

public:
	/*只支持小写字母*/
	BTree(const string &s)
	{
		stack<BTNode *>nodeStack;
		stack<int>statusStack;
		nodeStack.push(&this->head);
		statusStack.push(0);
		for (string::const_iterator iter = s.cbegin(); iter != s.cend(); iter++)
		{
			if(*iter >= 'a' && *iter <= 'z')
				switch (statusStack.top())
				{
				case 0: nodeStack.top()->lChild = new BTNode(*iter); break;
				case 1: nodeStack.top()->rChild = new BTNode(*iter); break;
				}
			else if (*iter == ',')
				statusStack.top() = 1;
			else if(*iter == '(')
				switch (statusStack.top())
				{
				case 0:
					nodeStack.push(nodeStack.top()->lChild);
					statusStack.push(0);
					break;
				case 1:
					nodeStack.push(nodeStack.top()->rChild);
					statusStack.push(0);
					break;
				}
			else if (*iter == ')')
			{
				nodeStack.pop();
				statusStack.pop();
			}
		}
	}
	string LDR()const
	{
		string s;
		stack<BTNode *>nodeStack;
		stack<int>statusStack;
		if (this->head.lChild != NULL)
		{
			nodeStack.push(this->head.lChild);
			statusStack.push(0);
		}
		while (nodeStack.empty() == false)
		{
			switch (statusStack.top())
			{
			case 0:
				if (nodeStack.top()->lChild != NULL)
				{
					nodeStack.push(nodeStack.top()->lChild);
					statusStack.top() = 1;
					statusStack.push(0);
				}
				else
					statusStack.top() = 1;
				break;
			case 1:
				s += nodeStack.top()->data;
				statusStack.top() = 2;
				break;
			case 2:
				if (nodeStack.top()->rChild != NULL)
				{
					nodeStack.push(nodeStack.top()->rChild);
					statusStack.top() = 3;
					statusStack.push(0);
				}
				else
					statusStack.top() = 3;
				break;
			case 3:
				nodeStack.pop();
				statusStack.pop();
				break;
			}
		}
		return s;
	}
	string LRD()const
	{
		string s;
		stack<BTNode *>nodeStack;
		stack<int>statusStack;
		if (this->head.lChild != NULL)
		{
			nodeStack.push(this->head.lChild);
			statusStack.push(0);
		}
		while (nodeStack.empty() == false)
		{
			switch (statusStack.top())
			{
			case 0:
				if (nodeStack.top()->lChild != NULL)
				{
					nodeStack.push(nodeStack.top()->lChild);
					statusStack.top() = 2;
					statusStack.push(0);
				}
				else
					statusStack.top() = 2;
				break;
			case 1:
				s += nodeStack.top()->data;
				statusStack.top() = 3;
				break;
			case 2:
				if (nodeStack.top()->rChild != NULL)
				{
					nodeStack.push(nodeStack.top()->rChild);
					statusStack.top() = 1;
					statusStack.push(0);
				}
				else
					statusStack.top() = 1;
				break;
			case 3:
				nodeStack.pop();
				statusStack.pop();
				break;
			}
		}
		return s;
	}
	string LevelTraversal()const
	{
		string s;
		queue<BTNode *>nodeQueue;
		if(this->head.lChild != NULL)
			nodeQueue.push(this->head.lChild);
		while (nodeQueue.empty() == false)
		{
			s += nodeQueue.front()->data;
			if (nodeQueue.front()->lChild != NULL)
				nodeQueue.push(nodeQueue.front()->lChild);
			if (nodeQueue.front()->rChild != NULL)
				nodeQueue.push(nodeQueue.front()->rChild);
			nodeQueue.pop();
		}
		return s;
	}
	void SwapLR()
	{
		queue<BTNode *>nodeQueue;
		if (this->head.lChild != NULL)
			nodeQueue.push(this->head.lChild);
		while (nodeQueue.empty() == false)
		{
			BTNode * tPtr = nodeQueue.front()->lChild;
			nodeQueue.front()->lChild = nodeQueue.front()->rChild;
			nodeQueue.front()->rChild = tPtr;
			if (nodeQueue.front()->lChild != NULL)
				nodeQueue.push(nodeQueue.front()->lChild);
			if (nodeQueue.front()->rChild != NULL)
				nodeQueue.push(nodeQueue.front()->rChild);
			nodeQueue.pop();
		}
	}
	string ShowByLevel()const
	{
		string s;
		queue<BTNode *>nodeQueue;
		int height = GetHeight(this->head.lChild), n = 1;
		nodeQueue.push(this->head.lChild);
		while (height > 0)
		{
			s += '\n';
			for (int i = 0; i < n; i++)
			{
				int space = 1;
				for (int i = 0; i < height; i++)
					space *= 2;
				space -= 1;
				if (i == 0)
					for (int j = 0; j < (space + 1) / 2 - 1; j++)
						s += ' ';
				else
					for (int j = 0; j < space; j++)
						s+= ' ';
				if (nodeQueue.front() == NULL)
				{
					s+= ' ';
					nodeQueue.push(NULL);
					nodeQueue.push(NULL);
					nodeQueue.pop();
				}
				else
				{
					s += nodeQueue.front()->data;
					nodeQueue.push(nodeQueue.front()->lChild);
					nodeQueue.push(nodeQueue.front()->rChild);
					nodeQueue.pop();
				}
			}
			s += '\n';
			height--;
			n *= 2;
		}
		return s;
	}
};

void test()
{
	string s;
	cout << "Enter String:" << endl; // a(b(c(d(e,f),g),h),i(j(,k(,l(m,n))),o(p(q(r),s),t(u(v,w),x))))
	cin >> s;
	BTree btree(s);
	cout << "LDR:" << endl << btree.LDR() << endl;
	cout << "LRD:" << endl << btree.LRD() << endl;
	cout << "Level Traversal:" << endl << btree.LevelTraversal() << endl;
	cout << "Show By Level:" << endl << btree.ShowByLevel() << endl;
	btree.SwapLR();
	cout << "Show By Level After Swap:" << endl << btree.ShowByLevel() << endl;
}

int main()
{
	test();
	return 0;
}