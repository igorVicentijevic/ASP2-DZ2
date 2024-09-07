//
// Created by Jasam on 21-Nov-23.
//

#include "ListCustomers.h"
#include "Btree.h"

#include <iostream>

using namespace std;

std::ostream & operator<<(std::ostream & it, ListCustomers::Info & info)
{
    it << info.c_id<<"|"<<info.c_f_name<<"|"<<info.c_l_name<<"|"<<info.c_email_1;
    return it;
}



ListCustomers & ListCustomers::add(Info infoToAdd)
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

ListCustomers::Info ListCustomers::getHeadInfo() const
{
    return head->info;
}

std::ostream & operator<<(std::ostream & it, ListCustomers & list)
{
    if(list.head)
    {
        ListCustomers::Node * p = list.head;
        while(p)
        {
            ListCustomers::Info & info = p->info;
            it<<info<<endl;
            p = p->next;
        }
    }
    return it;
}


ListCustomers::Info * ListCustomers::findCustomer(long long c_id) const
{
    if(head == nullptr) return nullptr;

    Node* p = head;

    while(p)
    {
        if(p->info.c_id == c_id)
            return &p->info;

        p = p->next;
    }
    return nullptr;
}



ListCustomers::ListCustomers(ListCustomers && list)
{
    head = list.head;
    tail = list.tail;
    list.head = nullptr;
    list.tail = nullptr;
}

ListCustomers::ListCustomers(const ListCustomers & list)
{
    head = list.head;

    Node* p = list.head;
    while(p)
    {
        add(p->info);
        p = p->next;
    }
}

ListCustomers::~ListCustomers()
{
    while(head)
    {
        Node* t = head;
        head = head -> next;
        delete t;
    }
}
