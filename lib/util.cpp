#ifndef UTIL_CPP
#define UTIL_CPP
#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include <vector>
#include <deque>
#include <map>
using namespace std;

inline void seperator(int length=20){cout << string(length,'-') << endl;}
inline double uniformRand(double min=0, double max=1){return min+(max-min)*rand()/RAND_MAX;}

template <class S, class T>
S& operator<<(S& out, const vector<T>& vec){
    // print elements of a vector
    if(vec.size()==0) out << "[]";
    else{
        out << "[";
        for(auto p=vec.begin(); p!=vec.end(); p++)
            if(is_same<T,string>::value) out << "\"" << *p << "\"" << ((p==vec.end()-1)?"]":",");
            else out << *p << ((p==vec.end()-1)?"]":",");
    }
    return out;
}

template <class S, class T>
S& operator<<(S& out, const deque<T>& deq){
    // print elements of a deque
    if(deq.size()==0) out << "[]";
    else{
        out << "[";
        for(auto p=deq.begin(); p!=deq.end(); p++)
            if(is_same<T,string>::value) out << "\"" << *p << "\"" << ((p==deq.end()-1)?"]":",");
            else out << *p << ((p==deq.end()-1)?"]":",");
    }
    return out;
}

#endif
