#include <stdio.h>
#include <vector>
#include <algorithm>
struct Node
{
	int data;
	Node *left, *right;
	Node(int val)
	{
		data = val;
		left = this;
		right = this;
	}
};
Node *Append(Node *cur, Node *p)
{
	p->left = cur;
	p->right = cur->right;

	cur->right = p;
	p->right->left = p;
	return p;
}
void PrintList(Node *l,bool rightOrder=true)
{
	if (!l) return;
	if (!l->left || !l->right) return;
	
	if (rightOrder)
	{
		Node *p = l->right;
		while (p != l)
		{
			printf("%d ", p->data);
			p = p->right;
		}
		
	}
	else
	{
		Node *p = l->left;
		while (p != l)
		{
			printf("%d ", p->data);
			p = p->left;
		}
	}

	printf("\n");
}
Node* CreateList()
{
	Node *head = new Node(0);
	int arr[3] = { 4, 1, 2 };
	int len = sizeof(arr) / sizeof(arr[0]);
	Node *cur = head;
	for (int i = 0; i < len; i++)
	{
		Node *p = new Node(arr[i]);
		Append(cur, p);
		cur = p;
	}
	return head;
}
bool MySort(const Node &lhs, const Node &rhs)
{
	return lhs.data < rhs.data;
}
void LSort1(Node *h)
{
	Node *p = h->right;

	std::vector<Node> vec;
	while (p!=h)
	{
		vec.push_back(*p);
		p = p->right;
	}
	std::sort(vec.begin(), vec.end(), MySort);
	p = h->right;
	int id = 0;
	Node *cur = h;
	while (p!=h)
	{
		cur->left = p;
		cur = p;
		Node *q = p->left;
		*p = vec[id++];
		p = q;
	}
	cur->left = h;
}
typedef Node * NodePtr;
bool MySorts(NodePtr &lhs, NodePtr &rhs)
{
	return lhs->data < rhs->data;
}
void LSort1s(Node *h)
{
	Node *p = h->right;
	
	std::vector<NodePtr> vec;
	while (p != h)
	{
		vec.push_back(p);
		p = p->right;
	}
	std::sort(vec.begin(), vec.end(), MySorts);

	p = h;
	for (int i = 0; i < vec.size(); i++)
	{
		p->left = vec[i];
		p = vec[i];
	}
	p->left = h;
}
void LSort2(Node *h)
{
	if (!h) return;
	Node *pSorted = h;

	while (pSorted->left!=h)
	{
		Node *p = pSorted->left;
		Node *pmin = p;

		//find min node
		Node *prev = p;
		while (p != h)
		{
			if (p->data < pmin->data)
			{
				pmin = p;
			}
			prev = p;
			p = p->left;
		}
		//remove pmin in left list.
		prev->left = p->left;

		//insert pmin after pSorted
		pmin->left = pSorted->left;
		pSorted->left = pmin;
		pSorted = pmin;
	}
	
}
int main()
{
	Node *p = CreateList();
	PrintList(p, 1);
	PrintList(p, 0);
	LSort2(p);
	PrintList(p, 1);
	PrintList(p, 0);
	return 0;
}