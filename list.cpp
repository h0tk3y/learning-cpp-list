#include "stdafx.h"
#define BOOST_TEST_MODULE testList
#include <boost/test/unit_test.hpp>

struct List
{
	List()
	{
		head = Node(-1, nullptr, nullptr);
		tail = Node(-1, &head, nullptr);
		head.next = &tail;
		begin = iterator(&head);
		end = iterator(&tail);
	}

	List(List const& from)
	{
		head = Node(-1, nullptr, nullptr);
		tail = Node(-1, &head, nullptr);
		head.next = &tail;
		begin = iterator(&head);
		end = iterator(&tail);
		for (iterator i = iterator(from.head.next); i.node_ptr->next != nullptr; i = i++)
			insert(&tail, *i);
	}

	List& operator=(List const& from)
	{
		List l = List();
		for (iterator i = iterator(from.head.next); i.node_ptr->next != nullptr; i = i++)
			l.insert(&l.tail, *i);
		return l;
	}

	~List()
	{
		iterator it = iterator(head.next);
		while (it.node_ptr->next != nullptr)
		{
			it++;
			delete it.node_ptr->prev;
		}
	}

	void push_back(int x)
	{
		insert(&tail, x);
	}

	void pop_back()
	{
		erase(tail.prev);
	}

	int back()
	{
		return end.node_ptr->prev->value;
	}

	void push_front(int x)
	{
		insert(head.next, x);
	}

	void pop_front()
	{
		erase(head.next);
	}

	int front()
	{
		return begin.node_ptr->next->value;
	}

	struct Node
	{
	public:
		int value;
		Node* next;
		Node* prev;

		Node() {};

		Node(int v, Node* prev, Node* next)
		{
			this->value = v;
			this->next = next;
			this->prev = prev;
		}
	};

	Node head, tail;

	struct iterator 
	{
		Node* node_ptr;

		iterator(){};

		iterator(Node* i)
		{
			this->node_ptr = i;
		}

		int& operator*() const
		{
			return node_ptr->value;
		}

		iterator& operator++()
		{
			this->node_ptr = &(*node_ptr->next);
		}

		iterator operator++(int)
		{
			return iterator(node_ptr->next);
		}

		iterator& operator--()
		{
			this->node_ptr = &(*node_ptr->prev);
		}

		iterator operator--(int)
		{
			return iterator(node_ptr->prev);
		}
	};

	iterator begin;
	iterator end;

	void erase(Node* what)
	{
		what->next->prev = what->prev;
		what->prev->next = what->next;
		delete what;
	}

	iterator insert(Node* before, int x)
	{ 
		before->prev = new Node(x, before->prev, before);
		before->prev->prev->next = before->prev;
		return iterator(before->prev);
	}
};

/*
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}
*/

BOOST_AUTO_TEST_CASE(testList)
{
	List lst = List();
	lst.push_back(1);
	lst.push_back(2);
	lst.push_back(3);
	lst.push_front(0);
	List copied = lst;
	BOOST_CHECK_EQUAL(copied.front(), 0);
	copied.pop_front();
	BOOST_CHECK_EQUAL(copied.front(), 1);
	copied.pop_back();
	BOOST_CHECK_EQUAL(copied.back(), 2);
	BOOST_CHECK_EQUAL(lst.front(), 0);
	BOOST_CHECK_EQUAL(lst.back(), 3);
}

