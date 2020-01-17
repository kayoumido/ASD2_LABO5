//
// Created by rdema on 10.01.2020.
//

#ifndef LABO05_TERNARYSEARCHTRIE_H
#define LABO05_TERNARYSEARCHTRIE_H

#include <string>
#include "Dictionary.h"

class TernarySearchTrie : public Dictionary {

    struct Node {
    public:
        Node* left;
        Node* middle;
        Node* right;
        char value;
        bool endOfWorld; // Indicates that this node (char) is the end of a word

        Node(Node* left, Node* middle, Node* right, char value, bool endOfWorld) : left(left), middle(middle), right(right),
                                                                                   value(value), endOfWorld(endOfWorld) {}
    };

    Node* root;

public:

    TernarySearchTrie(const std::string& filename) {
        root = nullptr;
        this->readFromFile(filename);
    }

    ~TernarySearchTrie() {
        deleteSubTree(root);
    }

    void deleteSubTree(Node* x) {
        if(x == nullptr) return;
        deleteSubTree(x->right);
        deleteSubTree(x->left);
        deleteSubTree(x->middle);
        delete x;
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
    bool find(const std::string& word) override {
        return findInTrie(root, word);
    }

private:

    /**
     * Recursive function to find an existing word
     * @return
     */
    bool findInTrie(Node* node, std::string word) {
        if(node == nullptr)
            return false;

        if(word[0] < node->value)
            // Left
            return findInTrie(node->left, word);

        if(word[0] > node->value)
            // Right
            return findInTrie(node->right, word);

        if(word.length() == 1)
            // Last letter: must be end of the word
            return node->endOfWorld;

        // Middle (remove first letter and continue)
        return findInTrie(node->middle, word.substr(1, word.length() - 1));
    }

    /**
     * Recursive function to insert a new word
     * @param node
     * @param word
     */
    void insertInTrie(Node*& node, std::string word) {

        if(node == nullptr)
            // Empty: insert node
            node = new Node(nullptr, nullptr, nullptr, word[0], false);
        if(word[0] < node->value)
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