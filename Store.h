#pragma once
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
#include"Todo.h"



using namespace std;
struct Todo;




class Store {
public:
    map<int64_t, map<string, string> > mp;  //our store
    string Name;


    Store(string name) : Name(name)  {};
   
    string to_string(double x);
    void Insertion(unique_ptr<Todo>a);

    void Show(map<int64_t, map<string, string> > mp);

    void Update(int64_t Id, string key1, string value);

    void Get(map<int64_t, map<string, string> > mp, int64_t Id);

    void Remove(int64_t Id);

    void Query(map<int64_t, map<string, string> > mp, string key1, string value);

    void Range_query(map<int64_t, map<string, string> > mp, string key1, double a, double b);

    struct Child {       //making another store for not commited changes, its' map has acess to the class Store map. 
        Child(shared_ptr <Store> obj, string name) :Chmap{ obj->mp }, obj1(obj.get()), Name{ move(name) } {};
        map<int64_t, map<string, string> >  Chmap;
        string Name = "";
        Store* obj1;

        void Get(int64_t Id);
        void Update(int64_t Id, string key1, string value);

        void Query(string key1, string value);

        void Show();
        void Range_query(string key1, double a, double b);

        ~Child() { cout << "D-tor\n"; };
    };

    void Sync(map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >& stCh);

    void Commit(shared_ptr<Store::Child > T, map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >& stCh);




    ~Store() {};



};


