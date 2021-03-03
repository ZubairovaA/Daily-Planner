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
#include"Store.h"



using namespace std;
struct Todo {
    Todo(int64_t i, string Title, string Description, double Timestamp, string _priority, string _group) :id{ move(i) }, heading{ move(Title) },
        description{ move(Description) }, timestamp{ move(Timestamp) }, priority{ move(_priority) }, group{ move(_group) }{};

    int64_t id;

    string heading;
    string description;
    double timestamp;
    string priority;
    string group;

    ~Todo() { }
    // and many more in the shipping app
};
