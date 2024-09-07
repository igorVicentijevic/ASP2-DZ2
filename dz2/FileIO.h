//
// Created by Jasam on 24-Nov-23.
//

#ifndef ASP2DZ2_FILEIO_H
#define ASP2DZ2_FILEIO_H

#include "Btree.h"
#include <iostream>
#include "List.h"
#include "ListCustomers.h"

#include <fstream>
#include <string>


std::string* getOneRowAcc(std::istream& dat,int numOfStrings = 6);
std::string* getOneRowAcc(std::ifstream& dat,int numOfStrings=6);
void AddFileToATree(std::ifstream &, Btree & tree);
ListCustomers* MakeCustomerListFromAFile(std::ifstream & dat);

void PrintToFile(std::ofstream & fileToPrint,List & listToPrint);

void PrintFoundAccounts(std::ofstream & it,std::string prefix, List* accounts);

#endif //ASP2DZ2_FILEIO_H
