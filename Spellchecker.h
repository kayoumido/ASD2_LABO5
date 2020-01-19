/**
 * Authors: Robin Demarta, Loïc Dessaules, Doran Kayoumi
 * File: SpellChecker.h
 * Date: 10.01.2019
 * Description : Parses a given text file for any words by looking for each word in a given Dictionary (Dict).
 * 			If a mispelled word is found (i.e. it isn't in the dictionary) a list of suggestions is generated
 */

#include <utility>

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
    Dict* dict;
    std::string toCorrect;
    static const char DELIMITER = ' ';

    /**
     * Sanatize a given phrase
     * i.e. replaces any non alphabetic characters by a space
     * `'` are kept if they are surrounded by valid characters
     *  e.g.:   it's --> valid
     *          'A'  --> non valid
     *
     * @param phrase to sanatize
     * @return cleaned phrase
     */
    std::string sanitize(const std::string &phrase) {
        std::string cleaned = phrase;

        for (int i = 0; i < cleaned.length(); ++i) {
            char c = cleaned[i];

            // check if the current char is valid.
            // i.e. it's an alpha or a `'`
            if (!std::isalpha(c) and c != '\'') {
                cleaned.replace(i, 1, " ");
                continue;
            }

            // if the current char is a `'`, check that the surrounding chars are valid ones
            if (c == '\'' and (!isalpha(cleaned[i-1]) or !isalpha(cleaned[i+1])))
                cleaned.replace(i, 1, " ");
        }

        // to lower the cleaned phrase
        std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        return cleaned;
    }

public:
    Spellchecker(const std::string &toCorrect, Dict* dict) : toCorrect(toCorrect), dict(dict) {}

    /**
     * Parses the input file to find any typos
     * An `output.txt` file is generated containing all the typos w/ a list of suggestions
     */
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
            line = sanitize(line);

            // loop through all the words of a line
            std::istringstream iss(line);
            std::string word;
            while (std::getline(iss, word, Spellchecker::DELIMITER)) {
                // ignore any empty words
                if (word.empty())
                    continue;

                // check that the word is in the dictionary
                if (!dict->find(word))  {
                    outputFile << "*" << word << "\n";
                    // check for word alternatives
                    suggestions = generateSuggestions(word);

                    // save suggestions in an output file
                    for(std::string& suggestion : suggestions){
                        outputFile << suggestion << "\n";
                    }
                }
            }
        }
    }

private:

    /**
     * Generate suggestions by remove one character at a time.
     *
     * @param word containing a typo
     * @return list (vector) of suggestions
     */
    std::vector<std::string> extraLetter(const std::string &word) {
        std::vector<std::string> suggestions;
        std::string prefix = "1:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i < word.size(); ++i) {
            // Generate the word without a letter at current i pos
            cpy = word;
            cpy.erase(i, 1);
            if (dict->find(cpy)) {
                suggestions.push_back(cpy.insert(0, prefix));
            }
        }

        return suggestions;
    }

    /**
     * Generate suggestions by adding one character at a time.
     *
     * @param word containing a typo
     * @return list (vector) of suggestions
     */
    std::vector<std::string> forgottenLetter(const std::string &word) {
        std::vector<std::string> suggestions;
        std::string prefix = "2:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i <= word.size(); ++i) {
            // Loop through each alphabet letter
            for (char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd) {
                // Generate the word with an extra letter at current i pos
                cpy = word;
                std::string strLetter(1, letterToAdd);
                cpy.insert(i, strLetter);
                if (dict->find(cpy)) {
                    suggestions.push_back(cpy.insert(0, prefix));
                }
            }

            // Generate the word with an extra quote character at current i pos
            cpy = word;
            cpy.insert(i, "'");
            if (dict->find(cpy)) {
                suggestions.push_back(cpy.insert(0, prefix));
            }
        }

        return suggestions;
    }

    /**
     * Generate suggestions by replacing one character at a time.
     *
     * @param word containing a typo
     * @return list (vector) of suggestions
     */
    std::vector<std::string> misspelled(const std::string &word) {
        std::vector<std::string> suggestions;
        std::string prefix = "3:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i < word.size(); ++i) {
            // Loop through each alphabet letter
            for (char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd) {
                // Generate the word with an extra letter in place of the current i pos letter
                cpy = word;
                std::string strLetter(1, letterToAdd); // Cast letter in string
                // Erase the letter at the i pos and add the new one
                cpy.erase(i, 1);
                cpy.insert(i, strLetter);

                if (dict->find(cpy)) {
                    suggestions.push_back(cpy.insert(0, prefix));
                }
            }

            // Generate the word with an extra quote character in place of the current i pos letter
            cpy = word;
            cpy.erase(i, 1);
            cpy.insert(i, "'");
            if (dict->find(cpy)) {
                suggestions.push_back(cpy.insert(0, prefix));
            }
        }

        return suggestions;
    }

    /**
     * Generate suggestions by swapping two adjacent letters
     *
     * @param word containing a typo
     * @return list (vector) of suggestions
     */
    std::vector<std::string> swappedTwoLetters(const std::string &word) {
        std::vector<std::string> suggestions;
        std::string prefix = "4:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i < word.size() - 1; ++i) {
            // Generate the word with a swap between letter at pos i and i+1
            cpy = word;
            std::swap(cpy[i], cpy[i + 1]);

            if (dict->find(cpy)) {
                suggestions.push_back(cpy.insert(0, prefix));
            }
        }

        return suggestions;
    }

    /**
     * Generate suggestions for a word containing a typo
     * e.g. :   Containing an extra letter
     *          Missing a letter
     *          Misspelled (wrong letter was used)
     *          Two adjacent letters were swaped
     *
     * @param word containing a typo
     * @return list (vector) of suggestions
     */
    std::vector<std::string> generateSuggestions(const std::string &word) {
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
