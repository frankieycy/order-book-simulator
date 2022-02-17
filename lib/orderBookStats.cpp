#ifndef ORDERBOOKSTATS_CPP
#define ORDERBOOKSTATS_CPP
#include "orderBook.cpp"
using namespace std;

/**** class declarations ******************************************************/

class OrderBookStats {
private:
    vector<int> depthsLogTime;
    deque<Trade*> trades;
    map<int,int> topBidSizes, topAskSizes;
    map<int,double> topBids, topAsks, midPrices, microPrices, imbalance;
    map<int,map<double,int>> bidDepthsLog, askDepthsLog;
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
    map<int,map<double,int>> getBidDepthsLog() const {return bidDepthsLog;}
    map<int,map<double,int>> getAskDepthsLog() const {return askDepthsLog;}
    map<int,map<double,int>>* getBidDepthsLogPtr() {return &bidDepthsLog;}
    map<int,map<double,int>>* getAskDepthsLogPtr() {return &askDepthsLog;}
    /**** main ****/
    void initStats();
    map<double,double> calcAvgBookDepths(int band=0);
    map<int,double> calcBidPriceSignal(int maxSize=0);
    map<int,double> calcAskPriceSignal(int maxSize=0);
    map<double,double> calcImbalPriceSignal();
};

/**** class functions *********************************************************/
//### OrderBookStats class ###################################################

OrderBookStats::OrderBookStats(const map<int,map<double,int>>& bidDepthsLog, const map<int,map<double,int>>& askDepthsLog, const deque<Trade*>& trades): bidDepthsLog(bidDepthsLog), askDepthsLog(askDepthsLog) {
    for (auto t : trades) this->trades.push_back(t->copy());
}

OrderBookStats::OrderBookStats(const OrderBookStats& obs): bidDepthsLog(obs.bidDepthsLog), askDepthsLog(obs.askDepthsLog) {
    for (auto t : obs.trades) this->trades.push_back(t->copy());
}

OrderBookStats::OrderBookStats(string depthsFile, string tradesFile) {
    //
}

OrderBookStats::~OrderBookStats() {
    for (auto t : trades) delete t;
}

void OrderBookStats::initStats() {
    //
}

map<double,double> OrderBookStats::calcAvgBookDepths(int band) {
    //
    return {};
}

#endif
