//
// Created by Jasam on 21-Nov-23.
//

#include "List.h"
#include "Btree.h"

#include <iostream>

using namespace std;

List & List::add(Info infoToAdd)
{
    Node* newNode = new Node;
    newNode->info = infoToAdd;

    if(!head)
    {
        head = newNode;
    }
    else
    {
        tail -> next = newNode;
    }

    tail = newNode;

    return *this;
}

std::ostream & operator<<(std::ostream & it, List::Info & info)
{
    it<<fixed;
    it << info.ca_id<<"|"<<info.ca_c_id<<"|"<<info.ca_tax_st<<"|"<<info.ca_bal;
    return it;
}

List::Info List::getHeadInfo() const
{
    return head->info;
}

std::ostream & operator<<(std::ostream & it, List & list)
{
    if(list.head)
    {
        List::Node * p = list.head;
        while(p)
        {
            List::Info & info = p->info;
            it<<info<<endl;
            p = p->next;
        }
    }
    return it;
}


List::List(List && list)
{
    head = list.head;
    tail = list.tail;
    list.head = nullptr;
    list.tail = nullptr;
}

List::List(const List & list)
{
    head = list.head;

    Node* p = list.head;
    while(p)
    {
        add(p->info);
        p = p->next;
    }
}

List& List::remove(long long accKeyToRemove)
{
    Node* prev = nullptr;

    Node* found = findNodeThatPrimaryKeyEqualsTo(accKeyToRemove,prev);
    if(found == nullptr)
        return *this;

    if(prev)
    {
        prev -> next = found->next;
    }
    else
    {
        head = found->next;
    }

    delete found;
    return *this;
}

List::Node* List::findNodeThatPrimaryKeyEqualsTo(long long primaryKey, Node* & prev) const
{
    prev = nullptr;
    Node* p = head;

    while(p)
    {
        if(p->info.ca_c_id==primaryKey)//p->info.ca_id == primaryKey)
            return p;

        prev = p;
        p = p->next;
    }
    return nullptr;
}

List::Info * List::findInfoThatPrimaryKeyEqualsTo(long long keyValue) const
{
    Node* prev = nullptr;
    Node* p = findNodeThatPrimaryKeyEqualsTo(keyValue, prev);
    if(!p) return nullptr;

    return &p->info;
}


List::~List()
{
    while(head)
    {
        Node* t = head;
        head = head -> next;
        delete t;
    }
}
