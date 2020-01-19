/**
 * Authors: Robin Demarta, Loïc Dessaules, Doran Kayoumi
 * File: SpellChecker.h
 * Date: 10.01.2019
 * Class description : Class that will use to correct a specific input file text with a specific Dict. An output file text
 *                     will be generate with all misspelled words and a suggestion list of correct words for it.
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
     * Sanatize a specific phrase passed in parameter
     *
     * @param phrase The phrase to sanatize
     * @return A clean phrase
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
     * Correct the file pass in class constructor with a specific Dict. An output.txt file will be generate with
     * misspelled word and a list of correct word suggestions of it.
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
                // ignore any word that is empty
                if (word.empty())
                    continue;

                // check the word is in the dictionary
                if (!dict->find(word)) {
                    outputFile << "*" << word << "\n";
                    // check for word alternatives
                    suggestions = checkMisspelledWord(word);

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
     * Maybe the user type an extra letter, so we'll generate all word possibilities without 1 letter of the
     * initial word.
     *
     * @param initialWord The inital word to search suggestions for
     * @return A Vector<String> Contains all suggestions
     */
    std::vector<std::string> extraLetter(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "1:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i < initialWord.size(); ++i) {
            // Generate the word without a letter at current i pos
            cpy = initialWord;
            cpy.erase(i, 1);
            if (dict->find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    /**
     * Maybe the user forgot a letter, so we'll generate all word possibilities with one more letter inside
     * the initial word.
     *
     * @param initialWord The initial word to search suggestions for
     * @return A Vector<String> Contains all suggestions
     */
    std::vector<std::string> forgottenLetter(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "2:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i <= initialWord.size(); ++i) {
            // Loop through each alphabet letter
            for (char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd) {
                // Generate the word with an extra letter at current i pos
                cpy = initialWord;
                std::string strLetter(1, letterToAdd);
                cpy.insert(i, strLetter);
                if (dict->find(cpy)) {
                    wordsSuggestion.push_back(cpy.insert(0, prefix));
                }
            }

            // Generate the word with an extra quote character at current i pos
            cpy = initialWord;
            cpy.insert(i, "'");
            if (dict->find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    /**
     * Maybe the user has badly written, so we'll generate all word possibilities by replacing one letter of the
     * initial word.
     *
     * @param initialWord The initial word to search suggestions for
     * @return A Vector<String> Contains all suggestions
     */
    std::vector<std::string> misspelled(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "3:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i < initialWord.size(); ++i) {
            // Loop through each alphabet letter
            for (char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd) {
                // Generate the word with an extra letter in place of the current i pos letter
                cpy = initialWord;
                std::string strLetter(1, letterToAdd); // Cast letter in string
                // Erase the letter at the i pos and add the new one
                cpy.erase(i, 1);
                cpy.insert(i, strLetter);

                if (dict->find(cpy)) {
                    wordsSuggestion.push_back(cpy.insert(0, prefix));
                }
            }

            // Generate the word with an extra quote character in place of the current i pos letter
            cpy = initialWord;
            cpy.erase(i, 1);
            cpy.insert(i, "'");
            if (dict->find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    /**
     * Maybe the user has swapped two letters, so we'll generate all word possibilities by swapping two letters of the
     * initial word.
     *
     * @param initialWord The initial word to search suggestions for
     * @return A Vector<String> Contains all suggestions
     */
    std::vector<std::string> swappedTwoLetters(const std::string &initialWord) {
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "4:";
        std::string cpy;

        // Loop through each letter of the inital word
        for (int i = 0; i < initialWord.size() - 1; ++i) {
            // Generate the word with a swap between letter at pos i and i+1
            cpy = initialWord;
            std::swap(cpy[i], cpy[i + 1]);

            if (dict->find(cpy)) {
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    /**
     * Maybe the incorrect word has been misspelled so we'll generate all simple word suggestion possibilities
     * @param word The word that is maybe misspelled
     * @return A Vector<String> Contains all suggestions
     */
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
