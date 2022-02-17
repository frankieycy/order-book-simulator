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
    map<int,map<double,int>> bidCumDepthsLog, askCumDepthsLog;
    /**** stats log ****/
    map<double,double> avgBookDepths;
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
    map<int,double>* getImbalancePtr() {return &imbalance;}
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

/**** class functions *********************************************************/
//### OrderBookStats class ###################################################

OrderBookStats::OrderBookStats(const map<int,map<double,int>>& bidDepthsLog, const map<int,map<double,int>>& askDepthsLog, const deque<Trade*>& trades): bidDepthsLog(bidDepthsLog), askDepthsLog(askDepthsLog) {
    for (auto t : trades) this->trades.push_back(t->copy());
}

OrderBookStats::OrderBookStats(const OrderBookStats& obs): bidDepthsLog(obs.bidDepthsLog), askDepthsLog(obs.askDepthsLog) {
    for (auto t : obs.trades) this->trades.push_back(t->copy());
}

OrderBookStats::OrderBookStats(string depthsFile, string tradesFile) {
    // TO-DO
}

OrderBookStats::~OrderBookStats() {
    for (auto t : trades) delete t;
}

void OrderBookStats::initStats() {
    vector<int> timeB, timeA;
    for (auto b : bidDepthsLog) timeB.push_back(b.first);
    for (auto a : askDepthsLog) timeA.push_back(a.first);
    assert(timeB == timeA);
    depthsLogTime = timeB;
    for (auto t : depthsLogTime) {
        bidCumDepthsLog[t] = askCumDepthsLog[t] = {};
        map<double,int>* bidDepths = &bidDepthsLog.at(t);
        map<double,int>* askDepths = &askDepthsLog.at(t);
        double B = bidDepths->rbegin()->first, A = askDepths->begin()->first;
        int Sb = bidDepths->rbegin()->second, Sa = askDepths->begin()->second;
        topBids[t] = B;
        topAsks[t] = A;
        topBidSizes[t] = Sb;
        topBidSizes[t] = Sa;
        midPrices[t] = (A+B)/2;
        microPrices[t] = (A*Sb+B*Sa)/(Sa+Sb);
        imbalance[t] = (Sb-Sa)/(Sa+Sb);
        // TO-DO: bidCumDepthsLog, askCumDepthsLog
    }
}

map<double,double> OrderBookStats::calcAvgBookDepths(vector<double> band, int aggInterval) {
    double n = depthsLogTime.size();
    for (auto p : band) avgBookDepths[p] = 0;
    for (auto t : depthsLogTime) {
        if (t % aggInterval == 0) {
            int M = midPrices.at(t);
            map<double,int>* bidDepths = &bidDepthsLog.at(t);
            map<double,int>* askDepths = &askDepthsLog.at(t);
            for (auto depths : {bidDepths, askDepths}) {
                for (auto l : *depths) {
                    double P = l.first-M; // relative price
                    if (avgBookDepths.find(P) != avgBookDepths.end()) avgBookDepths[P] += l.second/n*aggInterval;
                }
            }
        }
    }
    return avgBookDepths;
}

#endif
