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
#include<mutex>
#include<stdexcept>
#include<condition_variable>
#include"Store.h"
#include"Todo.h"


chrono::system_clock::time_point Tp_End = chrono::system_clock::now();
chrono::system_clock::time_point Tp_Next, Tp_Try;
condition_variable CV;
volatile bool Check = true;
bool is_printed() { return Check; }

using namespace std;

mutex MU, MU_Resourse;





int64_t Date(set<pair<chrono::system_clock::time_point, int64_t> >& Tp) {
    tm T = { 0 };
    static int64_t z = 0;
    cout << "Enter the date and time like year,month,day,hour,minuts" << endl;
    int yr, mth;
    point1:
    try {
        cin >> yr;
        if (yr < 2020) {
            throw invalid_argument("Invalid year, please enter the year again.");
        }

    }
    catch (invalid_argument & e) {
        cout << e.what();
        goto point1;
    }
    T.tm_year = yr - 1900;

    cin >> mth;
    T.tm_mon = mth - 1;
    cin >> T.tm_mday >> T.tm_hour >> T.tm_min;
    

    T.tm_sec = 0;
    chrono::system_clock::time_point X = chrono::system_clock::from_time_t(mktime(&T));
    if (X > Tp_End) {
        Tp_End = X;
    }
    z++;
    pair<chrono::system_clock::time_point, int64_t> pr{ X,z };
    Tp.insert(pr);
    return z;

}

ostream& operator<<(ostream& os, tm& T) {
    os << T.tm_year + 1900 << " " << T.tm_mon << " " << T.tm_mday << " " << T.tm_hour << " " << T.tm_min << " " << T.tm_sec;
    return os;
}
void f(set<pair<chrono::system_clock::time_point, int64_t> >& TP, map<shared_ptr<Store>, vector<shared_ptr<Store::Child > > >& Children) {
    set<pair<chrono::system_clock::time_point, int64_t> > ::iterator it;
    for (it = TP.begin(); it != TP.end(); it++) {
        {unique_lock<mutex>UL1(MU);

        Tp_Next = it->first;
        Tp_Try = it->first - chrono::seconds(5);
        }

        time_t  ON = std::chrono::system_clock::to_time_t(it->first);
        tm on;
        localtime_s(&on, &ON);
        int year1 = on.tm_year + 1900, month = on.tm_mon + 1, day = on.tm_mday;
        string S = "";
        ostringstream sstr;
        sstr << year1 << month << day;
        S = sstr.str();

        map<shared_ptr<Store>, vector<shared_ptr<Store::Child > > >::iterator it1;

        Check = false;
        this_thread::sleep_until(it->first);
        unique_lock<mutex>UL1(MU);

        cout << endl << "TIME :" << endl;
        for (it1 = Children.begin(); it1 != Children.end(); it1++) {

            if (it1->first->Name == S) {

                it1->first->Get(it1->first->mp, it->second);


            }

            break;

        }
        Check = true;
        CV.notify_one();
    }

}

