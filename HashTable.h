//
//  HashTable.h
//  lab5
//
//  Created by Team #2 on 11/12/14.
//  Copyright (c) 2014 Team #2. All rights reserved.
//
#include <iostream>
#include <stack>

using namespace std;

#ifndef lab5_HashTable_h
#define lab5_HashTable_h

//#define TABLE_SIZE 15

template <class ItemType>
class HashTable {
private:
    ItemType * table;
    int loadNumber;
    int numberOfCollision;
    int TABLE_SIZE;
    
public:
   
    HashTable(const int hashSize);
    ~HashTable();
    
    bool isEmpty();
    int hash(const ItemType & newEntry);
    bool insert(const ItemType & newEntry);
    bool search(const ItemType & target, ItemType & returnTarget);
    bool remove(const ItemType target, stack<ItemType*> & Stack);
    void printItem(ItemType * nodePtr, bool displayList, void visit(ItemType &));
    void displayList(void visit(ItemType &));
    void printHashTable(void visit(ItemType &));
    void statistics();
    int countNode(int index);

};

template <class ItemType>
HashTable<ItemType>::HashTable(const int hashSize) {
    TABLE_SIZE = hashSize;
    table = new ItemType[TABLE_SIZE];
    loadNumber = 0;
    numberOfCollision = 0;
}

// **************************************
//  Destructor
// **************************************
template <class ItemType>
HashTable<ItemType>::~HashTable<ItemType>() {
    ItemType * currentPtr;
    ItemType * nextPtr;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        currentPtr = table[i].getNext();
        table[i].setNext(NULL); // Disconnect linked list from hash table
        
        // Start deleting the linked list from the beginning
        while (currentPtr) {
            nextPtr = currentPtr;
            currentPtr = currentPtr->getNext();
            delete nextPtr;
        }
    }
    delete [] table;
}

// *************************************************
//  isEmpty function
//  This function checks if the hash table is empty
//  Return: true if empty
//          false if not
// *************************************************
template <class ItemType>
bool HashTable<ItemType>::isEmpty() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i].getKey() == 0) {
            return false;
        }
    }
    return true;
}

// *************************************************
//  hash function
//  This function finds the sum of the newEntry as
//  a title by converting each character to ascii
//  and summing them up.
//  Return: a hashed number
// *************************************************
template <class ItemType>
int HashTable<ItemType>::hash(const ItemType & newEntry) {
    
    int index = (newEntry.getKey() * 13);
    
    return (index % TABLE_SIZE);
}

// ******************************************************
//  insert function
//  This function inserts new data into the hash table.
//  If the place in the hash table has been already taken
//  it will create and insert in a singly linked list
//  Return: true if inserted successfully
//          false if not
// ******************************************************
template<class ItemType>
bool HashTable<ItemType>::insert(const ItemType & newEntry) {
    bool success = false;
    ItemType * newNode = new ItemType(newEntry);
    int index = hash(newEntry); // Get the index
    ItemType * nodePtr = &table[index];
        
    // if the place in the hash table is empty
    if (table[index].getKey() == 0) {
        table[index] = *newNode;
        loadNumber++;
        success = true;
    }
        
    // if not, then create a linked list at this index
    else {
        while (nodePtr->getNext())
            nodePtr = nodePtr->getNext();
        nodePtr->setNext(newNode);

        numberOfCollision++;
        success = true;
    }
    
    return success;
}

// *************************************************
//  search function
//  This function searches for the year of the movie
//  by using the title as the key.
//  Return: true if found
//          false if not
// *************************************************
template <class ItemType>
bool HashTable<ItemType>::search(const ItemType & target, ItemType & returnTarget) {
    bool found = false;
    
    int index = hash(target);   // Get the index of the entered value
    ItemType * nodePtr = &table[index];
    
    while (nodePtr) {
        if (nodePtr->getKey() == target.getKey()) {
            returnTarget = *nodePtr;
            found = true;
        }
        nodePtr = nodePtr->getNext();
    }
    
    if (!found)
        found = false;
    
    return found;
}

