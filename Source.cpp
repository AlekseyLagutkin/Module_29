#include <iostream>
#include <mutex>

using namespace std;

struct Node
{
	Node() : next(nullptr) {}
	int value;
	Node* next;
	mutex node_mutex;
};

class FineGrainedQueue
{
	Node* head;
	mutex queue_mutex;
public:
	FineGrainedQueue() : head(nullptr) {}
	void insertIntoMiddle(int value, int pos)
	{
		Node* prew, * cur;
		bool curmutex=false;
		queue_mutex.lock();
		Node* newnode = new Node;
		newnode->value = value;
		int curpos = 2;   
		prew = this->head;
		cur = this->head->next;
		prew->node_mutex.lock();
		queue_mutex.unlock();
		if (cur)
		{
			cur->node_mutex.lock();
		}
		while (prew)
		{
			if ((curpos == pos) || (curpos < pos && cur == nullptr))
			{
				prew->next = newnode;
				newnode->next = cur;
				prew->node_mutex.unlock();
				if (cur)
				{
					cur->node_mutex.unlock();
				}
				return;
			}
			Node* old_prew = prew;
			prew = cur;
			cur = cur->next;
			curpos++;
			old_prew->node_mutex.unlock();
			if(cur)
			{
				cur->node_mutex.lock();
			}
		}
		prew->node_mutex.unlock();
	}
	void pushfirst(int value)   //                    
	{
		Node* newnode = new Node;
		newnode->value = value;
		head = newnode;
	}
	void shownodes()   //                     
	{
		Node* curnode = head;
		while (curnode != nullptr)
		{
			cout << curnode->value << endl;
			curnode = curnode->next;
		}
	}
};

int main()
{
	FineGrainedQueue fg;
	fg.pushfirst(1);
	fg.insertIntoMiddle(3, 3);
	fg.insertIntoMiddle(8, 2);
	fg.insertIntoMiddle(18, 10);
	fg.shownodes();
	return 0;
}