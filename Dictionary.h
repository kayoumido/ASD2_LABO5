//
// Created by ducky on 10.01.20.
//

#ifndef LABO05_DICTIONARY_H
#define LABO05_DICTIONARY_H

#include <string>
#include <fstream>

class Dictionary {

public:
    virtual void insert(std::string value) = 0;
    virtual bool find(const std::string &value) = 0;
protected:
    void readFromFile(const std::string& filename) {
        std::ifstream s(filename);

        if (!s.is_open())
            throw std::runtime_error("Unable to open file " + filename);

        this->readFromStream(s);
        s.close();
    }

    void readFromStream(std::istream& s) {
        std::string value;
        while (getline(s, value)) {
            this->insert(value);
        }
    }
};

#endif //LABO05_DICTIONARY_H
