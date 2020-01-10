//
// Created by ducky on 10.01.20.
//

#ifndef LABO05_SPELLCHECKER_H
#define LABO05_SPELLCHECKER_H

#include <string>
#include <utility>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "DictionaryCommon.h"
#include "STLDictionary.h"

class Spellchecker {
private:
    DictionaryCommon *dict;
    std::string toCorrect;

public:
    Spellchecker(std::string toCorrect, bool useSTL) : toCorrect(std::move(toCorrect)) {
        STLDictionary STLDict("data/dictionary");
        dict = &STLDict;
    }

    void correct() {
        std::ifstream s(this->toCorrect);

        if (!s.is_open())
            throw std::runtime_error("Unable to open file");

        std::string line;
        while (getline(s, line)) {
            std::replace_if(line.begin(), line.end(),[] (const char& c) {
                return std::ispunct(c) and c != '\'' ;
            },' ');

            std::istringstream iss(line);
            std::string item;
            while (std::getline(iss, item, ' ')) {
                if (item.empty())
                    continue;
                
            }
        }
    }
};


#endif //LABO05_SPELLCHECKER_H
