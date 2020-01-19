/**
 * Authors: Robin Demarta, Loïc Dessaules, Doran Kayoumi
 * File: main.cpp
 * Date: 10.01.2019
 */

#include <iostream>
#include <string>
#include "STLDictionary.h"
#include "TSTDictionary.h"
#include "Spellchecker.h"
#include <chrono>

// 0 to use the Ternary Search Trie
// 1 to use the STL container
#define USE_STL 0

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
    TSTDictionary tst("data/" + dict);
    dictLoadTime = chrono::high_resolution_clock::now() - start;
    
    Spellchecker<TSTDictionary> sc("data/" + fileToCorrect, &tst);
#endif

    start = chrono::high_resolution_clock::now();
    sc.correct();
    correctionTime = chrono::high_resolution_clock::now() - start;

    cout << "Correcting : " << fileToCorrect << endl;
    cout << "Dictionary load time : " << chrono::duration_cast<chrono::milliseconds>(dictLoadTime).count() << "ms" << endl;
    cout << "Correction time : " << chrono::duration_cast<chrono::milliseconds>(correctionTime).count() << "ms" << endl;

    /*
     * RESULTS
     * STLContainer
     * ===================================
     * Correcting : input_lates.txt
     * Dictionary load time : 673ms
     * Correction time : 369μs
     *
     * Correcting : input_simple.txt
     * Dictionary load time : 747ms
     * Correction time : 104μs
     *
     * Correcting : input_wikipedia.txt
     * Dictionary load time : 760ms
     * Correction time : 18ms
     *
     * Correcting : input_sh.txt
     * Dictionary load time : 699ms
     * Correction time : 94ms
     *
     *
     * TernarySearchTrie:
     * ===================================
     * Correcting : input_lates.txt
     * Dictionary load time : 1481ms
     * Correction time : 391μs
     *
     * Correcting : input_simple.txt
     * Dictionary load time : 1353ms
     * Correction time : 122μs
     *
     * Correcting : input_wikipedia.txt
     * Dictionary load time : 1452ms
     * Correction time : 29ms
     *
     * Correcting : input_sh.txt
     * Dictionary load time : 1451ms
     * Correction time : 162ms
     *
     *
     * Comments :
     * Looking at the results, we can clearly see that the Ternary Search Trie takes a lot longer to load the
     * dictionary. But the correction times are very similar.
     *
     * We've pre generated the suggestions for all the input files. They can be found in the `output` folder
     */

    return 0;
}
