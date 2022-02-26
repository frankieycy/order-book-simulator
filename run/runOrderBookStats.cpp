#include <iostream>
#include "util.cpp"
#include "zeroIntelligence.hpp"
#include "orderBookStats.hpp"
using namespace std;

int main() {
    srand(0);
    /**** parameters **********************************************************/
    int n       = 1e4;
    int L       = 30;
    int LL      = 1000;
    int snpInt  = 1;
    int snpLvl  = 40;
    double lda  = 1;
    double mu   = 50;
    double nu   = 0.2;
    /**** ZI simulation *******************************************************/
    ZeroIntelligence zi(n,LL,L,lda,mu,nu,snpInt,snpLvl);
    zi.initOrderBook();
    zi.simulate();
    zi.printBook(30,10);
    /**** OB stats ************************************************************/
    vector<double> band; for (int b=-20; b<=20; b++) band.push_back(b);
    map<int,map<double,int>>* depthsB = zi.getBidDepthsLogPtr();
    map<int,map<double,int>>* depthsA = zi.getAskDepthsLogPtr();
    deque<Trade*>* trades = zi.getTradesPtr();
    OrderBookStats obs(*depthsB,*depthsA,*trades);
    obs.initStats();
    cout << obs.calcAvgBookDepths(band) << endl;
    return 0;
}
