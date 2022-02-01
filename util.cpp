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

#endif
