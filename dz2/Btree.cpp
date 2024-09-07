//
// Created by Jasam on 20-Nov-23.
//

#include "Btree.h"
#include <iostream>
#include <string>
#include <cmath>
#include <queue>

using namespace std;
//  h            h
//  l
//0 1 2  3  4  5
//1 4 6  8  15 20
//9? 5?

int Btree::KeyBinSearch(long long keyToFind,Node* node, Node*& nextNode) const
{
    if(node == nullptr || node->n == 0) return -1;
    int low = 0;
    int high = node->n-1;

    int mid = 0;
    nextNode = nullptr;
    while(low<=high)
    {
        mid = (low+high)/2;

        if(node->keyArray[mid]->key == keyToFind)
        {
            return mid;
        }
        else if(node->keyArray[mid]->key > keyToFind)
        {
            high = mid-1;
        }
        else {
            low = mid + 1;
        }
    }

    if(node->keyArray[mid]->key<keyToFind)
    {
        nextNode = node->nodes[mid+1];
    }
    else
    {
        nextNode = node->nodes[mid];
    }
    return -1;
}

Btree::Btree(int m)
{
    this -> m = m;
    min = ::ceil(m/2);
}


Btree & Btree::addKey(long long ca_id, long long ca_b_id,long long ca_c_id,std::string ca_name,int ca_tax_st, double ca_bal)
{
    Key * keyToAdd = new Key;
    keyToAdd->key = ca_c_id;

    List::Info infoToAdd;
    infoToAdd.ca_b_id = ca_b_id;
    infoToAdd.ca_id = ca_id;
    infoToAdd.ca_c_id = ca_c_id;
    infoToAdd.ca_name = ca_name;
    infoToAdd.ca_tax_st = ca_tax_st;
    infoToAdd.ca_bal = ca_bal;

    keyToAdd->list.add(infoToAdd);

    addKeyToATree(keyToAdd);
    return *this;

}

Btree & Btree::addKeyToATree(Key *keyToAdd)
{
    Key* foundKey;
    Node* foundNode, *prevNode;

//    cout<<"^^^^^^^^^^^^^^^^^^^"<<endl;
//    cout<<*this<<endl;
//    cout<<"^^^^^^^^^^^^^^^^^^"<<endl;


    if(!Search(keyToAdd->key,foundKey,foundNode,prevNode))
    {
        AddingKey addingKey;
        addingKey.KeyToAdd = keyToAdd;
        addKeyToANode(prevNode,addingKey);
    }
    else
    {
        foundKey->list.add(keyToAdd->list.getHeadInfo());
    }

    return *this;
}

Btree::Node* Btree::makeNewNode(AddingKey *addingKey)
{
    Node* newNode = new Node(this);
    newNode->n =0 ;
    if(addingKey) {
        newNode->keyArray[0] = addingKey->KeyToAdd;
        newNode->nodes[0] = addingKey->left;
        newNode->nodes[1] = addingKey->right;

        if (addingKey->left != nullptr) {
            addingKey->left->parent = newNode;
            newNode->isLeaf = false;
        }
        if (addingKey->right != nullptr) {
            addingKey->right->parent = newNode;
            newNode->isLeaf = false;
        }

        newNode->n = 1;
    }
    return newNode;
}

Btree::Node* Btree::moveKeysToTheRightFromPos(int pos,Node* node)
{
    node -> nodes[node -> n+1] = node->nodes[node->n];
    for(int j = node->n; j > pos;j--)
    {
        node -> keyArray[j] = node -> keyArray[j-1];
        node -> nodes[j] = node -> nodes[j-1];
    }
    node->nodes[pos] = nullptr;
    return node;
}

Btree::Node* Btree::moveKeysToTheLeftFromPos(int pos,Node* node)
{

    for(int j = pos; j < node->n;j++)
    {
        node -> keyArray[j] = node -> keyArray[j+1];
        node -> nodes[j] = node -> nodes[j+1];
    }

    node -> nodes[node -> n] = node->nodes[node->n+1];
    node->nodes[node->n+1] = nullptr;
    return node;
}


