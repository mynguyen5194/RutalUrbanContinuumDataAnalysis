#ifndef COUNTY_H
#define COUNTY_H

#include <iostream>
#include <string>

//push for meeeee

using namespace std;

class County {
private:
    int key;
    int population;
    int rucc;
    string state;
    string county;
    
public:
    County() {key = 0; state = ""; county = ""; population = 0; rucc = 0;}

    int getKey() {return key;}
    int getPopulation() {return population;}
    int getRUCC() {return rucc;}
    string getState() {return state;}
    string getCounty() {return county;}

    void setKey(int aKey) {key = aKey;}
    void setPopulation(int aPop) {population = aPop;}
    void setRUCC (int aRucc) {rucc = aRucc;}
    void setState(string aState) {state = aState;}
    void setCounty(string aCounty) {county = aCounty;}
};
#endif
