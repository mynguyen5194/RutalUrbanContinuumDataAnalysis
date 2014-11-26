// main test driver for BST
// Created by Frank M. Carrano and Tim Henry.
// modified by Nghia Nguyen
// Email : fatboy_dn_2193@yahoo.com
// IDE: Visual Studio
// Extension until Sunday

#ifndef _BINARY_TREE
#define _BINARY_TREE

#include <iomanip>
#include "BinaryNode.h"
#include <queue>
#include <stack>
template<class ItemType>
class BinaryTree
{
protected:
	BinaryNode<ItemType>* rootPtr;		// ptr to root node
	int count;							// number of nodes in tree
public:
	// "admin" functions
 	BinaryTree() {rootPtr = 0; count = 0;}
	BinaryTree(const BinaryTree<ItemType> & tree){ count = 0; rootPtr = copyTree(tree.rootPtr, count); }
	virtual ~BinaryTree() {clear(); }
	BinaryTree & operator = (const BinaryTree & sourceTree);
   
	// common functions for all binary trees
 	bool isEmpty() const	{return count == 0;}
	int size() const	    {return count;}
	void clear()			{destroyTree(rootPtr); rootPtr = 0; count = 0;}
	void preOrder(void visit(ItemType &)) const {_preorder(visit, rootPtr);}
	void inOrder(void visit(ItemType &)) const  {_inorder(visit, rootPtr);}
	void postOrder(void visit(ItemType &)) const{_postorder(visit, rootPtr);}
	void inOrderRight(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const {_inorder(visit, nodePtr);}
	void InOrder_Interative(void visit(ItemType &)) const;
	void PostOrder_Interative(void visit(ItemType &)) const;
	void PreOrder_Interative(void visit(ItemType &)) const;
	int getcount() { return count; }
	int getKey_root() const {return rootPtr->getItem().getKey();}
	bool Breadth_First_Traversal(void visit(ItemType &));
	bool Print_Indented_Tree();
	

	// abstract functions to be implemented by derived class
	virtual bool insert(const ItemType & newData) = 0; 
	virtual bool remove(const ItemType & data, stack<ItemType*> & Stack) = 0;
	virtual bool getEntry(const ItemType & anEntry, ItemType & returnedItem) const = 0;

private:   
	// delete all nodes from the tree
	void destroyTree(BinaryNode<ItemType>* nodePtr);

	//Print indented tree
	void _print_indented(BinaryNode<ItemType>* nodePtr, int level, int space);

	// copy from the tree rooted at nodePtr and returns a pointer to the copy
	BinaryNode<ItemType>* copyTree(const BinaryNode<ItemType>* nodePtr, int &acount);

	// internal traverse
	void _preorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
	void _inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
	void _postorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
   
}; 

//////////////////////////////////////////////////////////////////////////

template<class ItemType>
void BinaryTree<ItemType>::PostOrder_Interative(void visit(ItemType &)) const
{


}

template<class ItemType>
void BinaryTree<ItemType>:: InOrder_Interative(void visit(ItemType &)) const
{
	if(!rootPtr)
		return;
	stack <BinaryNode<ItemType>*> astack;
	BinaryNode<ItemType> *nodePtr = rootPtr;
	
	bool done = false;
	while (!done)
	{
		if(nodePtr)
		{
			astack.push(nodePtr);
			nodePtr = nodePtr->getLeftPtr();
		}
		else
		{	if(!astack.empty())
			{
				nodePtr = astack.top();
				visit(nodePtr->getItem());
				astack.pop();
				nodePtr = nodePtr->getRightPtr();
			}//if
			else done = true;	
		}//else
	}//while
}


template<class ItemType>
void BinaryTree<ItemType>:: PreOrder_Interative(void visit(ItemType &)) const
{
	if(!rootPtr)
		return;
	stack <BinaryNode<ItemType>*> astack;
	BinaryNode<ItemType> *nodePtr;
	astack.push(rootPtr);
	while(!astack.empty())
	{
		nodePtr = astack.top();
		visit(nodePtr->getItem());
		astack.pop();
		if(nodePtr->getRightPtr())
			astack.push(nodePtr->getRightPtr());
		if(nodePtr->getLeftPtr())
			astack.push(nodePtr->getLeftPtr());
	}
	return;

}

template<class ItemType>
bool BinaryTree<ItemType>::Print_Indented_Tree()
{
	if(!rootPtr)
		return false;
	int level = 1, space = 0;
	_print_indented(rootPtr, level, space);
	return true;
}

template<class ItemType>
void BinaryTree<ItemType>::_print_indented(BinaryNode<ItemType>* nodePtr, int level, int space)
{
	if(!nodePtr)
	{
		cout << setw(space) <<level <<".\n";
		return;
	}
	cout << setw(space) <<level <<"." << nodePtr->getItem().getKey() << "\t" << nodePtr->getItem().getState() << "\t" << nodePtr->getItem().getCounty()
		 << "\t" << nodePtr->getItem().getPopulation() << "\t" << nodePtr->getItem().getRucc() << endl;
	
		_print_indented(nodePtr->getLeftPtr(), ++level, space=space+5);
		_print_indented(nodePtr->getRightPtr(),level, space);
}

template<class ItemType>
bool BinaryTree<ItemType>::Breadth_First_Traversal(void visit(ItemType &))
{
	if(!rootPtr)
		return false;
	
	queue<BinaryNode<ItemType>*> aqueue;
	aqueue.push(rootPtr);
	while(!aqueue.empty())
	{
		BinaryNode<ItemType>*nodePtr = aqueue.front();
        ItemType item = nodePtr->getItem();
        visit(item);
		
		if(nodePtr->getLeftPtr())
			aqueue.push(nodePtr->getLeftPtr());

		if(nodePtr->getRightPtr())
			aqueue.push(nodePtr->getRightPtr());
		aqueue.pop();
	}
	return true;
}

template<class ItemType>
BinaryNode<ItemType>* BinaryTree<ItemType>::copyTree(const BinaryNode<ItemType>* nodePtr, int & acount) 
{

	if(!nodePtr)
		return NULL;
	County acounty;
	BinaryNode<ItemType> *Tree_Copy = new BinaryNode<ItemType>(acounty);
		Tree_Copy->setItem(nodePtr->getItem()); 
		acount ++;
		Tree_Copy->setRightPtr(copyTree(nodePtr->getRightPtr(), acount));
		Tree_Copy ->setLeftPtr(copyTree(nodePtr->getLeftPtr(), acount));
	
	return Tree_Copy;	
}  

template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
	if(nodePtr)
	{
		destroyTree(nodePtr->getLeftPtr());
		destroyTree(nodePtr->getRightPtr());
		delete(nodePtr);
	}
}  

template<class ItemType>
void BinaryTree<ItemType>::_preorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr != 0)
	{
		ItemType item = nodePtr->getItem();
		visit(item);
		_preorder(visit, nodePtr->getLeftPtr());
		_preorder(visit, nodePtr->getRightPtr());
	} 
}  

template<class ItemType>
void BinaryTree<ItemType>::_inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr !=0)
	{
		ItemType item = nodePtr->getItem();
		_inorder(visit, nodePtr->getLeftPtr());
		visit(item);
		_inorder(visit, nodePtr->getRightPtr());
	}
}  

template<class ItemType>
void BinaryTree<ItemType>::_postorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const
{
	if (nodePtr !=0)
	{
		ItemType item = nodePtr->getItem();
		_postorder(visit, nodePtr->getLeftPtr());
		_postorder(visit, nodePtr->getRightPtr());
		visit(item);
	}
}  

template<class ItemType>
BinaryTree<ItemType> & BinaryTree<ItemType>::operator=(const BinaryTree<ItemType> & sourceTree)
{
	count = 0;
	rootPtr = copyTree(sourceTree.rootPtr, count);
	return *this;
}  

#endif

