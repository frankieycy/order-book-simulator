#ifndef ZEROINTELLIGENCE_HPP
#define ZEROINTELLIGENCE_HPP
#include <vector>
#include <deque>
#include <map>
#include "side.hpp"
#include "orderType.hpp"
#include "orderBook.hpp"
using namespace std;

/**** class declarations ******************************************************/

class ZeroIntelligence {
private:
    int id;
    int time;
    int numOrder, numOrderSent;
    int priceBnd, limPriceBnd;
    int snapInterval, snapBookLevels;
    double mktOrderArvRate;
    double limOrderArvRate;
    double cclOrderArvRate;
    LimitOrderBook ob;
    map<int,map<double,int>> bidDepthsLog, askDepthsLog;
public:
    /**** constructors ****/
    ZeroIntelligence(); virtual ~ZeroIntelligence(){};
    ZeroIntelligence(int numOrder, int priceBnd, int limPriceBnd,
        double limOrderArvRate, double mktOrderArvRate, double cclOrderArvRate,
        int snapInterval=1e3, int snapBookLevels=50);
    ZeroIntelligence(const ZeroIntelligence& zi);
    ZeroIntelligence* copy() const;
    /**** accessors ****/
    int getId() const {return id;}
    int getTime() const {return time;}
    int getNumOrder() const {return numOrder;}
    int getNumOrderSent() const {return numOrderSent;}
    int getPriceBnd() const {return priceBnd;}
    int getLimPriceBnd() const {return limPriceBnd;}
    int getSnapInterval() const {return snapInterval;}
    int getSnapBookLevels() const {return snapBookLevels;}
    double getMktOrderArvRate() const {return mktOrderArvRate;}
    double getLimOrderArvRate() const {return limOrderArvRate;}
    double getCclOrderArvRate() const {return cclOrderArvRate;}
    deque<Trade*> getTrades() const {return ob.getTrades();}
    deque<Trade*>* getTradesPtr() {return ob.getTradesPtr();}
    map<int,Order*> getOrdersLog() const {return ob.getOrdersLog();}
    map<int,Order*>* getOrdersLogPtr() {return ob.getOrdersLogPtr();}
    map<double,int> getBidDepths() const {return ob.getBidDepths();}
    map<double,int> getAskDepths() const {return ob.getAskDepths();}
    map<double,int>* getBidDepthsPtr() {return ob.getBidDepthsPtr();}
    map<double,int>* getAskDepthsPtr() {return ob.getAskDepthsPtr();}
    map<double,deque<LimitOrder*>>* getBidsPtr() {return ob.getBidsPtr();}
    map<double,deque<LimitOrder*>>* getAsksPtr() {return ob.getAsksPtr();}
    map<int,map<double,int>> getBidDepthsLog() const {return bidDepthsLog;}
    map<int,map<double,int>> getAskDepthsLog() const {return askDepthsLog;}
    map<int,map<double,int>>* getBidDepthsLogPtr() {return &bidDepthsLog;}
    map<int,map<double,int>>* getAskDepthsLogPtr() {return &askDepthsLog;}
    LimitOrderBook* getLimitOrderBookPtr() {return &ob;}
    /**** mutators ****/
    int setNumOrder(int numOrder);
    int setPriceBnd(int priceBnd);
    int setLimPriceBnd(int limPriceBnd);
    int setSnapInterval(int snapInterval);
    int setSnapBookLevels(int snapBookLevels);
    double setMktOrderArvRate(double arvRate);
    double setLimOrderArvRate(double arvRate);
    double setCclOrderArvRate(double arvRate);
    /**** main ****/
    virtual void initOrderBook(vector<int> sizes={});
    virtual void sendLimitOrder(Side side);
    virtual void sendMarketOrder(Side side);
    virtual void sendCancelOrder(Side side, int depthBtw=0);
    virtual void generateOrder();
    void simulate();
    void snapBook();
    void printBook(int bookLevels=0, int tradeLevels=0,
        bool summarizeDepth=true) const;
    void printTradesToJson(string filename);
    void printDepthsLogToJson(string filename);
    void printTradesToCsv(string filename);
    void printDepthsLogToCsv(string filename);
};

#endif
