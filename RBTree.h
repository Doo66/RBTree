#include <iostream>
using namespace std;

enum Colour
{
	RED,
	BLACK,
};

template <class K, class V>
struct RBTreeNode
{
	typedef RBTreeNode<K, V> Node;

	Node* _left;
	Node* _right;
	Node* _parent;

	K _key;
	V _value;

	Colour _Col;
	
	RBTreeNode(const K& key, const V& value)
		:_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_key(key)
		,_value(value)
		,_Col(RED)
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}
public:
	bool Insert(const K& key, const V& value)
	{
		if(_root == NULL)
		{
			_root = new Node(key, value);
			_root->_Col = BLACK;
			return true;
		}
		Node* cur = _root;
		Node* parent = NULL;
		
		//找到要插入节点的位置
		while(cur)
		{
			if(cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if(cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		//插入节点
		cur = new Node(key, value);
		if(parent->_key > key)
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}
		cur->_parent = parent;

		//调整节点颜色，是其再次成为RBTree
		while(cur != _root && parent && parent->_Col == RED)
		{
			Node* grandfather = parent->_parent;
			if(grandfather->_left == parent)
			{
				Node* uncle = grandfather->_right;
				if(uncle && uncle->_Col == RED)
				{
					uncle->_Col = parent->_Col = BLACK;
					grandfather->_Col = RED;

					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if(parent->_right == cur)
					{
						_RotateL(parent);
					}
					parent->_Col = BLACK;
					grandfather->_Col =  RED;

					_RotateR(grandfather);
					break;
				}
			}
			else
			{
				Node* uncle = grandfather->_left;
				if(uncle && uncle->_Col == RED)
				{
					uncle->_Col = BLACK;
					parent->_Col = BLACK;
					grandfather->_Col = RED;

					cur= grandfather;
					parent = cur->_parent;
				}
				else
				{
					if(parent->_left == cur)
					{
						_RotateR(parent);
					}
					parent->_Col = BLACK;
					grandfather->_Col = RED;
					_RotateL(grandfather);

					break;
				}
			}
		}
		_root->_Col = BLACK;
		return true;
	}
	Node* Find(const K& key)
	{
		Node* cur = _root;
		while(cur)
		{
			if(cur->_key > key)
			{
				cur = cur->_left;
			}
			else
			{
				cur = cur->_right;
			}
		}
	}
	bool CheckBalance()
	{
		int BlackCount = 0; //一条路径上黑色节点的个数
		int CurBlackCount = 0;	//当前路径黑色节点的个数
		Node* cur = _root;
		while(cur)
		{
			if(cur->_Col == BLACK)
			{
				++BlackCount;
			}
			cur = cur->_left;
		}

		return _CheckBalance(_root, BlackCount, CurBlackCount);
		
	}
	void InOrder()
	{
		_InOrder(_root);
		cout<<endl;
	}
protected:
	void _InOrder(Node* root)
	{
		if(root == NULL)
		{
			return;
		}
		_InOrder(root->_left);
		cout<<root->_key<<" ";
		_InOrder(root->_right);
	}

	bool _CheckBalance(Node* root, int BlackCount, int CurBlackCount)
	{
		if(root == NULL)
		{
			return true;
		}

		if(root->_Col == BLACK)
		{
			++CurBlackCount;
		}
		else
		{
			if(root->_parent && root->_parent->_Col == RED)
			{
				cout<<"Not Balance! Abnormal Node: "<<root->_key<<endl;
				return false;
			}
		}
		if(root->_left == NULL && root->_right == NULL)
		{
			if(BlackCount == CurBlackCount)
			{
				return true;
			}
			else
			{
				cout<<"Not Balance!"<<root->_key<<endl;
				return false;
			}
		}
		return _CheckBalance(root->_left, BlackCount, CurBlackCount) && _CheckBalance(root->_right, BlackCount, CurBlackCount);
	}
	void _RotateL(Node*& parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		parent->_right = subRL;
		if(subRL)
		{
			subRL->_parent = parent;
		}
		subR->_left = parent;
		Node* ppNode = parent->_parent;
		subR->_parent = ppNode;
		if(ppNode == NULL)
		{
			_root = subR;
		}
		else
		{
			if(ppNode->_left == parent)
			{
				ppNode->_left = subR;
			}
			else
			{
				ppNode->_right = subR;
			}
		}
	}
	void _RotateR(Node*& parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		parent->_right = subLR;
		if(subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		Node* ppNode = parent->_parent;
		subL->_parent = ppNode;
		if(ppNode == NULL)
		{
			_root = subL;
		}
		else
		{
			if(ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}
		}
		
	}
	
private:
	Node* _root;
};

void Test()
{
	RBTree<int, int> RB;
	int arr[] = { 1, 4, 6, 8, 0, 2, 3, 5, 9, 7 };  
	for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
	{
		RB.Insert(arr[i], i);
	}
	RB.InOrder();
	cout<<RB.CheckBalance()<<endl;
}