Btree::Node * Btree::addKeyToANode(Node* node,AddingKey addingKey)
{
    if(node == nullptr)
    {
        Node* newNode = makeNewNode(&addingKey);
        newNode -> n = 1;
        root = newNode;
        height++;
        return newNode;
    }

    if(node -> n < m-1)
    {
        int i = findPosOfAFirstKeyLargerThen(node,addingKey.KeyToAdd);

        node = moveKeysToTheRightFromPos(i,node);

        node -> keyArray[i] = addingKey.KeyToAdd;
        node -> nodes[i] = addingKey.left;
        node -> nodes[i+1] = addingKey.right;

        if(addingKey.left != nullptr) {
            addingKey.left->parent = node;
            node->isLeaf = false;
        }
        if(addingKey.right != nullptr) {
            addingKey.right->parent = node;
            node->isLeaf = false;
        }

        node -> n ++;
        return node;
    }

    Node * currentNode = node;
    if(currentNode->n == m-1)
    {
        int addingKeyIndex;
        Key** temp = makeTmpArray(node, addingKey.KeyToAdd,addingKeyIndex);

        int mid = m /2;


        Node* left = new Node(this);
        Node* right = new Node(this);

        for (int i = 0,j = 0; i < m; ++i,j++) {

            AddingKey keyToAdd;
            keyToAdd.KeyToAdd = temp[i];



            if(temp[i]->key != addingKey.KeyToAdd->key)
            {
                keyToAdd.left = currentNode->nodes[j];
                keyToAdd.right = currentNode->nodes[j + 1];
            }
            else
            {

                keyToAdd.left = addingKey.left;
                keyToAdd.right = addingKey.right;
                j--;
            }

            //preuzimanje pokazivaca dece od cvora koji se dodaje, jer je neki od njihove dece obrisan u procesu spajanja
            if(i-1 == addingKeyIndex)
            {
                keyToAdd.left=addingKey.right;
            }
            else if (i+1 == addingKeyIndex)
            {
                keyToAdd.right = addingKey.left;
            }


            if(i < mid)
            {
                addKeyToANode(left,keyToAdd);
            }
            else if(i > mid)
            {
                addKeyToANode(right,keyToAdd);
            }

        }

        AddingKey midAddingKey;
        midAddingKey.KeyToAdd = temp[mid];
        midAddingKey.left = left;
        midAddingKey.right = right;

        delete[] temp;

        Node* parent = currentNode->parent;

        if(parent) parent->nodes[findIndexOfAChildNode(parent,currentNode)] = nullptr;
        delete currentNode;

        return addKeyToANode(parent,midAddingKey);
    }

}
List* Btree::findKey(long long key, int & cnt) const
{
    Key* foundKey = nullptr;
    Node* prevNode = nullptr;
    Node* foundNode = nullptr;
    if(!Search(key, foundKey,prevNode,foundNode,cnt))return nullptr;

    return &foundKey->list;

}

List* Btree::findKey(long long key) const
{
    int a;
    return findKey(key,a);
}



int Btree::findPosOfAFirstKeyLargerThen(Node * node, Key * keyToCompare)
{

    for (int i = 0; i < node->n; ++i) {
        if(node->keyArray[i] -> key > keyToCompare->key)
        {
            return i;
        }
    }

    return node->n;
}


Btree::Key** Btree::makeTmpArray(Node* node,Key * keyToAdd,int& addingKeyIndex)
{
    Key **temp = new Key*[m];
    bool flag = false;
    addingKeyIndex = -1;
    for (int i = 0; i < m-1; ++i) {
        if(node->keyArray[i] -> key > keyToAdd->key && !flag)
        {
            addingKeyIndex = i;
            temp[i] = keyToAdd;
            flag = true;
        }
        else
        {
            temp[i] = node->keyArray[i - flag];
        }
    }

    temp[m-1] = flag ? node->keyArray[m-2] : keyToAdd;

    addingKeyIndex = addingKeyIndex==-1? m-1:addingKeyIndex;

    return temp;
}

