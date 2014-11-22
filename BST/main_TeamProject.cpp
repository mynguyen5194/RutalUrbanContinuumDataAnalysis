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
// Functions Prototypes
void display(County & county);
using namespace std;

// Functions prototypes
bool readFile(HashTable<County>* counties, BinarySearchTree<County>* Tree);
void menu(County * counties, BinarySearchTree<County> *Tree);
void help();
void Depth_First_Traversals(BinarySearchTree<County>* tree);
void Depth_First_Traversals_Iterative(BinarySearchTree<County>*tree);
void Search_county(BinarySearchTree<County> *tree);

int main() {
    HashTable<County> * counties = new HashTable<County>();
	BinarySearchTree<County> *Tree = new BinarySearchTree<County>();
    readFile(counties, Tree);
    cout << "Binary Search Tree:\n\n";

	cout << "\n\nHash Table:\n\n";
    counties->displayList(display);
    cout << endl;
    counties->printHashTable(display);
    
    // menu(counties);
    system("pause");
    return 0;
}

// ********************************************
//  display function
// ********************************************
void display(County & county) {
    if (county.getKey() != 0 && county.getPopulation() != 0)
        cout << county.getKey() << " " << county.getState() << " " << county.getCounty() << " "
            << county.getPopulation() << " " << county.getRucc() << endl;
    return;
}
// ********************************************
//  readFile function
// ********************************************
bool readFile(HashTable<County> * counties, BinarySearchTree<County>* Tree) {
    ifstream inFile;
    int key, population, rucc;
    string line, state, county, pop;
    string delimiter = "\t";
    bool empty = true;
    size_t endPos;
    County countyInfo;
    
    inFile.open("RU_database.txt");
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
        for (size_t i = 0; i < pop.length(); i++) {
            if (pop[i] == ',') {
                pop.erase(i, 1);   // Erase comma if any
            }
        }
        population = atoi(pop.c_str());
        line.erase(0, endPos+1);
        
        rucc = atoi(line.substr(0, 1).c_str());
        
        countyInfo.setInfo(key, state, county, population, rucc);
        counties->insert(countyInfo);
		Tree->insert(countyInfo);
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
void menu(County * counties, BinarySearchTree<County> *Tree) {
    string option;
    
    do {
        cout << "Please enter your option: ";
        getline(cin, option);
        
        switch (toupper(option[0])) {
            case 'A':
        // Add new data
                break;
           
			case 'B':
		//Tree Breadth-First Traversal: Print by level
				if(!Tree->Breadth_First_Traversal())
			 cout << "There is no data.\n";
				break;
            case 'D':
        // Delete data
                break;
			
			case 'F':
		// Tree - Search by a unique key
                Search_county(Tree);
				break;

            case 'S':
        // Search
                break;
                
            case 'L':
        // List data in hash table sequence
                break;
                
            case 'K':
        // List data in key sequence (sorted)
                break;
                
			case 'G':
		//Print Greater than the key
				int key; cin >> key;
				Tree->getGreater(key, display);
				break;

			case 'I':
		//Iterative Depth-First Traversals:inorder, preorder, postorder
				Depth_First_Traversals_Iterative(Tree);
				break;
            case 'P':
        // Print indented tree
				Tree->Print_Indented_Tree();
                break;
                
            case 'W':
        // Write data to a file
                break;
                
            case 'T':
        // Hash statistics (info)
                break;
             
			case 'R':
		// Recursive Depth-First Traversals:inorder, preorder, postorder
				Depth_First_Traversals(Tree);
				break;

			case 'X':
		//Delete a node in a tree
				int remove_key;
				cin >> remove_key;
				Tree->remove(remove_key);
            case 'H':
                help();
                break;
                
            default:
                break;
        }
    } while (toupper(option[0]) != 'Q' && option.length() != 1);
    
    return;
}

// ********************************************
//  menu function
// ********************************************
void help() {
    cout << "\n ~*~ HELP ~*~ \n"
        << "A - Add new data \n"
        << "D - Delete data \n"
        << "S - Search and display one element using the primary key \n"
        << "L - List data in hash table sequence \n"
        << "K - List data in key sequence (sorted) \n"
        << "P - Print indented tree \n"
        << "W - Write data to a file \n"
        << "T - Hast statistics (info) \n"
        << "H - Help \n";
	cout << "X - Delete a node in a tree.\n";
	cout << " F - Search by a unique key.\n";
	cout << " R - Recursive Depth-First Traversals:inorder, preorder, postorder.\n";
	cout << " I - Iterative Depth-First Traversals:inorder, preorder, postorder.\n";
	cout << " B - Tree Breadth-First Traversal: Print by level.\n";
	cout << " G - Print the items with a key greater than a given target key.\n";
    return;
}

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

void Depth_First_Traversals_Iterative(BinarySearchTree<County>*tree)
{
	if(!tree->getKey_root())
	{
		cout << "There is no data.\n";
		return;
	}
	cout << "\nPre order: \n";
	tree->PreOrder_Interative(display);
	cout << "\nIn order: \n";
	tree->InOrder_Interative(display);
}

void Search_county(BinarySearchTree<County> *tree)
{
	int year;
	cout << "\nEnter the year of a movie you want to search: ";
	cin >> year;
	County acounty;
	tree->getEntry(year, acounty);
	if (!acounty.getKey())	
	{
		cout << "The year of movie you enter is not in the list.\n";
		return;
	}
		 display(acounty);
}
