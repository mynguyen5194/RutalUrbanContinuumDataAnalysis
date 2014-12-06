//
//  main.cpp
//  TeamProject
//
//  Created by Team #2 on 11/10/14.
//  Copyright (c) 2014 Team #2. All rights reserved.
//

/**********************
 PROGRAM DOCUMENTATION
 
 Prorgram proccess county data using hashed tables and Binary search trees for various functions.
 The program is menu driven with the following options:
 
 print through hash table
 print through BST
 add new data
 delete data
 undo deletions
 write to file
 search by key
 show menu
 *************************/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "County.h"
#include "HashTable.h"
#include "BinarySearchTree.h"
#include "Stack.h"

#define LIST_SIZE 40
using namespace std;

// Functions prototypes
void display(County & county);
bool readFile(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree);
void outFileDisplay(County & county, ofstream & outFile);
void menu(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
bool insert(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree);
bool searchCounty(HashTable<County> * countyHash);
bool deleteCounty(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
void undoDelete(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
void Depth_First_Traversals(BinarySearchTree<County>* tree);
void writeToFile(BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
void eliminateComma(string & line);
int determineHashSize(int inFileCount);
bool isPrime(int checkPrime);
bool isNumeric(string numStr);
void briefDisplay(County & county);
void showMenu();

int main() {
    const int hashTableSize = determineHashSize(LIST_SIZE);
    
    Stack<County*> Stack;
    HashTable<County> * countyHash = new HashTable<County>(hashTableSize);
    BinarySearchTree<County> * countyTree = new BinarySearchTree<County>();
    
    readFile(countyHash, countyTree);
    menu(countyHash, countyTree, Stack);
    
    delete countyHash;
    delete countyTree;
    
    return 0;
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
void menu(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack) {
    string option;
    
    do {
        cout << "Please enter your option: ";
        getline(cin, option);
        
        if (isalpha(option[0]) && option.length() == 1 && !countyHash->isEmpty() && !countyTree->isEmpty()) {
            
            switch (toupper(option[0])) {
                case 'A':
                    cout << " ~*~ Insert Data ~*~\n";
                    if(!insert(countyHash, countyTree))
                        cout << "The key is already exist or invalid data.\n\n";
                    break;
                    
                case 'D':
                    cout << " ~*~ Delete County Information ~*~\n";
                    if (!deleteCounty(countyHash, countyTree, Stack))
                        cout << "County is not in the list\n\n";
                    else
                        cout << "County deleted\n\n";
                    break;
                    
                case 'S':
                    cout << " ~*~ Search County Information ~*~\n";
                    searchCounty(countyHash);
                    break;
                    
                case 'K':
                    cout << " ~*~ List data in hash table sequence ~*~ \n";
                    countyTree->inOrder(display);
                    break;
                    
                case 'L':
                    cout << " ~*~ List data in hash table sequence ~*~ \n";
                    countyHash->displayList(display);
                    break;
                    
                case 'P':
                    cout << " ~*~ Print Hash Table ~*~ \n";
                    countyHash->printHashTable(display);
                    break;
                    
                case 'T':
                    cout << " ~*~ Hash Table Statistics ~*~\n";
                    countyHash->statistics();
                    break;
                    
                case 'B':
                    cout << " ~*~ Tree Breadth-First Traversal ~*~\n";
                    if(!countyTree->Breadth_First_Traversal(display))
                        cout << "There is no data.\n";
                    break;
                    
                case 'R':
                    cout << " ~*~ Recursive Depth-First Traversals ~*~\n";
                    Depth_First_Traversals(countyTree);
                    break;
                    
                case 'I':
                    cout << " ~*~ Print indented tree ~*~ \n";
                    countyTree->Print_Indented_Tree(briefDisplay);
                    break;
                    
                case 'Z':
                    cout << " ~*~ Undo Delete County Information ~*~\n";
                    undoDelete(countyHash, countyTree, Stack);
                    break;
                    
                case 'W':
                    cout << "~*~ Saving data to 'RU_updated.txt' ~*~ \n\n";
                    writeToFile(countyTree, Stack);
                    break;
                    
                case 'M':
                    cout << "\n ~*~ Menu ~*~ \n";
                    showMenu();
                    break;
                    
                default:
                    break;
                    
            }
        }
    } while (toupper(option[0]) != 'Q');
    
    //automatic writeToFile
    writeToFile(countyTree, Stack);
    
    return;
}


// ********************************************
//  insert function
// ********************************************
bool insert (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree) {
    string key = "", state = "", county = "", population = "", rucc = "";
    County countyInfo;
    
    cout << "Please enter the key (FIPS): ";
    getline(cin, key);
    
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), "", "", 0, 0);
        if (countyHash->search(countyInfo, countyInfo) == false) {
            
            cout << "Valid key!\n";
            cout << "Please enter the state abbreviation: ";
            getline(cin, state);
            if (isNumeric(state) || state.length() != 2)
                return false;
            
            cout << "Please enter the county name: ";
            getline(cin, county);
            
            cout << "Please enter the county's population: ";
            getline(cin, population);
            eliminateComma(population);
            if(!isNumeric(population))
                return false;
            
            cout << "Please enter the county's RUCC (from 1 to 9): ";
            getline(cin, rucc);
            if(!isNumeric(rucc) || atoi(rucc.c_str()) < 0 || atoi(rucc.c_str()) > 9)
                return false;
            
            countyInfo.setInfo(atoi(key.c_str()), state, county, atoi(population.c_str()), atoi(rucc.c_str()));
            countyHash->insert(countyInfo);
            countyTree->insert(countyInfo);
            
            cout << "County added successfully!\n\n";
        }
        
        else
            return false;
        
        return true;
    }
    return false;
}

// ********************************************
//  Search_county function
// ********************************************
bool searchCounty (HashTable<County> * countyHash) {
    bool found = false;
    string key;
    County countyInfo;
    
    cout << "Please enter the county's key (FIPS): ";
    
    getline(cin, key);
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), "", "", 0, 0);
        if (countyHash->search(countyInfo, countyInfo)) {
            display(countyInfo);
            found = true;
        }
        else {
            cout << "The county is not in the list!\n\n";
            found = false;
        }
    }
    
    else
        cout << "Invalid input!\n\n";
    return found;
}

