/**
 * Authors: Robin Demarta, Loïc Dessaules, Doran Kayoumi
 * File: Dictionary.h
 * Date: 10.01.2019
 */

#ifndef LABO05_DICTIONARY_H
#define LABO05_DICTIONARY_H

#include <string>
#include <fstream>
#include <algorithm>

class Dictionary {
public:
    virtual void insert(const std::string& value) = 0;
    virtual bool find(const std::string &value) = 0;

protected:

    /**
     * Read the content of a file
     *
     * @param filename to read
     */
    void readFromFile(const std::string& filename) {
        std::ifstream s(filename);

        if (!s.is_open())
            throw std::runtime_error("Unable to open file " + filename);

        this->readFromStream(s);
        s.close();
    }

    /**
     * Read the content of a stream
     * @param s stream to read
     */
    void readFromStream(std::istream& s) {
        std::string value;
        while (getline(s, value)) {
            this->insert(toLower(value));
        }
    }

private:

    /**
     *
     * @param word
     * @return
     */
    std::string toLower(std::string word){
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return word;
    }

};

#endif //LABO05_DICTIONARY_H
