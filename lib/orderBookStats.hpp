#ifndef ORDERBOOKSTATS_HPP
#define ORDERBOOKSTATS_HPP
#include <vector>
#include <deque>
#include <map>
#include "orderBook.hpp"
using namespace std;

/**** class declarations ******************************************************/

class OrderBookStats {
private:
    vector<int> depthsLogTime;
    deque<Trade*> trades;
    map<int,int> topBidSizes, topAskSizes;
    map<int,double> topBids, topAsks, midPrices, microPrices, imbalances, spreads;
    map<int,map<double,int>> bidDepthsLog, askDepthsLog;
    map<int,map<double,int>> bidCumDepthsLog, askCumDepthsLog;
public:
    /**** constructors ****/
    OrderBookStats(){}; ~OrderBookStats();
    OrderBookStats(const map<int,map<double,int>>& bidDepthsLog,
                   const map<int,map<double,int>>& askDepthsLog,
                   const deque<Trade*>& trades={});
    OrderBookStats(const OrderBookStats& obs);
    OrderBookStats(string depthsFile, string tradesFile="");
    /**** accessors ****/
    deque<Trade*> getTrades() const {return trades;}
    deque<Trade*>* getTradesPtr() {return &trades;}
    map<int,int>* getTopBidSizesPtr() {return &topBidSizes;}
    map<int,int>* getTopAskSizesPtr() {return &topAskSizes;}
    map<int,double>* getTopBidsPtr() {return &topBids;}
    map<int,double>* getTopAsksPtr() {return &topAsks;}
    map<int,double>* getMidPricesPtr() {return &midPrices;}
    map<int,double>* getMicroPricesPtr() {return &microPrices;}
    map<int,double>* getImbalancesPtr() {return &imbalances;}
    map<int,double>* getSpreadsPtr() {return &spreads;}
    map<int,map<double,int>> getBidDepthsLog() const {return bidDepthsLog;}
    map<int,map<double,int>> getAskDepthsLog() const {return askDepthsLog;}
    map<int,map<double,int>>* getBidDepthsLogPtr() {return &bidDepthsLog;}
    map<int,map<double,int>>* getAskDepthsLogPtr() {return &askDepthsLog;}
    /**** main ****/
    void initStats();
    void clearStats();
    map<double,double> calcAvgBookDepths(vector<double> band, int aggInterval=1);
    map<int,double> calcBidPriceSignal(int maxSize=0);
    map<int,double> calcAskPriceSignal(int maxSize=0);
    map<double,double> calcImbalPriceSignal();
};

#endif
