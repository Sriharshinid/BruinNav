// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#ifndef MYMAP_H
#define MYMAP_H
#include "provided.h"


template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap()
	{
		m_root = nullptr;
		m_size = 0;
	}
	~MyMap()
	{
		clear();
	}
	void clear()
	{
		Node* ptr = m_root;
		deleteNodes(ptr);
		m_root = nullptr;
	}
	int size() const { return m_size; }
	void associate(const KeyType& key, const ValueType& value)
	{
		ValueType* val = find(key);

		if (val != nullptr)
			*(val) = value;
		else
			insertNewNode(m_root, key, value);
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
	{
		const ValueType* nodePtr = findNode(key, m_root);
		return nodePtr;
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	int m_size;
	struct Node
	{
		KeyType m_key;
		ValueType m_value;
		Node *left;
		Node *right;
	};
	Node* m_root;
	
	
	ValueType* findNode(const KeyType& key, Node* ptr) const
	{
		if (ptr == nullptr)
			return nullptr;
		else if (ptr->m_key == key)
			return &(ptr->m_value);
		else
		{
			if (key < ptr->m_key)
				return findNode(key, ptr->left);
			else 
				return findNode(key, ptr->right);
		}	
	}
	void insertNewNode(Node* &ptr, KeyType key, ValueType value)
	{
		if (ptr == nullptr)
		{
			ptr = new Node;
			ptr->m_key = key;
			ptr->m_value = value;
			ptr->left = nullptr;
			ptr->right = nullptr;
			m_size++;
			return;
		}
		if (key < ptr->m_key)
			return insertNewNode(ptr->left, key, value);
		else 
			return insertNewNode(ptr->right, key, value);
	}

	void deleteNodes(Node* temp)
	{
		if (temp == nullptr)
			return;
		deleteNodes(temp->left);
		deleteNodes(temp->right);
		delete temp;
		temp = nullptr;
		m_size--;
	}

};


#endif
