#include "Store.h"
#include <cassert>
#include <iostream>
#include<map>
#include<sstream>
#include<set>
#include<memory>
#include<thread>
#include<vector>
#include<algorithm>
#include<ctime>
#include<chrono>
#include<iomanip>
#include<string>



using namespace std;

struct Todo;






string Store::to_string(double x) {  //converting double into string type
    ostringstream sstr;
    sstr << x;
    return sstr.str();
}
void Store::Insertion(unique_ptr<Todo>a) {
    map<string, string>mp1;
    mp1["AHEADING"] = a->heading;
    mp1["Description"] = a->description;
    mp1["Timestamp"] = to_string(a->timestamp);
    mp1["Priority"] = a->priority;
    mp1["BGroup"] = a->group;
    pair<int64_t, map<string, string> > pr(a->id, mp1);
    Store::mp.insert(pr);
    mp1.erase(mp1.begin(), mp1.end());

}

void Store::Show(map<int64_t, map<string, string> > mp) {
    map<int64_t, map<string, string> > ::iterator iter;
    for (iter = mp.begin(); iter != mp.end(); iter++) {
        cout << iter->first << ": " << endl;
        for (auto it1 = iter->second.begin(); it1 != iter->second.end(); it1++) {
            cout << it1->first << ": ";
            for (auto it2 : it1->second) {
                cout << it2 << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

}

void Store::Update(int64_t Id, string key1, string value) {   //key1 is the key of the nested map(mp1), value= new meaning for this key

    map<int64_t, map<string, string> > ::iterator iter = Store::mp.find(Id);
    for (auto it1 = iter->second.begin(); it1 != iter->second.end(); it1++)
        if (it1->first == key1) {
            it1->second = value;
        }

}

void Store::Get(map<int64_t, map<string, string> > mp, int64_t Id) {
    map<int64_t, map<string, string> > ::iterator iter = mp.find(Id);
    for (auto it1 = iter->second.begin(); it1 != iter->second.end(); it1++) {
        /*cout << it1->first << ": ";*/
        for (auto it2 : it1->second) {
            cout << it2 << " ";
        }
        cout << endl;
    }
}

void Store::Remove(int64_t Id) {
    Store::mp.erase(Id);
}

void Store::Query(map<int64_t, map<string, string> > mp, string key1, string value) {    //returns set over here, also can return as vector for example
    set< int64_t>st1;
    map<int64_t, map<string, string> > ::iterator iter;
    for (iter = mp.begin(); iter != mp.end(); iter++) {
        for (auto it1 = iter->second.begin(); it1 != iter->second.end(); it1++) {
            if (it1->first == key1 && it1->second == value) {
                st1.insert(iter->first);
            }
        }
    }

    set< int64_t>::iterator it;
    cout << "Object, which value of " << key1 << " is " << value << ":" << endl;
    for (it = st1.begin(); it != st1.end(); it++) {
        cout << *it << " ";
    }

}

void Store::Range_query(map<int64_t, map<string, string> > mp, string key1, double a, double b) {
    set< int64_t>st1;
    map<int64_t, map<string, string> > ::iterator iter;
    for (iter = Store::mp.begin(); iter != Store::mp.end(); iter++) {
        for (auto it1 = iter->second.begin(); it1 != iter->second.end(); it1++) {
            if (it1->first == key1 && (it1->second >= to_string(a) && it1->second <= to_string(b))) {
                st1.insert(iter->first);
            }
        }
    }
    set< int64_t>::iterator itR;
    for (itR = st1.begin(); itR != st1.end(); itR++) {
        cout << *itR << " ";
    }

}



void Store::Child::Get(int64_t Id) {

    return obj1->Get(Chmap, Id);
}
void Store::Child::Update(int64_t Id, string key1, string value) {   //key1 is the key of the nested map(mp1), value= new meaning for this key

    map<int64_t, map<string, string> > ::iterator iter = Store::Child::Chmap.find(Id);
    for (auto it1 = iter->second.begin(); it1 != iter->second.end(); it1++)
        if (it1->first == key1) {
            it1->second = value;
        }


}

void Store::Child::Query(string key1, string value) {

    return obj1->Query(Chmap, key1, value);

}
void Store::Child::Range_query(string key1, double a, double b) {
    return obj1->Range_query(Chmap, key1, a, b);
}

void Store::Child::Show() {
    return obj1->Show(Chmap);



}



void Store::Sync(map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >& stCh) {   //sync all the child store with the main store

    map <shared_ptr <Store>, vector<shared_ptr<Store::Child > > >::iterator iter;
    for (iter = stCh.begin(); iter != stCh.end(); iter++) {
        if (iter->first.get() == this) {

            for (auto it1 = iter->second.begin(); it1 != iter->second.end(); it1++) {

                (*it1)->Chmap = this->mp;

            }
        }

    }


}

void Store::Commit(shared_ptr<Store::Child > T, map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >& stCh) { //commiting the changes from the child store into the main store and all other child store
    this->mp = T->Chmap;
    /* shared_ptr <Store> current_object;*/
    map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >::iterator iter;
    for (iter = stCh.begin(); iter != stCh.end(); iter++) {
        if (iter->first.get() == this) {
            break;

        }

    }
    stCh.find(iter->first)->second.erase(remove(stCh.find(iter->first)->second.begin(), stCh.find(iter->first)->second.end(), T), stCh.find(iter->first)->second.end());

    thread t(&Store::Sync, this, ref(stCh));
    t.join();

}






