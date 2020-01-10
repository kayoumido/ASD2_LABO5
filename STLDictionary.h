//
// Created by ducky on 10.01.20.
//

#ifndef LABO05_STLDICTIONARY_H
#define LABO05_STLDICTIONARY_H


#include <set>
#include <string>
#include "DictionaryCommon.h"

class STLDictionary : public DictionaryCommon {
private:
    std::set<std::string> data;

public:
    STLDictionary(const std::string &filename) {
        this->readFromFile(filename);
    }

    void insert(std::string value) override {
        this->data.insert(value);
    }

    bool find(const std::string &value) const {
        return this->data.find(value) != this->data.end();
    }
};


#endif //LABO05_STLDICTIONARY_H