void User_Request(map< shared_ptr <Store>, vector<shared_ptr<Store::Child > > >& Children) {




    while ((chrono::system_clock::now()) <= Tp_End) {

        string Answer1 = "", Answer = "", Object = "", Main_Object = "";
        bool Bl = true;

        if ((chrono::system_clock::now()) > Tp_Try) {
            this_thread::sleep_until(Tp_Next);
            unique_lock<mutex>UL1(MU);
            CV.wait(UL1, is_printed);

        }

        {unique_lock<mutex>UL1(MU);
        cout << "Do you need to work with the main objects' store or with its' substore?";
        }


        cin >> Answer1;

        if ((chrono::system_clock::now()) > Tp_Try) {
            this_thread::sleep_until(Tp_Next);
            unique_lock<mutex>UL1(MU);
            CV.wait(UL1, is_printed);

        }

        {unique_lock<mutex>UL1(MU);
        cout << "Enter the name";
        }
        cin >> Object;

        if ((chrono::system_clock::now()) > Tp_Try) {
            this_thread::sleep_until(Tp_Next);
            unique_lock<mutex>UL1(MU);
            CV.wait(UL1, is_printed);

        }

        {unique_lock<mutex>UL1(MU);
        cout << "What do you need to do ? Enter get, update, show, query, range_query, remove";
        }

        cin >> Answer;

        (Answer1.find("main") != -1) ? Bl = true : Bl = !Bl;

        shared_ptr<Store >Obj;
        shared_ptr<Store::Child> Obj1;


        map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >::iterator it;
        if (Bl == true) {

            for (it = Children.begin(); it != Children.end(); it++) {

                if (it->first->Name == Object) {

                    Obj = (it->first);

                    break;
                }
            }
        }
        else {

            for (it = Children.begin(); it != Children.end(); it++) {

                for (auto it2 : it->second) {

                    if (it2->Name == Object) {

                        Obj1 = it2;

                        break;
                    }
                }
            }
        }



        string key, value;
        double a, b;
        int64_t ID = 0;

        if (Answer == "get") {

            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
                unique_lock<mutex>UL1(MU);
                CV.wait(UL1, is_printed);

            }

            {unique_lock<mutex>UL1(MU);
            cout << "Enter the ID";
            }

            cin >> ID;

            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
                unique_lock<mutex>UL1(MU);
                CV.wait(UL1, is_printed);

            }

            {unique_lock<mutex>UL1(MU);
            (Bl == true) ? Obj->Get(Obj->mp, ID) : Obj1->Get(ID);
            }

        }

        else if (Answer == "update") {
            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
            }

            {unique_lock<mutex>UL1(MU);
            cout << "Enter the ID, key, value";
            }

            cin >> ID >> key >> value;

            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
            }

            {unique_lock<mutex>UL1(MU);
            (Bl == true) ? Obj->Update(ID, key, value) : Obj1->Update(ID, key, value);
            }

        }

        else if (Answer == "show") {
            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
                unique_lock<mutex>UL1(MU);
                CV.wait(UL1, is_printed);

            }

            {unique_lock<mutex>UL1(MU);
            (Bl == true) ? Obj->Show(Obj->mp) : Obj1->Show();
            }

        }

        else if (Answer == "query") {
            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
                unique_lock<mutex>UL1(MU);
                CV.wait(UL1, is_printed);

            }

            {unique_lock<mutex>UL1(MU);
            cout << "Enter key and value";
            }

            cin >> key >> value;

            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
                unique_lock<mutex>UL1(MU);
                CV.wait(UL1, is_printed);

            }

            {unique_lock<mutex>UL1(MU);
            (Bl == true) ? Obj->Query(Obj->mp, key, value) : Obj1->Query(key, value);
            }

        }


        else if (Answer == "range_query") {
            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
            }

            {unique_lock<mutex>UL1(MU);
            cout << "Enter key and values";
            }

            cin >> key >> a >> b;
            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
            }

            {unique_lock<mutex>UL1(MU);
            (Bl == true) ? Obj->Range_query(Obj->mp, key, a, b) : Obj1->Range_query(key, a, b);
            }

        }

        else if (Answer == "remove") {
            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
            }

            {unique_lock<mutex>UL1(MU);
            cout << "Enter the ID";
            }

            cin >> ID;

            if ((chrono::system_clock::now()) > Tp_Try) {
                this_thread::sleep_until(Tp_Next);
            }

            {unique_lock<mutex>UL1(MU);
            Obj->Remove(ID);
            }

        }


    }

}


bool operator < (shared_ptr <Store> const& obj1, shared_ptr <Store> const& obj2) {
    return obj1->Name < obj2->Name;
}

void New_Note(map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >&Children, set<pair<chrono::system_clock::time_point, int64_t> > &TP) {
    cout << "Enter the date like year->month->day(ex.20201029)";
    string Name="", Title="", Description="", Priority="", Group="", Child="";
    double Time_Stamp = 0;
    cin >> Name;
    cout << "Enter the Title, Description, Priority, Group name and the timestamp";
    cin >> Title >> Description >> Priority >> Group >> Time_Stamp;
    
    
    shared_ptr <Store>Obj = { 0 }; 
    try {
        Obj= make_shared <Store>(Name);
        Obj->Insertion(make_unique<Todo>(Date(TP), Title, Description, Time_Stamp, Priority, Group));
        Obj->Insertion(make_unique<Todo>(Date(TP), "Watch on weekends", "Terminator", 5.6, "low", "Plans"));
        Obj->Insertion(make_unique<Todo>(Date(TP), "Buy", "Toys", 5.0, "high", "House"));
        
    }
    catch (bad_alloc&) {
        cerr << "There are no enought memory!";
        throw bad_alloc();
    }
    cout << "Enter the child store name";
    cin >> Child;
    shared_ptr<Store::Child >X = { 0 };
    shared_ptr<Store::Child >Y = { 0 };
    try {
        X = make_shared <Store::Child>(Obj, Child);  //creating child store
        Y = make_shared <Store::Child>(Obj, "Y");
    }
    catch (bad_alloc&) {
        cerr << "There are no enought memory!";
        throw bad_alloc();
    }
    vector<shared_ptr<Store::Child > > Vobj;
    Vobj.push_back(X);
    Vobj.push_back(Y);
    pair< shared_ptr <Store>, vector<shared_ptr<Store::Child > > > pr{ Obj, Vobj };
    Vobj.clear();

    Children.insert(pr);  //adding the pointer to the set of the pointers to the child stores
   
    pr.first = NULL;
    pr.second.clear();
    Vobj.clear();
   
    shared_ptr<Store::Child >Z{ new Store::Child(Obj,"Z") };  // creating new child store
    Children.find(Obj)->second.push_back(Z);

}

void Delete_Objects(map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > > &Children) {
    cout << "How many objects would you like to delete?";
    int number = 0;
    cin >> number;
    string _Name;
    map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >::iterator it;
    for (int i = 0; i < number; i++) {
        cin >> _Name;
        for (it = Children.begin(); it != Children.end(); it++) {
            if (it->first->Name == _Name)
            {
                Children.erase(it->first);
            }
        }
    }
}


