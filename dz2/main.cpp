#include <iostream>
#include "Btree.h"

#include "ListCustomers.h"
#include "FileIO.h"

#include <string>
#include <sstream>



using namespace std;

Btree * bTree= nullptr;

ifstream datCustomer;

string customerAccountName = "CustomerAccount20.txt"; // IME FAJLA KOJI SE OTVARA


ListCustomers *listC = nullptr;
void Option1()
{
    int opcija;
    cout<<"----->1. Stvaranje indeksa"<<endl;
    cout<<"----->2. Unistavanje indeksa"<<endl;
    cout<<"Izaberite opciju: ";
    cin>>opcija;

    if(opcija == 1)
    {
        // stvaranje indeksa

        if(bTree)
        {
            cout<<"Stablo je vec stvoreno!"<<endl;
            return;
        }

        ifstream datCustomerAccount;
        datCustomerAccount.open(customerAccountName);

        if (!datCustomer.is_open()) exit(1);

        int m;
        cout<<"Unesite red stabla [3-10]: ";
        cin >> m;
        if(m<3 || m > 10)
        {
            cout<<"Uneli ste nevalidni red stabla!"<<endl;
            Option1();
        }

        bTree = new Btree(m);

        if(!datCustomer.is_open()) exit(1);


        AddFileToATree(datCustomerAccount,*bTree);

        cout<<"Stablo uspesno kreirano!"<<endl;

        datCustomerAccount.close();


    }
    else if(opcija == 2)
    {
        // unistavanje indeksa
        if(!bTree)
        {
            cout << "Stablo je vec unisteno!" <<endl;
            return;

        }
        delete bTree;
        bTree = nullptr;

        cout << "Stablo uspesno unisteno!"<<endl;
    }
    else
    {
        cout <<"Uneli ste nevalidnu opciju!"<<endl;
    }

}

void Option2()
{
    //Ispis indeksa
    if(!bTree)
    {
        cout<<"Stablo nije stvoreno!"<<endl;
        return;
    }
    cout<<*bTree;
}

void Option3()
{
    //cout<<"3. Ispisivanje podataka o korisniku i pretraga svih njegovih racuna"<<endl;

    int cnt;

    long long primaryCKey;
    cout<<"Unesite ID Customer za pretrazivanje racuna: ";
    cin>>primaryCKey;
    ListCustomers::Info *customerInfo = listC->findCustomer(primaryCKey);

    if(!customerInfo) {
        cout << "Nije pronadjen Customer sa ukucanim ID-jem!" << endl;
        return;
    }

    string fileName;
    cout<<"Unesite ime fajla za ispis: ";
    cin >> fileName;

    ofstream outputFile;
    outputFile.open(fileName+".txt");
    if(!outputFile.is_open()) exit(1);

    List* accounts = bTree->findKey(customerInfo->c_id,cnt);

    stringstream ss;
    ss<<*customerInfo;
    string prefix;
    ss>>prefix;
    PrintFoundAccounts(outputFile,prefix,accounts);
    cout<<"Broj koraka je "<<cnt<<"!"<<endl;
    cout<<"Lista uspesno ispisana u fajl "<<fileName<<"!"<<endl;



    outputFile.close();

}

void Option4()
{
    //4. Dodavanje novog zapisa u indeks

    cout<<"Ukucajte zapis koji zelite da unesete u formatu:"<<endl;
    cout<<"format: CA_ID|CA_B_ID|CA_C_ID|CA_NAME|CA_TAX_ST|CA_BAL"<<endl;

    string* row = getOneRowAcc(cin);
    long long ca_id = stoll(row[0]);
    long long ca_b_id = stoll(row[1]);
    long long ca_c_id = stoll(row[2]);
    string ca_name = row[3];
    int ca_tax_st = stoi(row[4]);
    double ca_bal = stod(row[5]);

    delete[] row;

    if(!bTree->isThisUniqueKeyForTheAccountKey(ca_id))
    {
        cout<<"Primarni kljuc "<<ca_id<<" vec postoji!"<<endl;
        return;
    }

    bTree->addKey(ca_id,ca_b_id,ca_c_id,ca_name,ca_tax_st,ca_bal);
    cout << "Kljuc uspesno dodat!"<<endl;



}

void Option5()
{
    //5. Brisanje zapisa iz indeksa

    long long keyToRemove;
    cout<<"Unesite kljuc koji zelite da izbrisete"<<endl;
    cin>>keyToRemove;

    bTree->removeKeyFromTheTree(keyToRemove);
}

void Option6()
{
   // Brisanje svih racuna jednog CUSTOMER-a"

}
void IspisiMeni()
{
     cout<<"===================================================================="<<endl;
     cout<<"1. Stvaranje i unistavanje indeksa nad tabelom CUSTOMER_ACCOUNT"<<endl;
     cout<<"2. Ispis indeksa"<<endl;
     cout<<"3. Ispisivanje podataka o korisniku i pretraga svih njegovih racuna"<<endl;
     cout<<"4. Dodavanje novog zapisa u indeks"<<endl;
     cout<<"5. Brisanje zapisa iz indeksa"<<endl;
     cout<<"6. Brisanje svih racuna jednog CUSTOMER-a"<<endl;
     cout<<"7. Kraj programa"<<endl;
     cout<<"===================================================================="<<endl;

}

bool Meni()
{
    IspisiMeni();
    int option;
    cout<<"Unesite opciju: "<<endl;
    cin>>option;

    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

    switch (option) {
        case 1:
            Option1(); break;
        case 2:
            Option2(); break;
        case 3:
            Option3(); break;
        case 4:
            Option4(); break;
        case 5:
            Option5(); break;
        case 6:
            Option6(); break;
        case 7: return true;
        default:
            cout<<"Niste uneli validnu opciju!"<<endl;
            Meni();
    }

    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

    return false;
}


int main() {

    datCustomer.open("Customer.txt");
    if (!datCustomer.is_open()) exit(1);

    listC = MakeCustomerListFromAFile(datCustomer);


    while(true)
    {
        if(Meni()) break;
    }

    datCustomer.close();

    delete bTree;
    delete listC;

    return 0;

}


