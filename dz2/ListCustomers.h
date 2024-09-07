//
// Created by Jasam on 23-Nov-23.
//

#ifndef ASP2DZ2_LISTCUSTOMERS_H
#define ASP2DZ2_LISTCUSTOMERS_H

#include "List.h"
#include <string>

class ListCustomers {
public:
    struct Info{
        long long c_id;
        std::string c_f_name;
        std::string c_l_name;
        std::string c_email_1;
        long long c_ad_id;

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
    ListCustomers() = default;
    ListCustomers(const ListCustomers & list);
    ListCustomers(ListCustomers && list);
    ~ListCustomers();

    friend std::ostream & operator<<(std::ostream & it, ListCustomers & list);


    Info * findCustomer(long long c_id) const;

    ListCustomers & add(Info infoToAdd);
    Info getHeadInfo() const;
};



#endif //ASP2DZ2_LISTCUSTOMERS_H
