// main test driver for BST
// Created by Frank M. Carrano and Tim Henry.
// modified by Nghia Nguyen
// Email : fatboy_dn_2193@yahoo.com
// IDE: Visual Studio
// Extension until Sunday
 
#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"


template<class ItemType>
class BinarySearchTree : public BinaryTree<ItemType> {
private:
	// internal insert node: insert newNode in nodePtr subtree
	BinaryNode<ItemType>* _insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode);
   
	// internal remove node: locate and delete target node under nodePtr subtree
	BinaryNode<ItemType>* _remove(BinaryNode<ItemType>* nodePtr, const int target, bool & success, stack<ItemType*> & Stack);
   
	// delete target node from tree, called by internal remove node
	BinaryNode<ItemType>* deleteNode(BinaryNode<ItemType>* targetNodePtr);
   
	// remove the leftmost node in the left subtree of nodePtr
	BinaryNode<ItemType>* removeLeftmostNode(BinaryNode<ItemType>* nodePtr, ItemType & successor);
   
	// search for target node
	BinaryNode<ItemType>* findNode(BinaryNode<ItemType>* treePtr, const int & target) const;
   

public:
	// insert a node at the correct location
    bool insert(const ItemType & newEntry);
	// remove a node if found
	bool remove(const int & anEntry, stack<ItemType*> & Stack);
	// find a target node
	bool getEntry(const int & target, ItemType & returnedItem) const;
	// Print sub tree
	bool getGreater(const int & target,void visit(ItemType &)) const;
	void _subGreater(const int & target, void visit(ItemType &), BinaryNode<ItemType> *nodePtr) const;

};


///////////////////////// public function definitions ///////////////////////////

template<class ItemType>
bool BinarySearchTree<ItemType>::insert(const ItemType & newEntry)
{
	BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
	this->rootPtr = _insert(this->rootPtr, newNodePtr);
	
	return true;
}  

template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const int & target, stack<ItemType*> & Stack)
{
	bool isSuccessful = false;
	this->rootPtr = _remove(this->rootPtr, target, isSuccessful, Stack);
	return isSuccessful; 
}  

template<class ItemType>
bool BinarySearchTree<ItemType>::getEntry(const int& anEntry, ItemType & returnedItem) const
{
	BinaryNode<ItemType> *found = findNode(this->rootPtr, anEntry);
	if(found)
	{
		returnedItem = found->getItem();
		return true;
	}
	return false;
}  

template<class ItemType>
bool BinarySearchTree<ItemType>::getGreater(const int & target, void visit(ItemType &)) const 
{
	if(!this->rootPtr)
		return false;
	BinaryNode<ItemType> *pWalk = this->rootPtr;
	BinaryNode<ItemType> *nodePtr;
	while(pWalk)
	{
		nodePtr = pWalk;
		pWalk = pWalk->getRightPtr();
	}
	if(nodePtr->getItem().getKey() <= target)
		return false;
	_subGreater(target, visit, this->rootPtr);
	return true;
}

template<class ItemType>
void BinarySearchTree<ItemType>::_subGreater(const int & target, void visit(ItemType &), BinaryNode<ItemType> *nodePtr) const
{
	if(!nodePtr)
		return;
	
	if(nodePtr->getItem().getKey() < target  )
		_subGreater(target, visit, nodePtr->getRightPtr());
	
	else if(nodePtr->getItem().getKey() > target)
		{
			_subGreater(target, visit, nodePtr->getLeftPtr());
			visit(nodePtr->getItem());
			if(nodePtr->getRightPtr())
				inOrderRight(visit,nodePtr->getRightPtr());			
		}
	else if (nodePtr->getItem().getKey() == target && nodePtr->getRightPtr())
		inOrderRight(visit, nodePtr->getRightPtr());
}

//////////////////////////// private functions ////////////////////////////////////////////

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr,// nodeptr is root
                                                          BinaryNode<ItemType>* newNodePtr)
{
	if (!nodePtr)
	{
		this->count++;
		return newNodePtr;
	}
	else
	{
		if (newNodePtr->getItem().getKey() >= nodePtr->getItem().getKey())
			nodePtr->setRightPtr(_insert(nodePtr->getRightPtr(), newNodePtr));
		else
			nodePtr->setLeftPtr(_insert(nodePtr->getLeftPtr(), newNodePtr));
	}
    return nodePtr;
}  

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr,
                                                          const int target,
                                                          bool & success, stack<ItemType*> & Stack)

{
	if (nodePtr == 0)                   
	{
		success = false;
		return 0;
	}
	if (nodePtr->getItem().getKey() > target)		 
		nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success, Stack));
	else if (nodePtr->getItem().getKey() < target)	 
		nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success, Stack));
	else		
	{
        ItemType * delPtr = new ItemType(nodePtr->getItem());
        Stack.push(delPtr);
		nodePtr = deleteNode(nodePtr);
		success = true;
	}      
	return nodePtr;   
}  

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::deleteNode(BinaryNode<ItemType>* nodePtr)
{
	if (nodePtr->isLeaf())				
	{
		delete nodePtr;
		nodePtr = 0;
		return nodePtr;
	}
	else if (nodePtr->getLeftPtr() == 0)  
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getRightPtr();
        
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else if (nodePtr->getRightPtr() == 0) 
	{
		BinaryNode<ItemType>* nodeToConnectPtr = nodePtr->getLeftPtr();
        
        delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;      
	}
	else                                  
	{
		ItemType newNodeValue;
		nodePtr->setRightPtr(removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
		nodePtr->setItem(newNodeValue);
		return nodePtr;
	}  
}  

template<class ItemType> 
BinaryNode<ItemType>* BinarySearchTree<ItemType>::removeLeftmostNode(BinaryNode<ItemType>* nodePtr,
                                                                     ItemType & successor)
{
	if (nodePtr->getLeftPtr() == 0)
	{
		successor = nodePtr->getItem();
		return deleteNode(nodePtr);
	}
	else 
	{
		nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
		return nodePtr;
	}       
}

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::findNode(BinaryNode<ItemType>* nodePtr,
                                                           const int & target) const 
{
	if(!nodePtr)
		return nodePtr;
	BinaryNode<ItemType> *pWalk = nodePtr;
	while(pWalk)
	{
		if(target < pWalk->getItem().getKey())
			pWalk = pWalk->getLeftPtr();
		else
			if(target > pWalk->getItem().getKey())
				pWalk = pWalk->getRightPtr();
			else 
				return pWalk;
	}
	return pWalk;
}  

#endif
