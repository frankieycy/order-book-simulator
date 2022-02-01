#ifndef UTIL_CPP
#define UTIL_CPP
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <string>
#include <vector>
using namespace std;

inline void seperator(int length=20){cout << string(length,'-') << endl;}
inline double uniformRand(double min=0, double max=1){return min+(max-min)*rand()/RAND_MAX;}

template <class T>
ostream& operator<<(ostream& out, const vector<T>& vec){
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

#endif
