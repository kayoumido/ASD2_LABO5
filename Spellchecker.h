//
// Created by ducky on 10.01.20.
//

#ifndef LABO05_SPELLCHECKER_H
#define LABO05_SPELLCHECKER_H

#include <string>
#include <utility>

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
};


#endif //LABO05_SPELLCHECKER_H