// ********************************************
//  deleteCounty function
// ********************************************
bool deleteCounty(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack) {
    string key;
    bool deleted = false;
    County countyInfo;
    
    cout << "Please enter the county's key (FIPS): ";
    getline(cin, key);
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), "", "", 0, 0);
        if (countyHash->search(countyInfo, countyInfo)) {
            countyHash->remove(countyInfo);
            countyTree->remove(countyInfo, Stack);
            deleted = true;
        }
    }
    
    return deleted;
}

// ********************************************
//  undoDelete function
// ********************************************
void undoDelete(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack) {
    County * county;
    
    if (!Stack.isEmpty()) {
        if (Stack.getTop(county)) {
            
            // Print and reinsert if the top county in the stack doesn't exist
            if(!countyHash->search(*county, *county)) {
                cout << "Reinsert ";
                display(*county);
                
                if (Stack.pop(county)) {
                    countyHash->insert(*county);
                    countyTree->insert(*county);
                }
            }
            else {  // Pop and display error message if the county already exists
                cout << "County already exists!\n";
                Stack.pop(county);
            }
        }
    }
    else
        cout << "Stack is empty!\n\n";
    
    return;
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
    
    cout <<"\t\t -----Pre order----- \n";
    tree->preOrder(display);
    cout<<"\t\t -----In order----- \n";
    tree->inOrder(display);
    cout<<"\t\t -----Post order----- \n";
    tree->postOrder(display);
}

// ********************************************
//  writeToFile function
// ********************************************
void writeToFile(BinarySearchTree<County> * countyTree, Stack<County*> & Stack) {
    County * county;
    ofstream outFile;
    outFile.open("/Users/mynguyen5194/Desktop/TeamProject/TeamProject/TeamProject/TeamProject_OutputFile.txt");
    
    countyTree->preOrder_outFile(outFileDisplay, outFile);
    
    outFile.close();
    
    while(!Stack.isEmpty())
        Stack.pop(county);
    
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
    int addToPrime = 1;
    
    while (!isPrime(inFileCount + addToPrime))
        addToPrime++;
    
    return (inFileCount + addToPrime);
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
//  display function: display the county to
//  with description
// ********************************************
void display(County & county) {
    
    //array of descriptions correlating to RUCC number
    string arrayDescriptions[9] = {"Metro - Counties in metro areas of 1 million population or more",
        "Metro - Counties in metro areas of 250,000 to 1 million population",
        "Metro - Counties in metro areas of fewer than 250,000 population",
        "Nonmetro - Urban population of 20,000 or more, adjacent to a metro area",
        "Nonmetro - Urban population of 20,000 or more, not adjacent to a metro area",
        "Nonmetro - Urban population of 2,500 to 19,999, adjacent to a metro area",
        "Nonmetro - Urban population of 2,500 to 19,999, not adjacent to a metro area",
        "Nonmetro - Completely rural or less than 2,500 urban population, adjacent to a metro area",
        "Nonmetro - Completely rural or less than 2,500 urban population, not adjacent to a metro area"};
    
    if (county.getKey() != 0 && county.getPopulation() != 0)
        cout << county.getKey() << "  " << county.getState() << "  " << county.getCounty()
        << "  " << county.getPopulation() << endl << "\t" << arrayDescriptions[county.getRucc() - 1] << endl << endl;
    return;
}
// ********************************************
//  display function: display the county to screen
// with no description
// ******************************************
void briefDisplay(County & county) {
    
    if (county.getKey() != 0 && county.getPopulation() != 0)
        cout << county.getKey() << "  " << county.getState() << "  " << county.getCounty()
        << "  " << county.getPopulation() << "  " << county.getRucc() << endl;
    return;
}

// ********************************************
//  outFileDisplay function: display the county
//  to outFile
// ********************************************
void outFileDisplay(County & county, ofstream & outFile) {
    
    if (county.getKey() != 0 && county.getPopulation() != 0)
        outFile << county.getKey() << "\t" << county.getState() << "\t" << county.getCounty()
        << "\t" << county.getPopulation() << "\t" << county.getRucc() << endl;
    return;
}

// ********************************************
//  menu function
// ********************************************
void showMenu() {
    cout << "A - Add new data \n"
    << "D - Delete data \n"
    << "B - Tree Breadth-First Traversal: Print by level \n"
    << "S - Search and display one element using the primary key \n"
    << "L - List data in hash table sequence \n"
    << "K - List data in key sequence (sorted) \n"
    << "I - Print indened Tree \n"
    << "P - Print Hash Table \n"
    << "R - Recursive Depth-First Traversals: preorder, inorder, postorder \n"
    << "W - Write data to a file and commit deletes \n"
    << "T - Hast statistics (info) \n"
    << "Z - Undo Delete \n"
    << "M - showMenu \n\n";
    return;
}