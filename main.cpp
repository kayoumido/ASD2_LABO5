#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "STLDictionary.h"
#include "TernarySearchTrie.h"
#include "Spellchecker.h"

#define USE_STL 1

using namespace std;

int main() {

    string inputFile = "data/input_lates.txt";


    #if USE_STL
        STLDictionary stldict("data/dictionary.txt");
        Spellchecker<STLDictionary> sc("data/input_lates.txt", stldict);
    #else
        TernarySearchTrie tst("data/dictionary.txt");
        Spellchecker<TernarySearchTrie> sc("data/input_lates.txt", tst);
    #endif

    sc.correct();

    return 0;
}
