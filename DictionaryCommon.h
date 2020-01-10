//
// Created by ducky on 10.01.20.
//

#ifndef LABO05_DICTIONARYCOMMON_H
#define LABO05_DICTIONARYCOMMON_H

#include <string>
#include <fstream>

class DictionaryCommon {

public:
    virtual void insert(std::string value) = 0;

protected:
    // lecture depuis un fichier.
    void readFromFile(const std::string& filename) {
        std::ifstream s(filename);
        this->readFromStream(s);
        s.close();
    }

    // lecture depuis un stream. Utilise addEdge(...)
    // a definir par les classe fille
    void readFromStream(std::istream& s) {
        std::string value;
        while (getline(s, value)) {
            this->insert(value);
        }
    }
};

#endif //LABO05_DICTIONARYCOMMON_H
