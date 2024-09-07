//
// Created by Jasam on 24-Nov-23.
//

#include "FileIO.h"
#include "Btree.h"
#include <iostream>
#include "List.h"
#include "ListCustomers.h"

#include <fstream>

#include <string>
#include <sstream>

#include <regex>

using namespace  std;

string* getOneRowAcc(ifstream& dat,int numOfStrings)
{
    string row;


    getline(dat,row);

    string* res = new string[numOfStrings];

    regex rgx("\\|");
    sregex_token_iterator iter(row.begin(),row.end(),rgx,-1);
    std::sregex_token_iterator end;

    for ( int i = 0; i < numOfStrings; i++)
    {
        if(iter == end) return nullptr;
        res[i] = iter->str();
        iter++;
    }

    return res;


}

string* getOneRowAcc(istream& dat,int numOfStrings)
{
    string row;

    dat>>row;

    string* res = new string[numOfStrings];

    regex rgx("\\|");
    sregex_token_iterator iter(row.begin(),row.end(),rgx,-1);
    std::sregex_token_iterator end;

    for ( int i = 0; i < numOfStrings; i++)
    {
        if(iter == end) return nullptr;
        res[i] = iter->str();
        iter++;
    }

    return res;
}
void AddFileToATree(ifstream & dat, Btree & tree)
{

    while (!dat.eof()) {

        string *red = getOneRowAcc(dat);
        if(!red) return;

        long long ca_id = stoll(red[0]);
        long long ca_b_id = stoll(red[1]);
        long long ca_c_id = stoll(red[2]);
        string ca_name = red[3];
        int ca_tax_st = stoi(red[4]);
        double ca_bal = stod(red[5]);

        delete[] red;

        List::Info info;
        info.ca_name = ca_name;
        info.ca_id = ca_id;
        info.ca_c_id = ca_c_id;
        info.ca_tax_st = ca_tax_st;
        info.ca_b_id = ca_b_id;
        info.ca_bal = ca_bal;

        tree.addKey(ca_id, ca_b_id, ca_c_id, ca_name, ca_tax_st, ca_bal);
    }

    dat.close();
}

ListCustomers* MakeCustomerListFromAFile(ifstream & dat)
{

    ListCustomers *list = new ListCustomers;

    while (!dat.eof()) {
        string *red = getOneRowAcc(dat,5);

        if(!red) return nullptr;

        long long c_id = stoll(red[0]);
        string c_f_name = red[1];
        string c_l_name = red[2];
        string c_email_1 = red[3];
        long long c_ad_id = stoll(red[4]);

        delete[] red;

        ListCustomers::Info info;
        info.c_id= c_id;
        info.c_f_name = c_f_name;
        info.c_l_name = c_l_name;
        info.c_email_1= c_email_1;
        info.c_ad_id=c_ad_id;

        list->add(info);
    }
    return list;
}

void PrintToFile(ofstream & fileToPrint,List & listToPrint)
{
    //C_ID|C_F_NAME|C_L_NAME|C_EMAIL_1|CA_ID|CA_C_ID|CA_TAX_ST|CA_BAL

    fileToPrint<<listToPrint<<endl;
}

void PrintFoundAccounts(ofstream & it,string prefix, List* accounts)
{


    if(accounts == nullptr)
    {
        it<<prefix<<"|null|null|null|null"<<endl;
    }
    else
    {
        List::Node * p = accounts->head;
        while(p)
        {
            it<<prefix<<'|'<<p->info<<endl;
            p = p->next;
        }
    }


}