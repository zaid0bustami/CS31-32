#include "Sequence.h"

Sequence::Sequence()    // Create an empty sequence (i.e., one with no items)
{
	
}
Sequence::Sequence(const Sequence& other)
{
	if (other.m_head == nullptr)
	{
		m_head = nullptr;
	}
	else
	{
		//make a pointer to the other list and make a new node
		Node * otherCurrent = other.m_head;
		Node* ourCurrent = new Node;
		m_head = ourCurrent;
		ourCurrent->val = otherCurrent->val;
		//create nodes until the end of the other node is reached
		while(otherCurrent->next != other.m_head)
		{
			ourCurrent->next = new Node;
			ourCurrent->next->val = otherCurrent->next->val;
			ourCurrent->next->prev = ourCurrent;
			ourCurrent = ourCurrent->next;
			otherCurrent = otherCurrent->next;
		}
		//link the ends of the circular list
		ourCurrent->next = m_head;
		m_head->prev = ourCurrent;
	}
}
Sequence::~Sequence()
{
	for (int i = 0; i < size();)
	{
		erase(0);
	}
}
Sequence& Sequence::operator=(const Sequence& other)
{
	if (this != &other)
	{
		Sequence temp = other;
		swap(temp);
	}
	return(*this);
}
void Sequence::dump() const
{
	std::cerr << "L";
	if (m_head != nullptr)
	{
		Node* p = m_head;
		do
		{
			std::cerr << "-(" << p->val << ")";
			p = p->next;
		} while (p != m_head);
	}
	std::cerr << std::endl;
}
bool Sequence::empty() const  // Return true if the sequence is empty, otherwise false.
{
	if (m_head == nullptr)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}
int Sequence::size() const    // Return the number of items in the sequence.
{
	if (empty())
	{
		return (0);
	}
	else
	{
		int s = 0;
		for (Node* p = m_head; p->next != m_head; p = p->next)
		{
			s++;
		}
		return(s + 1);
	}
}
int Sequence::insert(int pos, const ItemType& value)
{
	if (pos <= size() && pos >= 0)
	{
		Node* q = new Node;
		q->val = value;
		if (empty()) //assigns head to the new node if the sequence was empty to begin with
		{
			m_head = q;
		}
		else
		{
			Node* p = m_head;
			for (int i = 0; i < pos; i++)
			{
				p = p->next;
			}
			q->next = p;         //inserts the node
			q->prev = p->prev;
			p->prev->next = q;
			p->prev = q;
			if (pos == 0) //adjusts the head if the node was inserted at the beginning
			{
				m_head = q;
			}
		}
		return(pos);
	}
	else
	{
		return(-1);
	}
}
// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one greater than they were at before.
// Return pos if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// e.g., because it's implemented using a fixed-size array.)  Otherwise,
// leave the sequence unchanged and return -1.  Notice that
// if pos is equal to size(), the value is inserted at the end.

int Sequence::insert(const ItemType& value) //does not return -1 because a linked list is not fixed capacity
{
	Node* p = m_head;
	int i = 0;
	int pos = -1;
	do
	{
		if (p == nullptr || value <= p->val)
		{
			pos = i;
			break;
		}
		else
		{
			i++;
			p = p->next;
		}
	} while (p != m_head);
	if (pos == -1)
	{
		pos = size();
	}
	return(insert(pos, value));
}
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item in position
// p.  The original item at position p and those that follow it end
// up at positions one greater than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).

bool Sequence::erase(int pos)
{
	if (pos < size() && pos >= 0)
	{
		Node* q = m_head;
		if (size() == 1)
		{
			m_head = nullptr;
		}
		else if (size() > 1)
		{
			for (int i = 0; i < pos; i++)
			{
				q = q->next;
			}
			q->prev->next = q->next; //makes sure no pointers point to q
			q->next->prev = q->prev;
			if (pos == 0)
			{
				m_head = m_head->next; //makes sure that m_head doesn't become a dangling pointer
			}
		}
		delete q;
		return(true);
	}
	else
	{
		return(false);
	}
}
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed that item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.

