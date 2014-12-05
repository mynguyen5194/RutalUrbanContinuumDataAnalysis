#ifndef COUNTY_H
#define COUNTY_H

#include <iostream>
#include <string>

using namespace std;

class County {
private:
    int key;
    int population;
    int rucc;
    string state;
    string county;
    County * next;
    
public:
    County() {key = 0; state = ""; county = ""; population = 0; rucc = 0; next = NULL;}

    int getKey() const {return key;}
    int getPopulation() const {return population;}
    int getRucc() const {return rucc;}
    string getState() const {return state;}
    string getCounty() const {return county;}
    County * getNext() {return next;}

    void setNext(County * Next) {next = Next;}
    void setInfo(int Key, string State, string County, int Population, int Rucc)
    {key = Key; state = State; county = County; population = Population; rucc = Rucc;}
};
#endif
