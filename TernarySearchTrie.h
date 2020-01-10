//
// Created by rdema on 10.01.2020.
//

#ifndef LABO05_TERNARYSEARCHTRIE_H
#define LABO05_TERNARYSEARCHTRIE_H

#include <string>
#include "DictionaryCommon.h"

class TernarySearchTrie : public DictionaryCommon {

    class Node {
    public:
        Node* left;
        Node* middle;
        Node*  right;
        char value;
        bool endOfWorld; // Indicates that this node (char) is the end of a word

        Node(Node* left, Node* middle, Node* right, char value, bool endOfWorld) : left(left), middle(middle), right(right), value(value), endOfWorld(endOfWorld) {}
        ~Node() {
            delete left;
            delete middle;
            delete right;
        }
    };

    Node* root;

public:

    TernarySearchTrie() {
        root = nullptr;
    }

    ~TernarySearchTrie() {
        delete root;
    }

    /**
     * Public function to insert a new word in the TST
     * @param word
     */
    void insert(std::string word) override {
        insertInTrie(root, word);
    }

    /**
     * Public function to check if a word exists in the TST
     * @param word
     * @return
     */
    bool find(std::string word) {
        findInTrie(root, word);
    }

private:

    /**
     * Recursive function to find an existing word
     * @return
     */
    bool findInTrie(Node* node, std::string word) {
        if(node == nullptr)
            return false;

        std::string newWord = word.substr(1, word.length() - 1); // Remove first char

        if(word[0] < node->value)
            // Left
            return findInTrie(node->left, word);

        if(word[0] > node->value)
            // Right
            return findInTrie(node->right, word);

        if(word.length() == 1)
            // Last letter: must be end of the word
            return node->endOfWorld;

        // Middle
        return findInTrie(node->middle, newWord);
    }

    /**
     * Recursive function to insert a new word
     * @param node
     * @param word
     */
    void insertInTrie(Node* node, std::string word) {

        if(node == nullptr)
            // Empty: insert node
            node = new Node(nullptr, nullptr, nullptr, word[0], false);
        else if(word[0] < node->value)
            // Left
            insertInTrie(node->left, word);
        else if(word[0] > node->value)
            // Right
            insertInTrie(node->right, word);
        else
            // Middle
            if(word.length() == 1)
                // No more letters
                node->endOfWorld = true;
            else
                // Same letter: ignore it and continue
                insertInTrie(node->middle, word.substr(1, word.length() - 1));
    }
};

#endif //LABO05_TERNARYSEARCHTRIE_H