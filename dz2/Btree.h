//
// Created by Jasam on 20-Nov-23.
//

#ifndef ASP2DZ2_BTREE_H
#define ASP2DZ2_BTREE_H

#include <string>
#include "List.h"

//MODIFIKACIJE

//-Dodaj binarnu pretragu za trazenje kljuca u samom cvoru
//-Koristi binarnu za pretrazivanje da li postoji duplikat primarnog kljuca


class Btree {
public:

private:
    int m;
    int min;
    int height = -1;

    struct Key{
        long long key;
        List list;
    };

    struct Node{

        int n = 0;
        bool isLeaf = true;
        Node *parent = nullptr;
        Key **keyArray = nullptr;
        Node **nodes = nullptr;
        Node(Btree *bt)
        {
            keyArray = new Key*[bt->m-1];
            nodes = new Node*[bt->m]{nullptr};

        }
        ~Node()
        {
            delete[] nodes;
            delete[] keyArray;
        }
    };

    struct AddingKey
    {
        Key *KeyToAdd;
        Node* left = nullptr;
        Node* right = nullptr;
    };


    Node * root = nullptr;
private:
    Btree & addKeyToATree(Key * keyToAdd);
    Node * addKeyToANode(Node * node, AddingKey addingKey);
    Key** makeTmpArray(Node* node,Key * keyToAdd,int& addingKeyIndex);
    int findPosOfAFirstKeyLargerThen(Node * node, Key * keyToCompare);
    Node* makeNewNode(AddingKey *addingKey);
    Node* moveKeysToTheRightFromPos(int pos,Node* node);
    Node* moveKeysToTheLeftFromPos(int pos,Node* node);
    int findIndexOfAChildNode(Node* parent, Node* child) const;
    void print() const;

    Node* findSuccesor(int keyToFindSuccesorIndex,Node* currentNode,int& successorIndex) const;

    bool Search(long long primaryKey, Key* & foundKey,Node* & prevNode, Node* & foundNode) const;
    bool Search(long long primaryKey, int& foundKeyIndex,Node* & prevNode, Node*& foundNode, int & cntSteps) const;
    bool Search(long long primaryKey, Key* & foundKey,Node* & prevNode, Node* & foundNode, int & cntSteps) const;

    int KeyBinSearch(long long keyToFind,Node* node,Node*& nextNode) const;

    Btree & removeKeyFromTheTree(Node* foundNode,int foundKeyIndex,bool removeWholeList = false);
    Btree & removeEmptyNodeFromTheTree(Node* emptyNode,int emptyNodeIndex);

public:
    Btree(int m);
    Btree & addKey(long long ca_id,long long ca_b_id,/*primaryKey*/long long ca_c_id,std::string ca_name,int ca_tax_st, double ca_bal);

    List *findKey(long long key) const;
    List *findKey(long long key,int & cnt) const;

    Btree & removeKeyFromTheTree(long long keyToRemove);



    bool isThisUniqueKeyForTheAccountKey(long long accKeyToCompare);  //POBOLJSAJJJJ!!!!!!!!

    friend std::ostream & operator<<(std::ostream& it, Btree & treeToPrint);

    ~Btree();


};


#endif //ASP2DZ2_BTREE_H
