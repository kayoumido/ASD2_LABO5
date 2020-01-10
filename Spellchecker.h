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


private:
    std::vector<std::string> wroteAnExtraLetter(const std::string &initialWord, const STLDictionary &dict){
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "1:";

        // Loop through each letter of the inital word
        for(int i = 0; i < initialWord.size(); ++i){
            // Generate the word without a letter at current i pos
            std::string cpy = initialWord;
            //cout << cpy.erase(i,1) << endl;
            cpy.erase(i,1);
            if(dict.find(cpy)){
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }

    std::vector<std::string> forgotALetter(const std::string &initialWord, const STLDictionary &dict){
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "2:";

        // Loop through each letter of the inital word
        for(int i = 0; i <= initialWord.size(); ++i){
            // Loop throuhgh each alphabet letter
            for(char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd){
                // Generate the word with an extra letter at current i pos
                std::string cpy = initialWord;
                std::string strLetter(1, letterToAdd);
                //cout << cpy.insert(i, strLetter) << endl;
                cpy.insert(i, strLetter);
                if(dict.find(cpy)){
                    wordsSuggestion.push_back(cpy.insert(0, prefix));
                }
            }
        }

        return wordsSuggestion;
    }

    std::vector<std::string> misspelledALetter(const std::string& initialWord, const STLDictionary &dict){
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "3:";

        // Loop through each letter of the inital word
        for(int i = 0; i < initialWord.size(); ++i){
            // Loop throuhgh each alphabet letter
            for(char letterToAdd = 'a'; letterToAdd <= 'z'; ++letterToAdd){
                // Generate the word with an extra letter in place of the current i pos letter
                std::string cpy = initialWord;
                std::string strLetter(1, letterToAdd); // Cast letter in string
                // Erase the letter at the i pos and add the new one
                cpy.erase(i,1);
                cpy.insert(i, strLetter);

                if(dict.find(cpy)){
                    wordsSuggestion.push_back(cpy.insert(0, prefix));
                }
            }
        }

        return wordsSuggestion;
    }

    std::vector<std::string> swappedTwoLetters(const std::string& initialWord, const STLDictionary &dict){
        std::vector<std::string> wordsSuggestion;
        std::string prefix = "4:";

        // Loop through each letter of the inital word
        for(int i = 0; i < initialWord.size() - 1; ++i){
            // Generate the word with a swap between letter at pos i and i+1
            std::string cpy = initialWord;
            std::swap(cpy[i], cpy[i+1]);

            if(dict.find(cpy)){
                wordsSuggestion.push_back(cpy.insert(0, prefix));
            }
        }

        return wordsSuggestion;
    }


    std::vector<std::string> checkMisspelledWord(const std::string &word, const STLDictionary &dict) {
        std::vector<std::string> a = wroteAnExtraLetter(word, dict);
        std::vector<std::string> b = forgotALetter(word, dict);
        std::vector<std::string> c = misspelledALetter(word, dict);
        std::vector<std::string> d = swappedTwoLetters(word, dict);

        std::vector<std::string> suggestions;
        suggestions.insert(suggestions.begin(), a.begin(), a.end());
        suggestions.insert(suggestions.end(), b.begin(), b.end());
        suggestions.insert(suggestions.end(), c.begin(), c.end());
        suggestions.insert(suggestions.end(), d.begin(), d.end());

        return suggestions;
    }
};


#endif //LABO05_SPELLCHECKER_H
