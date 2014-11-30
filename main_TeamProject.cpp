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
#include <iomanip>
#include "County.h"
#include "HashTable.h"
#include "BinarySearchTree.h"
#include "Stack.h"

#define LIST_SIZE 40
using namespace std;

// Functions prototypes
void display(County & county);
bool readFile (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree);
void outFileDisplay(County & county, ofstream & outFile);
void menu (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
void insert (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree);
bool searchCounty (HashTable<County> * countyHash);
bool deleteCounty(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
void undoDelete(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
void Depth_First_Traversals(BinarySearchTree<County>* tree);
void writeToFile(BinarySearchTree<County> * countyTree, Stack<County*> & Stack);
void eliminateComma(string & line);
int determineHashSize(int inFileCount);
bool isPrime(int checkPrime);
bool isNumeric(string numStr);
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
                    // Add new data
                    insert(countyHash, countyTree);
                    break;
                    
                case 'S':
                    searchCounty(countyHash);
                    break;
                    
                case 'D':
                    // Delete data (both BST & hash table)
                    deleteCounty(countyHash, countyTree, Stack);
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
                    countyTree->Print_Indented_Tree(display);
                    break;
                    
                case 'O':
                    // In Order
                    countyTree->inOrder(display);
                    break;

                case 'Z':
                    // Undo delete
                    undoDelete(countyHash, countyTree, Stack);
                    break;
                    
                case 'W':
                    // Write data to a file
                    cout << "saving data to 'RU_updated.txt'\n";
                    writeToFile(countyTree, Stack);
                    break;
        
                case 'M':
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
void insert (HashTable<County> * countyHash, BinarySearchTree<County> * countyTree) {
    string key, state, county, population, rucc;
    County countyInfo;
    
    cout << " ~*~ Insert New Data ~*~\n";
    cout << "Please enter the key (FIPS): ";
    getline(cin, key);
    
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), "", "", 0, 0); //, state, county, atoi(population.c_str()), atoi(rucc.c_str()));
        if (countyHash->search(countyInfo, countyInfo) == false) {
            cout << "Please enter the state abbreviation: ";
            getline(cin, state);
            
            cout << "Please enter the county name: ";
            getline(cin, county);
            
            cout << "Please enter the county's population: ";
            getline(cin, population);
            eliminateComma(population);
            
            cout << "Please enter the county's RUCC (from 1 to 9): ";
            getline(cin, rucc);
            
            if (state.length() == 2 && isNumeric(population) &&
                rucc.length() == 1 && atoi(rucc.c_str()) > 0 && atoi(rucc.c_str()) < 10) {
                countyInfo.setInfo(atoi(key.c_str()), state, county, atoi(population.c_str()), atoi(rucc.c_str()));
                countyHash->insert(countyInfo);
                countyTree->insert(countyInfo);
            }
        }
        else
            cout << "The key is already exist!\n";
    }
    else
        cout << "Invalid input!\n";
    
    return;
}

// ********************************************
//  Search_county function
// ********************************************
bool searchCounty (HashTable<County> * countyHash) {
    bool found = false;
    string key;
    County countyInfo;
    
    cout << " ~*~ Search County Information ~*~\n";
    cout << "Please enter the county's key (FIPS): ";

    getline(cin, key);
    if (isNumeric(key)) {
        countyInfo.setInfo(atoi(key.c_str()), "", "", 0, 0);
        if (countyHash->search(countyInfo, countyInfo)) {
            display(countyInfo);
            found = true;
        }
        else {
            cout << "The county is not in the list!\n";
            found = false;
        }
    }
    
    else
        cout << "Invalid input!\n";
    return found;
}

// ********************************************
//  deleteCounty function
// ********************************************
bool deleteCounty(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack) {
    string key;
    bool deleted = false;
    
    County countyInfo;
    cout << " ~*~ Delete County Information ~*~\n";
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
    
    else
        cout << "Invalid input!\n";
    
    return deleted;
}

// ********************************************
//  undoDelete function
// ********************************************
void undoDelete(HashTable<County> * countyHash, BinarySearchTree<County> * countyTree, Stack<County*> & Stack) {
    County * county;
    
    cout << " ~*~ Undo Delete County Information ~*~\n";
    if (!Stack.isEmpty()) {
        if (Stack.getTop(county)) {
            cout << "Reinsert ";
            display(*county);
        }
        
        if (Stack.pop(county)) {
            countyHash->insert(*county);
            countyTree->insert(*county);
        }
        
    }
    else
        cout << "Stack is empty!\n";
    
    return;
}

// ********************************************
//  Depth_First_Traversals function
// ********************************************
void Depth_First_Traversals(BinarySearchTree<County>* countyTree)
{
    if(!countyTree->getKey_root()) {
        cout << "There is no data.\n";
        return;
    }
    cout<<"\nIn order: \n";
    countyTree->inOrder(display);
    cout<<"\nPost order: \n";
    countyTree->postOrder(display);
    cout <<"\nPre order: \n";
    countyTree->preOrder(display);
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
//  display function
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
        cout << county.getKey() << "\t" << county.getState() << "\t" << county.getCounty()
        << "\t" << county.getPopulation() << "\t" << county.getRucc()
        << endl << "\t" << arrayDescriptions[county.getRucc() - 1] << endl << endl;
    return;
}

// ********************************************
//  outFileDisplay function
// ********************************************
void outFileDisplay(County & county, ofstream & outFile) {
    
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
        outFile << county.getKey() << "\t" << county.getState() << "\t" << county.getCounty()
        << "\t" << county.getPopulation() << "\t" << county.getRucc()
        << endl << "\t" << arrayDescriptions[county.getRucc() - 1] << endl << endl;
    return;
}

// ********************************************
//  menu function
// ********************************************
void showMenu() {
    cout << "\n ~*~ Menu ~*~ \n"
        << "A - Add new data \n"
        << "S - Search and display one element using the primary key \n"
        << "D - Delete data \n"
        << "L - List data in hash table sequence \n"
        << "P - Print indented hash table \n"
        << "T - Hast statistics (info) \n"
        << "B - Print BST by level \n"
        << "R - Recursive depth-first traversal \n"
        << "I - Print indented tree \n"
        << "O - List data in key sequence (sorted) \n"
        << "Z - Undo delete \n"
        << "W - Write data to a file \n"
        << "M - showMenu \n";
    return;
}