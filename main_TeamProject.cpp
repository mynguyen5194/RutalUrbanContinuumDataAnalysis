//
//  main.cpp
//  TeamProject
//
//  Created by Team #2 on 11/10/14.
//  Copyright (c) 2014 Team #2. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include "County.h"
#include "HashTable.h"
#include "BinarySearchTree.h"
#include <stack>
#define LIST_SIZE 40
using namespace std;

// Functions prototypes
void display(County & county);
bool readFile (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree);
void menu (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, stack<County*> & Stack);
void searchCounty (HashTable<County> * countyHash);
void deleteCounty(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, stack<County*> & Stack);
void insert (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree);
void undoDelete(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, stack<County*> & Stack);
void eliminateComma(string & line);
int determineHashSize(int inFileCount);
bool isPrime(int checkPrime);
bool isNumeric(string numStr);
void Depth_First_Traversals(BinarySearchTree<County>* tree);
void showMenu();

int main() {
    const int hashTableSize = determineHashSize(LIST_SIZE);
    
    stack<County*> Stack;
    HashTable<County> * countyHash = new HashTable<County>(hashTableSize);
    BinarySearchTree<County> * countyTree = new BinarySearchTree<County>();
    
    readFile(countyHash, countyTree);
    menu(countyHash, countyTree, Stack);
    
    return 0;
}

// ********************************************
//  display function
// ********************************************
void display(County & county) {
    if (county.getKey() != 0 && county.getPopulation() != 0)
        cout << county.getKey() << "\t" << county.getState() << "\t" << county.getCounty()
            << "\t" << county.getPopulation() << "\t" << county.getRucc() << endl;
    return;
}

// ********************************************
//  readFile function
// ********************************************
bool readFile(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree) {
    ifstream inFile;
    int key, population, rucc;
    string line, state, county, pop, delimiter = "\t";
    bool empty = true;
    size_t endPos;
    County countyInfo;
    
    inFile.open("/Users/mynguyen5194/Desktop/TeamProject/TeamProject/TeamProject/RU_database.txt");
    if (!inFile) {
        cout << "Error opening file 'RU_database.txt'\n";
        return false;
    }
    while (getline(inFile, line)) {
        endPos = line.find(delimiter);
        key = atoi(line.substr(0, endPos).c_str());
        state = line.substr(endPos+1, 2);
        line.erase(0, endPos+4);
        
        endPos = line.find(delimiter);
        county = line.substr(0, endPos);
        line.erase(0, endPos+1);
        
        endPos = line.find(delimiter);
        pop = line.substr(0, endPos);
        eliminateComma(pop);
        
        population = atoi(pop.c_str());
        line.erase(0, endPos+1);
        
        rucc = atoi(line.substr(0, 1).c_str());
        
        countyInfo.setInfo(key, state, county, population, rucc);
        
        countyHash->insert(countyInfo);
        countyTree->insert(countyInfo);

        empty = false;
    }

    
    inFile.close();
    
    if (empty) {
        return false;
    }

    return true;
}

// ********************************************
//  menu function
// ********************************************
void menu(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, stack<County*> & Stack) {
    string option;
    
    do {
        cout << "Please enter your option: ";
        getline(cin, option);
        
        if (isalpha(option[0]) && option.length() == 1 && !countyHash->isEmpty()) {
        
            switch (toupper(option[0])) {
                case 'A':
                    // Add new data (both BST & hash table)
                    insert(countyHash, countyTree);
                    break;
            
                case 'D':
                    // Delete data (both BST & hash table)
                    deleteCounty(countyHash, countyTree, Stack);
                    break;
                
                case 'S':   // By both ??????? (hash is faster)
                    searchCounty(countyHash);
                    break;
                    
                case 'L':
                    cout << " ~*~ List data in hash table sequence ~*~ \n";
                    countyHash->displayList(display);
                    break;
                
                case 'P':
                    cout << " ~*~ Print Indented Hash Table ~*~ \n";
                    countyHash->printHashTable(display);
                    break;
                    
                case 'T':
                    // Hash statistics (info)
                    cout << " ~*~ Hash Table Statistics ~*~\n";
                    countyHash->statistics();
                    break;
                    
                case 'B':
                    //Tree Breadth-First Traversal: Print by level
                    if(!countyTree->Breadth_First_Traversal(display))
                        cout << "There is no data.\n";
                    break;

                case 'R':
                    // Recursive Depth-First Traversals:inorder, preorder, postorder
                    Depth_First_Traversals(countyTree);
                    break;

                case 'I':
                    // Print indented tree
                    countyTree->Print_Indented_Tree();
                    break;

                case 'Z':
                    // Undo delete
                    undoDelete(countyHash, countyTree, Stack);
                    break;
                    
                case 'W':
                    // Write data to a file
                    break;
        
                case 'M':
                    showMenu();
                    break;
                
                default:
                    break;
            }
        }
    } while (toupper(option[0]) != 'Q' && !countyHash->isEmpty());
    
    return;
}


