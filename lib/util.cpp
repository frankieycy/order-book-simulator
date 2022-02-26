#ifndef UTIL_CPP
#define UTIL_CPP
#include <iostream>
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

template <class S, class T>
S& operator<<(S& out, const vector<T>& vec){
    // print elements of a vector
    if(vec.size()==0) out << "[]";
    else{
        out << "[";
        for(auto p=vec.begin(); p!=vec.end(); p++) {
            out << ((p==vec.begin())?"":",");
            if(is_same<T,string>::value) out << "\"" << *p << "\"";
            else out << *p;
        }
        out << "]";
    }
    return out;
}

template <class S, class T>
S& operator<<(S& out, const deque<T>& deq){
    // print elements of a deque
    if(deq.size()==0) out << "[]";
    else{
        out << "[";
        for(auto p=deq.begin(); p!=deq.end(); p++) {
            out << ((p==deq.begin())?"":",");
            if(is_same<T,string>::value) out << "\"" << *p << "\"";
            else out << *p;
        }
        out << "]";
    }
    return out;
}

template <class S, class T1, class T2>
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
