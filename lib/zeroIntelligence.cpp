#ifndef ZEROINTELLIGENCE_CPP
#define ZEROINTELLIGENCE_CPP
#include <fstream>
#include <numeric>
#include <vector>
#include <deque>
#include <map>
#include "util.cpp"
#include "side.hpp"
#include "orderType.hpp"
#include "orderBook.hpp"
#include "zeroIntelligence.hpp"
using namespace std;

/**** class functions *********************************************************/
//### ZeroIntelligence class ###################################################

ZeroIntelligence::ZeroIntelligence(): id(0), time(0), numOrder(0), numOrderSent(0), priceBnd(0), limPriceBnd(0), snapInterval(1e3), snapBookLevels(50), mktOrderArvRate(0), limOrderArvRate(0), cclOrderArvRate(0) {}

ZeroIntelligence::ZeroIntelligence(int numOrder, int priceBnd, int limPriceBnd, double limOrderArvRate, double mktOrderArvRate, double cclOrderArvRate, int snapInterval, int snapBookLevels): id(0), time(0), numOrder(numOrder), numOrderSent(0), priceBnd(priceBnd), limPriceBnd(limPriceBnd), snapInterval(snapInterval), snapBookLevels(snapBookLevels), limOrderArvRate(limOrderArvRate), mktOrderArvRate(mktOrderArvRate), cclOrderArvRate(cclOrderArvRate) {}

ZeroIntelligence::ZeroIntelligence(const ZeroIntelligence& zi): id(zi.id), time(zi.time), numOrder(zi.numOrder), numOrderSent(zi.numOrderSent), priceBnd(zi.priceBnd), limPriceBnd(zi.limPriceBnd), snapInterval(zi.snapInterval), snapBookLevels(zi.snapBookLevels), limOrderArvRate(zi.limOrderArvRate), mktOrderArvRate(zi.mktOrderArvRate), cclOrderArvRate(zi.cclOrderArvRate), ob(zi.ob) {}

ZeroIntelligence* ZeroIntelligence::copy() const {
    return new ZeroIntelligence(*this);
}

int ZeroIntelligence::setNumOrder(int numOrder) {
    this->numOrder = numOrder;
    return this->numOrder;
}

int ZeroIntelligence::setPriceBnd(int priceBnd) {
    this->priceBnd = priceBnd;
    return this->priceBnd;
}

int ZeroIntelligence::setLimPriceBnd(int limPriceBnd) {
    this->limPriceBnd = limPriceBnd;
    return this->limPriceBnd;
}

int ZeroIntelligence::setSnapInterval(int snapInterval) {
    this->snapInterval = snapInterval;
    return this->snapInterval;
}

int ZeroIntelligence::setSnapBookLevels(int snapBookLevels) {
    this->snapBookLevels = snapBookLevels;
    return this->snapBookLevels;
}

double ZeroIntelligence::setMktOrderArvRate(double arvRate) {
    this->mktOrderArvRate = arvRate;
    return this->mktOrderArvRate;
}

double ZeroIntelligence::setLimOrderArvRate(double arvRate) {
    this->limOrderArvRate = arvRate;
    return this->limOrderArvRate;
}

double ZeroIntelligence::setCclOrderArvRate(double arvRate) {
    this->cclOrderArvRate = arvRate;
    return this->cclOrderArvRate;
}

void ZeroIntelligence::initOrderBook(vector<int> sizes) {
    setTradesClock(0);
    if (!sizes.size()) sizes = {1,2,2,3,3,4,4,5};
    int limit = 1, idx = 0;
    while (limit <= priceBnd) {
        for (int i=0; i<sizes[idx]; i++) {
            ob.process(LimitOrder(id++,0,"ZI",BID,1,-limit));
            ob.process(LimitOrder(id++,0,"ZI",ASK,1,+limit));
        }
        idx = min(idx+1,(int)sizes.size()-1);
        limit++;
    }
    snapBook();
}

void ZeroIntelligence::sendLimitOrder(Side side) {
    int limit;
    int L = limPriceBnd;
    if (side == BID) {
        int a = ob.getTopAsk();
        limit = uniformIntRand(a-L,a-1);
    } else {
        int b = ob.getTopBid();
        limit = uniformIntRand(b+1,b+L);
    }
    ob.process(LimitOrder(id++,time++,"ZI",side,1,limit));
}

void ZeroIntelligence::sendMarketOrder(Side side) {
    ob.process(MarketOrder(id++,time++,"ZI",side,1));
}

