//
//  BinarySearchTree.h
//  SearchTrees
//
//  Created by Olivier Cuisenaire on 26.11.14.
//  Copyright (c) 2014 Olivier Cuisenaire. All rights reserved.
//

#ifndef SearchTrees_BinarySearchTree_h
#define SearchTrees_BinarySearchTree_h

#include <algorithm>

template < typename KeyType, typename ValueType >
class BinarySearchTree {
private:
    
    //
    // Noeud de l'arbre. contient une cle, une valeur, et les liens vers les
    // sous-arbres droit et gauche.
    //
    struct Node {
    public:
        KeyType key;
        ValueType value;
        Node* right; // sous arbre avec des cles plus grandes
        Node* left;  // sous arbre avec des cles plus petites
        int nodeSize; // taille du sous-arbre dont ce noeud est la racine.
        Node(KeyType key, ValueType value) : key(key),value(value), right(nullptr), left(nullptr), nodeSize(1) { }
    };
    
    //
    // Racine de l'arbre.
    //
    Node* root;

    // HELPER: Methode aide pour le methodes d'effacement. Efface del et retourne ret.
    Node* deleteAndReturn( Node* del, Node* ret) {
        delete del;
        return ret;
    }
    
    //
    // Constructeur. La racine est vide
    //
public:
    BinarySearchTree() : root(nullptr) { }
    
    
    //
    // Destructeur.
    //
public:
    ~BinarySearchTree() {
        deleteSubTree( root );
    }
private:
    void deleteSubTree(Node* x) {
        if( x == nullptr) return;
        deleteSubTree( x->right );
        deleteSubTree( x->left );
        delete x;
    }
    
    //
    // Nombre d'elements
    //
public:
    int size() {
        return size(root);
    }
private:
    int size(Node* x) {
        return ( x == nullptr ) ? 0 : x->nodeSize;
    }
    
    
    //
    // Insertion d'une paire cle / valeur.
    //
public:
    void put( const KeyType& key, ValueType value) {
        root = put(root,key,value);
    }
    
private:
    Node* put(Node* x, const KeyType& key, ValueType value) {
        if (x==nullptr)
            return new Node(key, value);
        
        if ( key < x->key )
            x->left = put(x->left,key,value);
        else if ( key > x->key)
            x->right = put(x->right,key,value);
        else // x->key == key
            x->value = value;
        x->nodeSize = 1 + size(x->left) + size(x->right);
        return x;
    }

    
    //
    // Recherche d'une cle. Renvoie true et modifie
    // value si la cle est trouvee. Renvoie false et
    // ne modifie pas value sinon.
    //
public:
    bool get( const KeyType& key, ValueType& value ) {
        Node* x = root;
        while ( x != nullptr ) {
            if ( key < x->key )
                x = x->left;
            else if ( key > x->key)
                x = x->right;
            else // x->key == key.
            {
                value = x->value;
                return true;
            }
        }
        return false;
    }
    
    //
    // idem précédent, mais sans retour de la valeur
    //
    bool contains( const KeyType& key ) {
        Node* x = root;
        while ( x != nullptr ) {
            if ( key < x->key )
                x = x->left;
            else if ( key > x->key)
                x = x->right;
            else // x->key == key.
                return true;
        }
        return false;
    }

    
    //
    // Profondeur de l'arbre.
    //
public:
    int height()
    {
        return height(root);
    }
private:
    int height(Node* x) {
        if ( x == nullptr )
            return -1;
        return std::max( height(x->right) , height(x->left) ) + 1;
    }
    
    
    //
    // Efface le plus petit element de l'arbre.
    //
public:
    void deleteMin() {
        if (root != nullptr) {
            root = deleteMin(root);
        }
    }
private:
    Node* deleteMin(Node* x) {
        if (x->left == nullptr)
            return deleteAndReturn( x, x->right );
        x->left = deleteMin(x->left);
        x->nodeSize--;
        return x;
    }
    
    
    //
    // Efface le plus grand element de l'arbre.
    //
public:
    void deleteMax() {
        if (root != nullptr) {
            root = deleteMin(root);
        }
    }
private:
    Node* deleteMax(Node* x) {
        if (x->right == nullptr)
            return deleteAndReturn( x, x->left );
        x->left = deleteMin(x->right);
        x->nodeSize--;
        return x;
    }

    
    //
    // Efface l'element de cle key
    //
public:
    void deleteElement( const KeyType& key) {
        root = deleteElement( root, key );
    }
private:
    Node* deleteElement( Node* x, const KeyType& key) {
        if ( x == nullptr )
            return nullptr; // element pas trouve.
        
        if ( key < x->key )
            x->left = deleteElement( x->left, key );
        else if ( key > x->key )
            x->right = deleteElement( x->right, key );
        else { // x->key == key
            if ( x->right == nullptr )
                return deleteAndReturn(x,x->left);
            if ( x->left == nullptr )
                return deleteAndReturn(x,x->right);
            
            Node* y = min(x->right);
            x->key = y->key;
            x->value = y->value;
            x->right = deleteMin(x->right);
        }
        x->nodeSize = 1 + size(x->left) + size(x->right);
        return x;
    }

    //
    // Visite de l'arbre par ordre croissant de cle
    //
public:
    template < typename Fn >
    void visitInOrder ( Fn f) {
        if ( root != nullptr)
            visitInOrder(root,f);
    }
private:
    template < typename Fn >
    void visitInOrder ( Node* x, Fn f) {
        if ( x->left != nullptr)
            visitInOrder(x->left,f);
        
        f( x->key, x->value );
        
        if ( x->right != nullptr)
            visitInOrder(x->right,f);
    }
    
    
    //
    // Renvoie la cle la plus petite de l'arbre
    //
public:
    KeyType min() {
        return min(root)->key;
    }
private:
    Node* min(Node* x) {
        if( x->left == nullptr )
            return x;
        return min(x->left);
    }
    
    //
    // Renvoie la cle la plus grande de l'arbre
    //
public:
    KeyType max() {
        return max(root)->key;
    }
private:
    Node* max(Node* x) {
        if( x->right == nullptr )
            return x;
        return max(x->right);
    }
    
    
    //
    // Renvoie le rang (nombre de cles plus petites) d'une cle
    //
public:
    int rank(const KeyType& key) {
        return rank(root, key);
    }
private:
    int rank(Node* x, const KeyType& key) {
        if ( x == nullptr ) return 0;
        
        if ( key < x->key )
            return rank( x->left, key);
        else if ( key > x->key )
            return 1 + size(x->left) + rank( x->right, key);
        else return size(x->left);
    }
    
    //
    // Renvoie la clé de rang rank. Permet de mettre en oeuvre un
    // operator [] par exemple.
    //
public:
    KeyType select(int rank) { // 0 <= rank < size()
        return select(root,rank).key;
    }
private:
    Node* select(Node* x, int rank) {
        int r = size(x->left);
        if ( r > rank )
            return select( x->left, rank );
        else if ( r < rank )
            return select( x->right, rank );
        else // r == rank
            return x;
    }
};


#endif
