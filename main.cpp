/**
 * Authors: Robin Demarta, Loïc Dessaules, Doran Kayoumi
 * File: main.cpp
 * Date: 10.01.2019
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "STLDictionary.h"
#include "TernarySearchTrie.h"
#include "Spellchecker.h"
#include <chrono>

#define USE_STL 1 // If 1, TernarySearchTrie will be used

using namespace std;

int main() {
    chrono::high_resolution_clock::time_point start;
    chrono::high_resolution_clock::duration dictLoadTime;
    chrono::high_resolution_clock::duration correctionTime;

    string fileToCorrect = "input_sh.txt";
    string dict = "dictionary.txt";

#if USE_STL
    start = chrono::high_resolution_clock::now();
    STLDictionary stldict("data/" + dict);
    dictLoadTime = chrono::high_resolution_clock::now() - start;

    Spellchecker<STLDictionary> sc("data/" + fileToCorrect, &stldict);
#else
    start = chrono::high_resolution_clock::now();
    TernarySearchTrie tst("data/" + dict);
    dictLoadTime = chrono::high_resolution_clock::now() - start;
    
    Spellchecker<TernarySearchTrie> sc("data/" + fileToCorrect, &tst);
#endif

    start = chrono::high_resolution_clock::now();
    sc.correct();
    correctionTime = chrono::high_resolution_clock::now() - start;

    cout << "Correcting : " << fileToCorrect << endl;
    cout << "Dictionary load time : " << chrono::duration_cast<chrono::milliseconds>(dictLoadTime).count() << "ms" << endl;
    cout << "Correction time : " << chrono::duration_cast<chrono::milliseconds>(correctionTime).count() << "ms" << endl;

    return 0;
}