void ZeroIntelligence::sendCancelOrder(Side side, int depthBtw) {
    int idRef = -1;
    int cumDepth = 0;
    int L = limPriceBnd;
    double limit;
    if (side == BID) {
        int a = ob.getTopAsk();
        int threshold = uniformIntRand(1,(depthBtw)?depthBtw:ob.getBidDepthBetween(a-L,a-1));
        deque<double>* bidPrices = ob.getBidPricesPtr();
        map<double,int>* bidDepths = ob.getBidDepthsPtr();
        for (auto p : *bidPrices) {
            cumDepth += bidDepths->at(p);
            if (cumDepth >= threshold) {
                limit = p; break;
            }
        }
        idRef = ob.peekBidOrderAt(limit)->getId();
    } else if (side == ASK) {
        int b = ob.getTopBid();
        int threshold = uniformIntRand(1,(depthBtw)?depthBtw:ob.getAskDepthBetween(b+1,b+L));
        deque<double>* askPrices = ob.getAskPricesPtr();
        map<double,int>* askDepths = ob.getAskDepthsPtr();
        for (auto p : *askPrices) {
            cumDepth += askDepths->at(p);
            if (cumDepth >= threshold) {
                limit = p; break;
            }
        }
        idRef = ob.peekAskOrderAt(limit)->getId();
    }
    ob.process(CancelOrder(id++,time++,"ZI",idRef));
}

void ZeroIntelligence::generateOrder() {
    int event = 0;
    int a = ob.getTopAsk();
    int b = ob.getTopBid();
    int L = limPriceBnd;
    int bidDepthBtw = ob.getBidDepthBetween(a-L,a-1);
    int askDepthBtw = ob.getAskDepthBetween(b+1,b+L);
    double p = uniformRand();
    vector<double> prob{
        limPriceBnd*limOrderArvRate,
        limPriceBnd*limOrderArvRate,
        mktOrderArvRate/2,
        mktOrderArvRate/2,
        bidDepthBtw*cclOrderArvRate,
        askDepthBtw*cclOrderArvRate,
    };
    double totalProb = accumulate(prob.begin(), prob.end(), 0);
    transform(prob.begin(), prob.end(), prob.begin(), [totalProb](double& p){return p/totalProb;});
    while (p > prob[event]) {
        p -= prob[event];
        event++;
    }
    switch(event) {
        case 0: sendLimitOrder(BID); break;
        case 1: sendLimitOrder(ASK); break;
        case 2: sendMarketOrder(BID); break;
        case 3: sendMarketOrder(ASK); break;
        case 4: sendCancelOrder(BID,bidDepthBtw); break;
        case 5: sendCancelOrder(ASK,askDepthBtw); break;
        default: return;
    }
}

void ZeroIntelligence::simulate() {
    while (numOrderSent < numOrder) {
        generateOrder();
        setTradesClock(time);
        numOrderSent++;
        snapBook();
    }
}

void ZeroIntelligence::snapBook() {
    if (time % snapInterval == 0) {
        bidDepthsLog[time] = ob.snapBidDepths(snapBookLevels);
        askDepthsLog[time] = ob.snapAskDepths(snapBookLevels);
    }
}

void ZeroIntelligence::printBook(int bookLevels, int tradeLevels, bool summarizeDepth) const {
    ob.printBook(bookLevels, tradeLevels, summarizeDepth);
}

void ZeroIntelligence::printTradesToJson(string filename) {
    ofstream f; f.open(filename);
    f << *ob.getTradesPtr() << endl;
    f.close();
}

void ZeroIntelligence::printDepthsLogToJson(string filename) {
    ofstream f; f.open(filename);
    f << "{\"BID\":" << bidDepthsLog << ",\"ASK\":" << askDepthsLog << "}" << endl;
    f.close();
}

void ZeroIntelligence::printTradesToCsv(string filename) {
    deque<Trade*> trades = *ob.getTradesPtr();
    ofstream f; f.open(filename);
    f << "TIME,ID,SIZE,PRICE,DIRECTION" << endl;
    for (auto t : trades) {
        int time      = t->getTime();
        int id        = t->getId(); // bookOrder (LIM)
        int size      = t->getSize();
        int price     = t->getPrice();
        int direction = (t->getSide()==BID)?1:-1; // matchOrder (MKT)
        f << time << "," << id << "," << size << "," << price << "," << direction << endl;
    }
    f.close();
}

void ZeroIntelligence::printDepthsLogToCsv(string filename) {
    ofstream f; f.open(filename);
    f << "TIME";
    for (int i=1; i<=snapBookLevels; i++) f << ",BID" << i << "_PRICE" << ",BID" << i << "_SIZE";
    for (int i=1; i<=snapBookLevels; i++) f << ",ASK" << i << "_PRICE" << ",ASK" << i << "_SIZE";
    f << endl;
    for (int t=0; t<=time; t+=snapInterval) {
        map<double,int> b = bidDepthsLog.at(t);
        map<double,int> a = askDepthsLog.at(t);
        f << t;
        for (auto i=b.rbegin(); i!=b.rend(); i++) f << "," << i->first << "," << i->second;
        for (auto i=a.begin(); i!=a.end(); i++) f << "," << i->first << "," << i->second;
        f << endl;
    }
    f.close();
}

#endif