int main()
{

    map<shared_ptr <Store>, vector<shared_ptr<Store::Child > > >Children;  //set to keep the pointers to all the child store together
    set<pair<chrono::system_clock::time_point, int64_t> > TP;
    
   
    
    try {
        New_Note(Children, TP);
    }
    catch (bad_alloc&) {
        cerr << "Please delete old objects to free the memory. Would you like to delete old objects? ";
        string Answer = "";
        cin >> Answer;
        (Answer=="Yes"? Delete_Objects(Children): exit(0));
    }
    

  /*  unique_ptr<Todo>A{ new Todo{Date(TP),"Read this month ","Lord of rings",5.7,"high","School"} };
      unique_ptr<Todo>B{ new Todo{Date(TP), "Watch on weekends", "Terminator", 5.6,"low","Plans"} };
      unique_ptr<Todo>C{ new Todo{Date(TP), "go to the Theater", "Romeo and Juliet", 5.5, "middle","Plans"} };
      unique_ptr<Todo>D{ new Todo{Date(TP), "Visit", "Bank", 5.0,"high","Plans"} };
      unique_ptr<Todo>E{ new Todo{Date(TP), "Buy", "Toys", 5.0,"high","House"} };
      unique_ptr<Todo>F{ new Todo{Date(TP), "Buy", "Clothes", 5.0,"high","House"} };
      unique_ptr<Todo>G{ new Todo{Date(TP), "Rent", "Car", 5.0,"high","House"} };*/

    set<pair<chrono::system_clock::time_point, int64_t> >::iterator itchrono;
    for (itchrono = TP.begin(); itchrono != TP.end(); itchrono++) {
        time_t tt = chrono::system_clock::to_time_t(itchrono->first);
        tm t1;
        localtime_s(&t1, &tt);
        cout << t1 << endl;
    }





  /* shared_ptr <Store> Obj{ new Store("20201019") };   //creating the main store
    Obj->Insertion(move(A));
    Obj->Insertion(move(B));
    /*Obj->Insertion(move(C));
    Obj->Insertion(move(D));
    Obj->Insertion(move(E));
    Obj->Insertion(move(F));
    Obj->Insertion(move(G));
    /* Obj->Update(A->id, "description", "Lord of rings-2");*/

    


     
     

/*
    shared_ptr<Store::Child >X{ new Store::Child(Obj,"X") };  //creating child store
    shared_ptr<Store::Child >Y{ new Store::Child(Obj,"Y") };

    vector<shared_ptr<Store::Child > > Vobj;
    
    Vobj.push_back(X);
    Vobj.push_back(Y);
    pair< shared_ptr <Store>, vector<shared_ptr<Store::Child > > > pr{Obj, Vobj };
 

    Children.insert(pr);  //adding the pointer to the set of the pointers to the child stores
    pr.first = NULL;
    pr.second.clear();
    Vobj.clear();
   
    Obj->Show(Obj->mp);
   
    

    shared_ptr<Store::Child >Z{ new Store::Child(Obj,"Z") };  // creating new child store
    Children.find(Obj)->second.push_back(Z);

   /* Obj->Commit(move(X), Children);*/

   
   /* string Tit{ "Description" }, Des{ "Movie" };

    X->Update(B->id, "description", "Lord of rings-7");
    cout << "qqw";

   X->Query(Tit, Des);
    cout << endl << "Child X:" << endl;

    X->Show();
    X->Get(1);


    shared_ptr<Store::Child >Y{ new Store::Child(Obj,"Y") };  //creating new child store
    Children.find(Obj)->second.push_back(Y);
     Y->Update(B->id, "description", "Lord of rings-9");

    shared_ptr<Store::Child >Z{ new Store::Child(Obj,"Z") };  // creating new child store
    Children.find(Obj)->second.push_back(Z);

    cout << "OOO";

    Obj->Commit(move(X), Children);  //commiting the changes from child store to the main store and deleting X

    cout << endl << "Main store:" << endl;
    Obj->Show(Obj->mp);

    cout << endl << "Child Y:" << endl;
    Y->Show();
    cout << endl << "Child Z:" << endl;
    Z->Show();
   */

    
    thread t1(f, ref(TP), ref(Children));
    thread t2(User_Request, ref(Children));


    if (t1.joinable()) {
        t1.join();
    }
    if (t2.joinable()) {
        t2.join();
    }
    
   






    return 0;
}

//Comments:
//1) When there are several child stores, if we commit the changes from one of them into the main store, other child stores will not contain this changes untill we sync them all with
// the main store. I sync them stright in the commit function. For this i need to insert the pointers to the child store into one common set, so it will be easy to make changes to all 
// of the child store at the same time. With thread it will be faster then with a special function and also thread can be modyfied.

//2) Mooving the shared_ptr to the Commit function makes it to be delated after the function finish it work. And i delete this pointer, which we dont' need any more, from the set.
//3) To improve the performance of the `range_query` implementation including the child stores, we can search separetly in the main store and in the child store, which can be
// acessed all together in the set Children. All the search result can be saved in the multiset.
// 4) Please note this this code doesnt' deal with the exeptions right now, it still needs to be improved
