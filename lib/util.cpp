#ifndef UTIL_CPP
#define UTIL_CPP
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <deque>
#include <map>
using namespace std;

inline void seperator(int length=20){cout << string(length,'-') << endl;}
inline double uniformRand(double min=0, double max=1){return min+(max-min)*rand()/RAND_MAX;}
inline int uniformIntRand(double min, double max){return floor(uniformRand(min,max+1));}
inline double exponentialRand(double lambda){return -log(uniformRand())/lambda;} // lambda: intensity
inline double normalRand(double mu=0, double sig=1){return mu+sig*sqrt(-2*log(uniformRand()))*cos(2*M_PI*uniformRand());}
inline double normalPDF(double x, double mu=0, double sig=1){return exp(-(x-mu)*(x-mu)/(2*sig*sig))/(sqrt(2*M_PI)*sig);}
inline double normalCDF(double x, double mu=0, double sig=1){return erfc(-M_SQRT1_2*x)/2;}
inline double stdNormalPDF(double x){return normalPDF(x);}
inline double stdNormalCDF(double x){return normalCDF(x);}

template <typename T>
void printToCsv(const vector<T>& v, string filename) {
    ofstream f; f.open(filename);
    for(auto p=v.begin(); p!=v.end(); p++) {
        f << ((p==v.begin())?"":",");
        if(is_same<T,string>::value) f << "\"" << *p << "\"";
        else f << *p;
    }
    f.close();
}

template <typename T1, typename T2>
void printToCsv(const map<T1,T2>& m, string filename) {
    ofstream f; f.open(filename);
    for(auto p=m.begin(); p!=m.end(); p++) {
        f << ((p==m.begin())?"":"\n");
        f << "\"" << p->first << "\"";
        f << ",";
        if(is_same<T2,string>::value) f << "\"" << p->second << "\"";
        else f << p->second;
    }
    f.close();
}

template <typename S, typename T>
S& operator<<(S& out, const vector<T>& v){
    // print elements of a vector
    if(v.size()==0) out << "[]";
    else{
        out << "[";
        for(auto p=v.begin(); p!=v.end(); p++) {
            out << ((p==v.begin())?"":",");
            if(is_same<T,string>::value) out << "\"" << *p << "\"";
            else out << *p;
        }
        out << "]";
    }
    return out;
}

template <typename S, typename T>
S& operator<<(S& out, const deque<T>& d){
    // print elements of a deque
    if(d.size()==0) out << "[]";
    else{
        out << "[";
        for(auto p=d.begin(); p!=d.end(); p++) {
            out << ((p==d.begin())?"":",");
            if(is_same<T,string>::value) out << "\"" << *p << "\"";
            else out << *p;
        }
        out << "]";
    }
    return out;
}

template <typename S, typename T1, typename T2>
S& operator<<(S& out, const map<T1,T2>& m){
    // print elements of a map
    if(m.size()==0) out << "{}";
    else{
        out << "{";
        for(auto p=m.begin(); p!=m.end(); p++) {
            out << ((p==m.begin())?"":",");
            out << "\"" << p->first << "\"";
            out << ":";
            if(is_same<T2,string>::value) out << "\"" << p->second << "\"";
            else out << p->second;
        }
        out << "}";
    }
    return out;
}

#endif