int Sequence::remove(const ItemType& value)
{
	int count = 0;
	Node* p = m_head;
	ItemType toCompare;
	for (int i = 0; i < size(); i++)
	{
		toCompare = p->val;  //store's p->value so that p can be moved
		p = p->next;         //ensures that p doesn't become a dangling pointer
		if (toCompare == value)
		{
			erase(i);
			count++;
			i--;
		}
	}
	return(count);
}
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < size() && pos >= 0)
	{
		Node* p = m_head;
		for (int i = 0; i < pos; i++)
		{
			p = p->next;
		}
		value = p->val;
		return(true);
	}
	else
	{
		return(false);
	}
}
// If 0 <= pos < size(), copy into value the item at position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < size() && pos >= 0)
	{
		Node* p = m_head;
		for (int i = 0; i < pos; i++)
		{
			p = p->next;
		}
		p->val = value;
		return(true);
	}
	else
	{
		return(false);
	}
}
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.

int Sequence::find(const ItemType& value) const
{
	Node* p = m_head; //identical code to insert without pos but the pos is returned
	int i = 0;
	int pos = -1;
	if (p != nullptr)
	{
		do
		{
			if (value == p->val)
			{
				pos = i; //pos only chnaged if a  match is found, so remains -1 if no matches
				break;
			}
			else
			{
				i++;
				p = p->next;
			}
		} while (p != m_head);
	}
	return(pos);
}
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.

void Sequence::swap(Sequence& other)
{
	Node* p = m_head;
	m_head = other.m_head;
	other.m_head = p;
}
// Exchange the contents of this sequence with the other one.

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if (seq2.empty())
	{
		return(-1);
	}
	int k = -1, n = 0;
	ItemType x, y;
	for (int i = 0; i < seq1.size(); i++)
	{
		seq1.get(i, x); //no for loop to go through seq2 because we want to read through it in the order that it matches seq1
		seq2.get(n, y); //so the index n won't increment until we have a match
		if (x == y && n == 0)
		{
			k = i; //sets the position to the beginning of the match
			n++;
		}
		else if (x == y && n > 0)
		{
			n++; //we just incrase the size of the match
		}
		else if (x != y && n > 0)
		{
			k = -1;
			break;
		}
		if (n == seq2.size())
		{
			n--; //decrements because we want a count starting from 0
			break;
		}
	}
	if (k + n < seq1.size())
	{
		return(k);
	}
	else
	{
		return(-1);
	}
}
// Consider all the items in seq2; let's call them seq20, seq21, ..., seq2n.
// If there exists at least one k such that seq1k == seq20 and seq1k+1 == seq21 
// and ... and seq1k+n == seq2n, and k+n < seq1.size(), then this function 
// returns the smallest such k. (In other words, if seq2 is a consecutive subsequence 
// of seq1, the function returns the earliest place in seq1 where that subsequence 
// starts.) If no such k exists or if seq2 is empty, the function returns -1

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	ItemType x, y;
	Sequence r;
	if (seq1.size() == seq2.size())
	{
		if (seq1.size() == 0)
		{
			r = seq1;
		}
		for (int i = 0; i < seq1.size(); i++)
		{
			seq1.get(i, x);
			seq2.get(i, y);
			r.insert(2 * i, x);
			r.insert((2 * i) + 1, y);
		}
	}
	else if (seq1.size() > seq2.size())
	{
		if (seq2.size() == 0)
		{
			r = seq1;
		}
		else 
		{
			for (int i = 0; i < seq2.size(); i++)
			{
				seq1.get(i, x);
				seq2.get(i, y);
				r.insert(2 * i, x);
				r.insert((2 * i) + 1, y);
			}
			for (int i = 0; i < seq1.size() - seq2.size(); i++)
			{
				seq1.get(i + seq2.size(), x);
				r.insert(2 * seq2.size() + i, x);
			}
		}
	}
	else if (seq1.size() < seq2.size())
	{
		if (seq1.size() == 0)
		{
			r = seq2;
		}
		else
		{
			for (int i = 0; i < seq1.size(); i++)
			{
				seq1.get(i, x);
				seq2.get(i, y);
				r.insert(2 * i, x);
				r.insert((2 * i) + 1, y);
			}
			for (int i = 0; i < seq2.size() - seq1.size(); i++)
			{
				seq2.get(i + seq1.size(), x);
				r.insert(2 * seq1.size() + i, x);
			}
		}
	}
	result = r; //intermediate sequence takes care of aliasing
}
// This function produces as a result a sequence that consists of the first item in 
// seq1, then the first in seq2, then the second in seq1, then the second in seq2, etc.