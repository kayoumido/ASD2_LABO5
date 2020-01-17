#include <utility>

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
#include <vector>
#include <iostream>

#include "Dictionary.h"
#include "STLDictionary.h"

template<typename Dict>
class Spellchecker {
    Dict dict;
    std::string toCorrect;
    static const char DELIMITER = ' ';

    std::string cleanPhrase(const std::string &phrase) {
        std::string cleaned;
        cleaned = phrase;

        // replace all the special chars by a space
        // as specified in the given, the `'` is kept
        std::replace_if(cleaned.begin(), cleaned.end(), [](const char &c) {
            return !std::isalpha(c) and c != '\'';
        }, ' '); // for some reason the static const doesn't work \o/

        std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return cleaned;
    }

public:
    Spellchecker(const std::string &toCorrect, const Dict &dict) : toCorrect(toCorrect), dict(dict) {}

    void correct() {
        std::vector<std::string> suggestions;
        std::ifstream inputFile(this->toCorrect);
        std::ofstream outputFile("data/output.txt");

        if (!inputFile.is_open())
            throw std::runtime_error("Unable to open file " + this->toCorrect);

        if(!outputFile)
            throw std::runtime_error("Unable to open the output file");

        // Read file  line by line
        std::string line;
        while (getline(inputFile, line)) {

            // Clean the line
            line = cleanPhrase(line);

            // split the line into words
            std::istringstream iss(line);
            std::string word;
            // keskesafait ? mais des caumentère
            while (std::getline(iss, word, Spellchecker::DELIMITER)) {

                if (word.empty() or (word[0] == '\'' or word[word.length()-1] == '\''))
                    continue;

                if (!dict.find(word)) {
                    outputFile << "*" << word << "\n";
                    suggestions = checkMisspelledWord(word);
                    for(std::string& suggestion : suggestions){
                        outputFile << suggestion << "\n";
                    }
                }
            }
        }
    }

private:
    std::vector<std::string> extraLetter(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "1:";

        // Loop through each letter of the inital word
        for (int i = 0; i < initialWord.size(); ++i) {
            // Generate the word without a letter at current i pos
            std::string cpy = initialWord;
            //cout << cpy.erase(i,1) << endl;
            cpy.erase(i, 1);
            if (dict.find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    std::vector<std::string> forgottenLetter(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "2:";

        // Loop through each letter of the inital word
        for (int i = 0; i <= initialWord.size(); ++i) {
            std::string cpy;
            // Loop through each alphabet letter
            for (char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd) {
                // Generate the word with an extra letter at current i pos
                cpy = initialWord;
                std::string strLetter(1, letterToAdd);
                cpy.insert(i, strLetter);
                if (dict.find(cpy)) {
                    wordsSuggestion.push_back(cpy.insert(0, prefix));
                }
            }

            // Generate the word with an extra quote character at current i pos
            cpy = initialWord;
            cpy.insert(i, "'");
            if (dict.find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    std::vector<std::string> misspelled(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "3:";

        // Loop through each letter of the inital word
        for (int i = 0; i < initialWord.size(); ++i) {
            std::string cpy;
            // Loop throuhgh each alphabet letter
            for (char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd) {
                // Generate the word with an extra letter in place of the current i pos letter
                cpy = initialWord;
                std::string strLetter(1, letterToAdd); // Cast letter in string
                // Erase the letter at the i pos and add the new one
                cpy.erase(i, 1);
                cpy.insert(i, strLetter);

                if (dict.find(cpy)) {
                    wordsSuggestion.push_back(cpy.insert(0, prefix));
                }
            }

            // Generate the word with an extra quote character in place of the current i pos letter
            cpy = initialWord;
            cpy.erase(i, 1);
            cpy.insert(i, "'");
            if (dict.find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    std::vector<std::string> swappedTwoLetters(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "4:";

        // Loop through each letter of the inital word
        for (int i = 0; i < initialWord.size() - 1; ++i) {
            // Generate the word with a swap between letter at pos i and i+1
            std::string cpy = initialWord;
            std::swap(cpy[i], cpy[i + 1]);

            if (dict.find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    std::vector<std::string> checkMisspelledWord(const std::string &word) {
        std::vector<std::string> a = extraLetter(word);
        std::vector<std::string> b = forgottenLetter(word);
        std::vector<std::string> c = misspelled(word);
        std::vector<std::string> d = swappedTwoLetters(word);

        std::vector<std::string> suggestions;
        suggestions.insert(suggestions.begin(), a.begin(), a.end());
        suggestions.insert(suggestions.end(), b.begin(), b.end());
        suggestions.insert(suggestions.end(), c.begin(), c.end());
        suggestions.insert(suggestions.end(), d.begin(), d.end());

        return suggestions;
    }
};


#endif //LABO05_SPELLCHECKER_H