bool Btree::Search(long long primaryKey, int &foundKeyIndex,Node* & prevNode, Node* & foundNode,int & cntSteps) const
{
    if(root == nullptr) {
        prevNode = root;
        return false;
    }

    foundKeyIndex = -1;

    Node* p = root;
    prevNode = nullptr;

    cntSteps = 1;

    while(p!= nullptr) {
        prevNode = p;

        Node* nextNode = nullptr;
        int index = KeyBinSearch(primaryKey,p,nextNode);

        if(index>-1)
        {
            foundNode = p;
            foundKeyIndex = index;
            return true;
        }

        p = nextNode;
        cntSteps ++;

    }

    foundNode = nullptr;
    return false;
}


bool Btree::Search(long long primaryKey, Key* & foundKey,Node* & foundNode, Node* & prevNode) const
{
    int br ;
    return Search(primaryKey,foundKey,foundNode,prevNode, br);
}

bool Btree::Search(long long primaryKey, Key* & foundKey,Node* & foundNode, Node* & prevNode, int & cntSteps) const
{
    int foundKeyIndex;
    if(Search(primaryKey,foundKeyIndex,prevNode,foundNode,cntSteps))
    {
        foundKey = foundNode->keyArray[foundKeyIndex];
        return true;
    }

    return false;
}

ostream & operator<<(ostream& it, Btree & treeToPrint){

    treeToPrint.print();
    return it;
}

bool Btree::isThisUniqueKeyForTheAccountKey(long long accKeyToCompare)
{
    //POBOLJSAJJJJ!!!!!!!!
    queue<Node *> q;
    q.push(root);

    while(!q.empty())
    {
        Node* current = q.front();
        q.pop();

        for (int i = 0; i < current->n; ++i) {
            if(current->keyArray[i]->list.findInfoThatPrimaryKeyEqualsTo(accKeyToCompare))
                return false;

            if(current->nodes[i])
                q.push(current->nodes[i]);

        }

        if(current->nodes[current->n])

            q.push(current->nodes[current->n]);

    }

    return true;

}


void Btree::print() const
{

    if (!root) return;

    if(root->isLeaf && root->n == 1)
    {
        cout<<root->keyArray[0]->key<<endl;
        return;
    }

    queue<Node *> q;
    int i, line_len = 120;
    int first_skip = line_len, in_between_skip;

    q.push(root);
    for (i = 0; i <= height; i++) {
        int j = pow(m, i), k, l;
        in_between_skip = first_skip;
        first_skip = (first_skip - m) / m;

        for (k = 0; k < first_skip; k++) cout << ' ';

        for (k = 0; k < j; k++) {

            Node *current = q.front();
            q.pop();

            if (current) {
                for (l = 0; l < m; ++l) {
                    if(l<=current->n) {
                        q.push(current->nodes[l]);
                    }
                    else
                    {
                        q.push(nullptr);
                    }
                }
            } else {
                for (l = 0; l < m; ++l) {
                    q.push(nullptr);
                }
            }

            if (current) {
                for (l = 0; l < current->n; ++l) {
                    cout << current->keyArray[l]->key;
                    if(l < current->n-1) cout<<"|";
                }
            }
            else cout << " ";

            for (l = 0; l < in_between_skip; l++) cout << ' ';
        }
        cout << endl << endl;
    }
}

int Btree::findIndexOfAChildNode(Node* parent, Node* child) const
{
    for (int i = 0; i < parent->n+1; ++i) {
        if(parent->nodes[i] == child)
            return i;
    }
    return -1;
}

Btree::Node* Btree::findSuccesor(int keyToFindSuccesorIndex,Node* currentNode, int& succesorIndex) const {
    succesorIndex = 0;
    Node* succNode = nullptr;

    succNode = currentNode->nodes[keyToFindSuccesorIndex + 1];

    while(succNode->nodes[0])
    {
        succNode = succNode->nodes[0];
    }

    return succNode;


}

