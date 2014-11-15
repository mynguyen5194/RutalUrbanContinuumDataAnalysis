//
//  main.cpp
//  TeamProject
//
//  Created by My Nguyen on 11/10/14.
//  Copyright (c) 2014 MyNguyen. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include "County.h"

#define KEY_LENGTH 5
#define STATE_LENGTH 2
#define LIST_LENGTH 22

using namespace std;

// Functions prototypes
bool readFile(County* counties);
bool isNumber(char character);
void menu(County * counties);
void help();

int main() {
    County* counties = new County[LIST_LENGTH];
    
    readFile(counties);
    
    for (int i = 0; i < LIST_LENGTH; i++) {
        cout << "county name: " << (counties + i)->getCounty() << endl;
    }
    
//    menu(counties);
    
    return 0;
}

// ********************************************
//  readFile function
// ********************************************
bool readFile(County* counties) {
    ifstream inFile;
    string line;
    string key_s = "";
    string rucc_s = "";
    string state = "";
    string county = "";
    string pop_s = "";
    int key, rucc, pop;
    int index = 0;
    int start_pop; //where population starts, depends on county, initialized later
    
    inFile.open("RU_database.txt");
    if (inFile.is_open()) {
        while (inFile) {
            getline(inFile, line);
        
            //read keys
            for(int i = 0; i < KEY_LENGTH; i++)
                key_s += line[i];
        
            //change to int
            key = stoi(key_s);				// ---some of the keys that begin with zeros are different, but                             it doesnt break the program -- //
        
        
            //read states
            for (int i = 5; i <= 5 + STATE_LENGTH; i++)
                state += line[i];
        
            int start_county = 9; //where county names start, always 9
        
            //read in county until hit numbers
            while(!isNumber(line[start_county])) {
                county += line[start_county];
                start_county++;
            }
        
            //where pop starts
            start_pop = start_county;
        
            //read in population
            while(isNumber(line[start_pop]) || line[start_pop] == ',') {
                if(line[start_pop] != ',')
                    pop_s += line[start_pop];
        
                start_pop++;
            }
        
            //change pop into int
            pop = stoi(pop_s);
        
            //read in rucc
            rucc_s = line[start_pop + 1];
        
            //change rucc_s into int
            rucc = stoi(rucc_s);
        
            //input fields into county
            (counties + index)->setCounty(county);
            (counties + index)->setKey(key);
            (counties + index)->setPopulation(pop);
            (counties + index)->setRUCC(rucc);
            (counties + index)->setState(state);
        
            //reset strings
            key_s = "";
            state = "";
            county = "";
            pop_s = "";
            rucc_s = "";
            index++;
        
        }
    }
    
    else {
        cout << "Error opening file 'RU_database.txt'" << endl;
        return false;
    }
    
    inFile.close();
    
    return true;
}

// ********************************************
//  isNumber function
// ********************************************
bool isNumber(char character) {
    return int(character) >= 48 && int(character) <= 57; //48 and 57 are range of ASCII code for integers
}

// ********************************************
//  menu function
// ********************************************
void menu(County * counties) {
    string option;
    
    do {
        cout << "Please enter your option: ";
        getline(cin, option);
        
        switch (toupper(option[0])) {
            case 'A':
        // Add new data
                break;
            
            case 'D':
        // Delete data
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
                
            case 'P':
        // Print indented tree
                break;
                
            case 'W':
        // Write data to a file
                break;
                
            case 'I':
        // Hash statistics (info)
                break;
                
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
        << "I - Hast statistics (info) \n"
        << "H - Help \n";
    return;
}
