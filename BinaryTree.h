// main test driver for BST
// Created by Frank M. Carrano and Tim Henry.

#ifndef _BINARY_TREE
#define _BINARY_TREE

#include <iomanip>
#include "BinaryNode.h"
#include "Stack.h"
#include "Queue.h"
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
	int getKey_root() const {return rootPtr->getItem().getKey();}
	bool Breadth_First_Traversal(void visit(ItemType &));
	bool Print_Indented_Tree(void visit(ItemType &));
    void preOrder_outFile(void visit_outFile(ItemType &, ofstream & outFile), ofstream & outFile)
        const {_preorderOutFile(visit_outFile, rootPtr, outFile);}

	// abstract functions to be implemented by derived class
	virtual bool insert(const ItemType & newData) = 0; 
	virtual bool remove(const ItemType & data, Stack<ItemType*> & Stack) = 0;
	virtual bool getEntry(const ItemType & anEntry, ItemType & returnedItem) const = 0;

private:   
	// delete all nodes from the tree
	void destroyTree(BinaryNode<ItemType>* nodePtr);

	//Print indented tree
	void _print_indented(BinaryNode<ItemType>* nodePtr, int level, int space, void visit(ItemType &));

	// copy from the tree rooted at nodePtr and returns a pointer to the copy
	BinaryNode<ItemType>* copyTree(const BinaryNode<ItemType>* nodePtr, int &acount);

	// internal traverse
	void _preorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
	void _inorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
	void _postorder(void visit(ItemType &), BinaryNode<ItemType>* nodePtr) const;
    void _preorderOutFile(void visit_outFile(ItemType &, ofstream & outFile), BinaryNode<ItemType>* nodePtr, ofstream & outFile) const;
   
}; 

//////////////////////////////////////////////////////////////////////////
template<class ItemType>
bool BinaryTree<ItemType>::Print_Indented_Tree(void visit(ItemType &))
{
	if(!rootPtr)
		return false;
	int level = 1, space = 0;
	_print_indented(rootPtr, level, space, visit);
	return true;
}

template<class ItemType>
void BinaryTree<ItemType>::_print_indented(BinaryNode<ItemType>* nodePtr, int level, int space, void visit(ItemType &))
{
	if(!nodePtr)
	{
		cout << setw(space) <<level <<".\n";
		return;
	}
    ItemType item = nodePtr->getItem();
    cout << setw(space) << level <<".";
    visit(item);
		_print_indented(nodePtr->getLeftPtr(), ++level, space=space+3, visit);
		_print_indented(nodePtr->getRightPtr(),level, space, visit);
}

template<class ItemType>
bool BinaryTree<ItemType>::Breadth_First_Traversal(void visit(ItemType &))
{
	if(!rootPtr)
		return false;
	
	Queue<BinaryNode<ItemType>*> aqueue;
	aqueue.enqueue(rootPtr);
	while(!aqueue.isEmpty())
	{
        BinaryNode<ItemType>*nodePtr;
        aqueue.queueFront(nodePtr);
        ItemType item = nodePtr->getItem();
        visit(item);
		
		if(nodePtr->getLeftPtr())
			aqueue.enqueue(nodePtr->getLeftPtr());

		if(nodePtr->getRightPtr())
			aqueue.enqueue(nodePtr->getRightPtr());
		aqueue.dequeue(nodePtr);
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
void BinaryTree<ItemType>::_preorderOutFile(void visit_outFile(ItemType &, ofstream & outFile), BinaryNode<ItemType>* nodePtr, ofstream & outFile) const
{
    if (nodePtr != 0)
    {
        ItemType item = nodePtr->getItem();
        visit_outFile(item, outFile);
        _preorderOutFile(visit_outFile, nodePtr->getLeftPtr(), outFile);
        _preorderOutFile(visit_outFile, nodePtr->getRightPtr(), outFile);
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

