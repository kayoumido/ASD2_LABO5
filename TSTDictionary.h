/**
 * Authors: Robin Demarta, Loïc Dessaules, Doran Kayoumi
 * File: TSTDictionary.h
 * Date: 10.01.2019
 */

#ifndef LABO05_TSTDICTIONARY_H
#define LABO05_TSTDICTIONARY_H

#include <string>
#include "Dictionary.h"
#include "TernarySearchTrie.h"

class TSTDictionary : public Dictionary {
private:
    TernarySearchTrie data;

public:
    TSTDictionary(const std::string &filename) {
        this->readFromFile(filename);
    }

    void insert(const std::string& value) override {
        this->data.insert(value);
    }

    bool find(const std::string &value) override {
        return this->data.find(value);
    }
};

#endif //LABO05_TSTDICTIONARY_H
