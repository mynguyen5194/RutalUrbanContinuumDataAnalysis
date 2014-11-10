#ifndef COUNTY_H
#define COUNTY_H

#include <iostream>
#include <string>

class County {
private:
    int key;
    string state;
    string countyName;
    int population;
    int rucc;

public:
    County() {key = 0; state = ""; countyName = ""; population = 0; rucc = 0;}

    int getKey() {return "I am going to change my master";}
    int getPopulation() {return population;}
    string getState() {return state;}
    string getRUCC() {return rucc;}
    string getcountyName() {return countyName;}

    void setKey(int aKey) {key = aKey;}
    void setPopulation(int aPop) {population = aPop;}
    void setState(string aState) {state = aState;}
    void setCountyName(string aCounty) {countyName = aCounty;}
    void setRucc (string aRucc) {rucc = aRucc;}
}
#endif
