/**
 * Authors: Robin Demarta, Loïc Dessaules, Doran Kayoumi
 * File: STLDictionary.h
 * Date: 10.01.2019
 */

#ifndef LABO05_STLDICTIONARY_H
#define LABO05_STLDICTIONARY_H


#include <unordered_set>
#include <string>
#include "Dictionary.h"

class STLDictionary : public Dictionary {
private:
    // We've decided to use an unordered_set to contain the dictionary
    // because it has a search and insert complexity of 0(1)
    std::unordered_set<std::string> data;

public:
    STLDictionary(const std::string &filename) {
        this->readFromFile(filename);
    }

    void insert(const std::string& value) override {
        this->data.insert(value);
    }

    bool find(const std::string &value) override {
        return this->data.find(value) != this->data.end();
    }
};


#endif //LABO05_STLDICTIONARY_H