// ********************************************
//  Search_county function
// ********************************************
void searchCounty (HashTable<County> * countyHash) {
    string state = "", county = "", key;
    int population = 0, rucc = 0;
    County countyInfo;
    cout << " ~*~ Search County Information ~*~\n";
    cout << "Please enter the county's key (FIPS): ";

    getline(cin, key);
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), state, county, population, rucc);
        if (countyHash->search(countyInfo, countyInfo))
            display(countyInfo);
        else
            cout << "The county is not in the list!\n";
    }
    
    else
        cout << "Invalid input!\n";
    return;
}

// ********************************************
//  deleteCounty function
// ********************************************
void deleteCounty(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, stack<County*> & Stack) {
    string state = "", county = "", key;
    int population = 0, rucc = 0;
    
    County countyInfo;
    cout << " ~*~ Delete County Information ~*~\n";
    cout << "Please enter the county's key (FIPS): ";
    getline(cin, key);
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), state, county, population, rucc);
        countyHash->remove(countyInfo, Stack);
        countyTree->remove(countyInfo, Stack);
    }
    
    else
        cout << "Invalid input!\n";
    return;
}

// ********************************************
//  undoDelete function
// ********************************************
void undoDelete(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, stack<County*> & Stack) {
    County * county;
    
    cout << " ~*~ Undo Delete County Information ~*~\n";
    if (Stack.top()) {                              // BREAK RIGHT HERE
        county = Stack.top();
        cout << "Reinsert ";
        display(*county);
    
        countyHash->insert(*county);
        countyTree->insert(*county);
    
        Stack.pop();
        Stack.pop();
    }
    else
        cout << "Stack is empty!\n";
    
    return;
}
// ********************************************
//  insert function
// ********************************************
void insert (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree) {
    string key = "", state = "", county = "", population = "", rucc = "";
    County countyInfo;
    
    cout << " ~*~ Insert Data ~*~\n";
    cout << "Please enter the key (FIPS): ";
    getline(cin, key);
    
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), state, county, atoi(population.c_str()), atoi(rucc.c_str()));
        if (countyHash->search(countyInfo, countyInfo) == false) {

            cout << "Valid key!\n";
            cout << "Please enter the state abbreviation: ";
            getline(cin, state);
            
            cout << "Please enter the county name: ";
            getline(cin, county);
            
            cout << "Please enter the county's population: ";
            getline(cin, population);
            eliminateComma(population);
            
            cout << "Please enter the county's RUCC (from 1 to 9): ";
            getline(cin, rucc);
            
            if (isNumeric(key) && state.length() == 2 && isNumeric(population) &&
                rucc.length() == 1 && atoi(rucc.c_str()) > 0 && atoi(rucc.c_str()) < 10) {
                
                countyInfo.setInfo(atoi(key.c_str()), state, county, atoi(population.c_str()), atoi(rucc.c_str()));
                countyHash->insert(countyInfo);
                countyTree->insert(countyInfo);
            }
        }
        else
            cout << "The key is already exist!\n";
    }

    return;
}

// *********************************************
//  eliminateComma function
// *********************************************
void eliminateComma (string & line) {
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == ',')
            line.erase(i, 1);   // Erase comma if any
    }
    return;
}

// ********************************************
//  determineHashSize Function
//  Define hash table size by counted number of
//  the lines in the input file, multiply it by 2,
//  and choose the next prime number.
//  Data in: int number of input file lines
//  Data out: int number of hash size
// ********************************************
int determineHashSize(int inFileCount) {
    inFileCount = inFileCount*2;
    int hashSize;
    
    int addToPrime = 1;
    
    while (!isPrime(inFileCount + addToPrime)) {
        addToPrime++;
    }
    
    hashSize = inFileCount + addToPrime;
    return hashSize;
}

// ********************************************
//  isPrime Function
//  Check if the received number is prime number or not.
//  Data in: int number
//  Data out: bool true or false
// ********************************************
bool isPrime(int checkPrime) {
    for (int index = 3; index < checkPrime; index += 2) {
        if ((checkPrime % index) == 0) {
            return false;
        }
    }
    return true;
}
//**********************************************
// Definition of isNumeric function
// This function checks if a string is numeric
// Return: True if the string is numeric
//         False if not numeric
//**********************************************
bool isNumeric(string numStr) {
    return all_of(numStr.begin(), numStr.end(), ::isdigit);
}

// ********************************************
//  Depth_First_Traversals function
// ********************************************
void Depth_First_Traversals(BinarySearchTree<County>* tree)
{
    if(!tree->getKey_root())
    {
        cout << "There is no data.\n";
        return;
    }
    cout<<"\nIn order: \n";
    tree->inOrder(display);
    cout<<"\nPost order: \n";
    tree->postOrder(display);
    cout <<"\nPre order: \n";
    tree->preOrder(display);
}

// ********************************************
//  menu function
// ********************************************
void showMenu() {
    cout << "\n ~*~ Menu ~*~ \n"
        << "A - Add new data \n"
        << "D - Delete data \n"
        << "S - Search and display one element using the primary key \n"
        << "L - List data in hash table sequence \n"
        << "K - List data in key sequence (sorted) \n"
        << "P - Print indented tree \n"
        << "W - Write data to a file \n"
        << "I - Hast statistics (info) \n"
        << "H - showMenu \n";
    return;
}