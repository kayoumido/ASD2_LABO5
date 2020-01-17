//
// Created by rdema on 10.01.2020.
//

#ifndef LABO05_TERNARYSEARCHTRIE_H
#define LABO05_TERNARYSEARCHTRIE_H

#include <string>
#include "Dictionary.h"
#include "AVLTree.h"

class TernarySearchTrie : public Dictionary {

    struct Node {
    public:
        Node* left;
        Node* middle;
        Node* right;
        char value;
        bool endOfWorld; // Indicates that this node (char) is the end of a word
        int nodeHeight;
        int nodeSize;

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
        root = insertInTrie(root, word);
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
    Node* insertInTrie(Node* node, std::string word) {

        if(node == nullptr)
            // Empty: insert node
            node = new Node(nullptr, nullptr, nullptr, word[0], false);
        if(word[0] < node->value)
            // Left
            node->left = insertInTrie(node->left, word);
        else if(word[0] > node->value)
            // Right
            node->right = insertInTrie(node->right, word);
        else
            // Middle
        if(word.length() == 1)
            // No more letters
            node->endOfWorld = true;
        else
            // Same letter: ignore it and continue
            node->middle =  insertInTrie(node->middle, word.substr(1, word.length() - 1));

        //updateNodeSize(node);
        //return restoreBalance(node);
        return node;
    }

    int height(Node* x) {
        if ( x == nullptr )
            return -1;
        return x->nodeHeight;
    }

    int balance(Node* x) {
        if(x==nullptr) return 0;
        return height(x->left) - height(x->right);
    }

    Node* restoreBalance(Node* x) {

        if(balance(x) < -1) // left < right-1
        {
            if (balance(x->right)>0)
                x->right = rotateRight( x->right );
            x = rotateLeft(x);
        }
        else if( balance(x) > 1) // left > right+1
        {
            if ( balance(x->left) < 0 )
                x->left = rotateLeft( x->left );
            x = rotateRight(x);
        }
        else updateNodeHeight(x);
        return x;
    }

    void updateNodeHeight(Node* x) {
        x->nodeHeight = std::max(height(x->right),height(x->left)) + 1;
    }

    Node* rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;

        y->nodeSize = x->nodeSize;
        updateNodeSize(x);

        updateNodeHeight(x);
        updateNodeHeight(y);
        return y;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;

        y->nodeSize = x->nodeSize;
        updateNodeSize(x);

        updateNodeHeight(x);
        updateNodeHeight(y);
        return y;
    }

    void updateNodeSize(Node* x) {
        x->nodeSize = size(x->right) + size(x->left) + 1;
    }

    int size(Node* x) {
        return ( x == nullptr ) ? 0 : x->nodeSize;
    }
};

#endif //LABO05_TERNARYSEARCHTRIE_H