Btree & Btree::removeEmptyNodeFromTheTree(Node* emptyNode,int emptyNodeIndex)
{
    if(emptyNode == root ) {
        root = emptyNode->nodes[0];
        return *this;
    }
    Node* parent = emptyNode->parent;
    AddingKey keyToAdd;


    Node* leftSon = parent->nodes[emptyNodeIndex == 0? 0:emptyNodeIndex-1];
    Node* rightSon = parent->nodes[emptyNodeIndex == 0?1 :emptyNodeIndex];

    emptyNodeIndex = emptyNodeIndex == parent->n ? emptyNodeIndex-1:emptyNodeIndex;

    keyToAdd.KeyToAdd=parent->keyArray[emptyNodeIndex];

    keyToAdd.left = leftSon->nodes[leftSon->n];

    keyToAdd.right = rightSon->nodes[0];

    parent->n--;
    addKeyToANode(leftSon,keyToAdd);


    Node* additionalNode =rightSon;// emptyNode == leftSon? leftSon : rightSon;// rightSon : leftSon; //???
    Node* nodeToAddKeys = leftSon;//emptyNode == leftSon? rightSon : leftSon;//leftSon : rightSon; //??
    for (int i = 0; i <additionalNode->n; ++i) {
        keyToAdd.KeyToAdd = additionalNode->keyArray[i];
        keyToAdd.right = additionalNode->nodes[i+1];
        keyToAdd.left = additionalNode->nodes[i];
        addKeyToANode(nodeToAddKeys,keyToAdd);
        }



    if(parent->n < min )
    {
        if(parent == root)
        {root = nodeToAddKeys; height--;}
        else removeEmptyNodeFromTheTree(parent, parent->parent?findIndexOfAChildNode(parent->parent,parent):-1);
    }


    delete additionalNode ;
    return *this;
}