// **************************************************
//  remove function
// **************************************************
template <class ItemType>
bool HashTable<ItemType>::remove(const ItemType target, stack<ItemType*> &Stack) {
    bool deleted = false;
    ItemType * delPtr;
    
    int index = hash(target);
    
    if (table[index].getKey() != 0) {
        ItemType * tempPtr = NULL;
        ItemType * nodePtr = &table[index];
        while (nodePtr->getNext() != NULL && nodePtr->getKey() != target.getKey()) {
            tempPtr = nodePtr;
            nodePtr = nodePtr->getNext();
        }
        
        if (nodePtr->getKey() == target.getKey()) {
            // If the target is found in the hash table
            if (tempPtr == NULL) {
                delPtr = new ItemType(table[index]);
                if (nodePtr->getNext() == NULL) {
                    Stack.push(delPtr);
                    table[index].setInfo(0, "", "", 0, 0);  // Overwrite the data to delete the element in the hash table
                    deleted = true;
                }
                else {
                    Stack.push(delPtr);
                    table[index].setInfo(nodePtr->getNext()->getKey(), nodePtr->getNext()->getState(), nodePtr->getNext()->getCounty(),
                                         nodePtr->getNext()->getPopulation(), nodePtr->getNext()->getRucc());
                    deleted = true;
                }
            }
            // If the target is found in the linked lists
            else {
                Stack.push(nodePtr);
                delete nodePtr;
                deleted = true;
                tempPtr->setNext(nodePtr->getNext());
            }
        }
    }
    else {
        cout << "The county is not in the list!\n";
        return deleted;
    }
    
    return deleted;
}

// ********************************************
//  printItem function
//  This function prints out the year and title
//  Return: none
// ********************************************
template <class ItemType>
void HashTable<ItemType>::printItem(ItemType * nodePtr, bool displayList , void visit(ItemType &)) {
    ItemType item;

    item.setInfo(nodePtr->getKey(), nodePtr->getState(), nodePtr->getCounty(),
                 nodePtr->getPopulation(), nodePtr->getRucc());
    if (!displayList) {
        cout << "\t\t\t  ";
    }
    visit(item);

    return;
}

// **********************************************
//  displayList function
//  This function displays every item in the file
//  Return: none
// **********************************************
template <class ItemType>
void HashTable<ItemType>::displayList(void visit(ItemType &)) {
    // Print the hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        ItemType * nodePtr = &table[i];
        printItem(nodePtr, true, visit);
        
        // Print the linked list if any
        while (nodePtr->getNext()) {
            nodePtr = nodePtr->getNext();
            printItem(nodePtr, true, visit);
        }
    }
    return;
}

// **************************************************
//  printHashTable function
//  This function prints out the indented hash table
//  by showing the index and what elements are in the
//  linked list
//  Return: none
// **************************************************
template <class ItemType>
void HashTable<ItemType>::printHashTable(void (visit)(ItemType &)) {
    // Print the hash table and the index
    for (int i = 0; i < TABLE_SIZE; i++) {
        ItemType * nodePtr = &table[i];
        cout << "Index " << i << ": ";
        if (nodePtr->getKey() != 0) {
            printItem(nodePtr, true, visit);
            
            // Print the linked list if any
            while (nodePtr->getNext()) {
                nodePtr = nodePtr->getNext();
                printItem(nodePtr, false, visit);
            }
        }
        else
            cout << "// Empty" << endl;
    }
    return;
}

// **************************************
//  countNode function
//  This function counts the number of nodes
//  in a linked list
//  Return: number of node if any
//          otherwise return 0
// **************************************
template <class ItemType>
int HashTable<ItemType>::countNode(int index) {
    int number = 0;
    ItemType * nodePtr = &table[index];
    
    if (!nodePtr)
        return 0;
    
    else {
        while (nodePtr->getNext()) {
            nodePtr = nodePtr->getNext();
            number++;
        }
    }
    return number;
}
// **************************************
//  statistics function
//  This function display some statistic info
//  about the hash table
//  Return: none
// **************************************
template <class ItemType>
void HashTable<ItemType>::statistics() {
    cout << "Number of Collision: " << numberOfCollision << endl;
    cout << "Load Factor: " << (loadNumber*100)/TABLE_SIZE << "%\n";
    
    double numberOfList = 0;
    double totalNode = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        totalNode += countNode(i);
        if (table[i].getNext())
            numberOfList++;
    }
    cout << "Number of Linked Lists: " << numberOfList << endl;
    
    int longest = countNode(0);
    for (int i = 1; i < TABLE_SIZE; i++) {
        if (longest <= countNode(i))
            longest = countNode(i);
    }
    
    cout << "Total Nodes in the Linked Lists: " << totalNode << " node(s)\n";
    cout << "Longest Linked List Contains : " << longest << " node(s)\n";
    if (numberOfList > 0)
        cout << "Average Number of Nodes Stored in Linked Lists: " << totalNode/numberOfList << endl;
    else
        cout << "Average Number of Nodes Stored in Linked Lists: 0" << endl;
    return;
}
#endif