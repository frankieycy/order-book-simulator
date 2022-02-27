#ifndef ORDERBOOKSTATS_CPP
#define ORDERBOOKSTATS_CPP
#include <vector>
#include <deque>
#include <map>
#include "orderBook.hpp"
#include "orderBookStats.hpp"
using namespace std;

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
        imbalances[t] = (Sb-Sa)/(Sa+Sb);
        spreads[t] = A-B;
        // TO-DO: bidCumDepthsLog, askCumDepthsLog
    }
}

void OrderBookStats::clearStats() {
    for (auto t : trades) delete t;
    depthsLogTime.clear();
    trades.clear();
    topBidSizes.clear();
    topAskSizes.clear();
    topBids.clear();
    topAsks.clear();
    midPrices.clear();
    microPrices.clear();
    imbalances.clear();
    spreads.clear();
    bidDepthsLog.clear();
    askDepthsLog.clear();
    bidCumDepthsLog.clear();
    askCumDepthsLog.clear();
}

map<double,double> OrderBookStats::calcAvgBookDepths(vector<double> band, int aggInterval) {
    double n = depthsLogTime.size();
    map<double,double> avgBookDepths;
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