Btree & Btree::removeKeyFromTheTree(Node* foundNode,int foundKeyIndex,bool removeWholeList)
{

//    if(foundNode->n < min)
//    {
//        // brisanje cvora koji ima manje od min kljuceva (foundKeyIndex u tom slucaju predstavlja poziciju spojenog noda)
//        return removeEmptyNodeFromTheTree(foundNode,foundKeyIndex);
//    }
    Key* foundKey = foundNode->keyArray[foundKeyIndex];

    long long keyToRemove = foundNode->keyArray[foundKeyIndex]->key;

    if(!removeWholeList)
    {
            foundKey->list.remove(keyToRemove);
            if(!foundKey->list.isEmpty())
            {
                return *this;
            }
    }

    if(foundNode->isLeaf)
    {
        if(foundNode->n > min || foundNode == root)
        {
            //1. slucaj: brise se iz lista u kome nakon brisanja ostaje dovoljno kljuceva

            foundKey->list.remove(keyToRemove);

            delete foundKey;
            foundNode->keyArray[foundKeyIndex] = nullptr;
            moveKeysToTheLeftFromPos(foundKeyIndex, foundNode);
            foundNode->n--;

            if(root->n == 0) root = nullptr;

        }
        else if(foundNode->n == min)
        {
            //2. slucaj: pozajmica od desnog ili levog brata

            int parentKeyIndex = findIndexOfAChildNode(foundNode->parent,foundNode);

            Node* rightBrother = nullptr;
            if(parentKeyIndex+1 < foundNode->parent->n+1)
                rightBrother = foundNode->parent->nodes[parentKeyIndex+1];

            Node* leftBrother = nullptr;
            if(parentKeyIndex-1 >= 0)
                leftBrother = foundNode->parent->nodes[parentKeyIndex-1];

            if(rightBrother!= nullptr && rightBrother->n > min)
            {
                AddingKey keyToAdd;
                keyToAdd.right = foundNode->nodes[foundKeyIndex+1];
                keyToAdd.right = foundNode->nodes[foundKeyIndex];


                delete foundNode -> keyArray[foundKeyIndex];
                moveKeysToTheLeftFromPos(foundKeyIndex,foundNode);
                foundNode->n--;

                keyToAdd.KeyToAdd = foundNode->parent->keyArray[parentKeyIndex];
                addKeyToANode(foundNode,keyToAdd);


                foundNode->parent->keyArray[parentKeyIndex] = rightBrother->keyArray[0];
                moveKeysToTheLeftFromPos(0,rightBrother);

                rightBrother->n--;
            }
            else if(leftBrother != nullptr && leftBrother->n > min)
            {
                AddingKey keyToAdd;
                keyToAdd.right = foundNode->nodes[foundKeyIndex+1];
                keyToAdd.right = foundNode->nodes[foundKeyIndex];


                delete foundNode -> keyArray[foundKeyIndex];
                moveKeysToTheLeftFromPos(foundKeyIndex,foundNode);
                foundNode->n--;

                keyToAdd.KeyToAdd = foundNode->parent->keyArray[parentKeyIndex-1];
                addKeyToANode(foundNode,keyToAdd);

                foundNode->parent->keyArray[parentKeyIndex-1] = leftBrother->keyArray[leftBrother->n-1];
                moveKeysToTheLeftFromPos(leftBrother->n-1,leftBrother);

                leftBrother->n--;
            }
            else
            {
                Node* parent = foundNode->parent;

                Node* leftSon = parent->nodes[parentKeyIndex ==0 ?0:parentKeyIndex-1];
                Node* rightSon = parent->nodes[parentKeyIndex ==0 ? 1:parentKeyIndex];

                delete foundKey;
                foundNode->n--;
                moveKeysToTheLeftFromPos(foundKeyIndex,foundNode);

                parentKeyIndex = parentKeyIndex==parent->n? parentKeyIndex-1:parentKeyIndex;

                AddingKey keyToAdd;
                keyToAdd.KeyToAdd = parent->keyArray[parentKeyIndex];
                 parent->n--;
                if(parent->n >= min)
                    moveKeysToTheLeftFromPos(parentKeyIndex,parent);

                addKeyToANode(leftSon,keyToAdd);
                for (int i = 0; i < rightSon->n; ++i) {
                    keyToAdd.KeyToAdd =rightSon->keyArray[i];
                    addKeyToANode(leftSon,keyToAdd);
                }


                if( parent->n < min)
                    removeEmptyNodeFromTheTree(parent, parent->parent?
                    findIndexOfAChildNode(parent->parent,parent):-1);
               // delete rightSon;




            }
        }
    }
    else
    {
        //4. slucaj: brisanje iz unutrasnjeg cvora
        int succIndex = -1;
        Node* succNode = findSuccesor(foundKeyIndex,foundNode,succIndex);

        Key* t = foundKey;
        foundNode->keyArray[foundKeyIndex] = succNode->keyArray[succIndex];
        succNode->keyArray[succIndex] = t;

        removeKeyFromTheTree(succNode,succIndex);


    }

    return *this;
}


Btree & Btree::removeKeyFromTheTree(long long keyToRemove)
{
    if(root == nullptr) return *this;

    Node* foundNode, *prevNode;
    int foundKeyIndex;
    int cnt;
    if(!Search(keyToRemove, foundKeyIndex,  prevNode,foundNode, cnt))
        return *this;


    return removeKeyFromTheTree(foundNode,foundKeyIndex);
}

Btree::~Btree()
{
    //POBOLJSAJJJJ!!!!!!!!
    queue<Node *> q;
    q.push(root);

    while(!q.empty())
    {
        Node* current = q.front();
        q.pop();


        for (int i = 0; i < current->n; ++i) {
            if(current->nodes[i])
                q.push(current->nodes[i]);

            delete current->keyArray[i];

        }
        if(current->nodes[current->n])
            q.push(current->nodes[current->n]);

        delete current->keyArray;
        delete current->nodes;

        delete current;
    }
}


