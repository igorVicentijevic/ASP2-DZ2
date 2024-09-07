//
// Created by Jasam on 21-Nov-23.
//

#ifndef ASP2DZ2_LIST_H
#define ASP2DZ2_LIST_H
#include <string>

#include "ListCustomers.h"

class List {
public:
    struct Info{

        long long ca_id;
        long long ca_b_id;
        long long ca_c_id;
        std::string ca_name;
        int ca_tax_st;
        double ca_bal;

        friend std::ostream & operator<<(std::ostream & it, Info & info);
    };

private:

    struct Node{
        Node * next = nullptr;
        Info info;
    };

    Node* head = nullptr;
    Node* tail = nullptr;

public:
    List() = default;
    List(const List & list);
    List(List && list);
    ~List();

    friend std::ostream & operator<<(std::ostream & it, List & list);
    friend void PrintFoundAccounts(std::ofstream &,std::string prefix, List* accounts);

    Info * findInfoThatPrimaryKeyEqualsTo(long long keyValue) const;

    Node* findNodeThatPrimaryKeyEqualsTo(long long primaryKey, Node* & prev) const ;

    bool isEmpty() const
    {
        return head == nullptr;
    }
    List & add(Info infoToAdd);

    List& remove(long long accKeyToRemove);
    Info getHeadInfo() const;
};


#endif //ASP2DZ2_LIST_H
