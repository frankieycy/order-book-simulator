#ifndef ZEROINTELLIGENCE_CPP
#define ZEROINTELLIGENCE_CPP
#include "orderBook.cpp"
using namespace std;

class ZeroIntelligence {
private:
    int id;
    int time;
    int numOrder, numOrderSent;
    int priceBnd, limPriceBnd;
    double mktOrderArvRate;
    double limOrderArvRate;
    double cclOrderArvRate;
    LimitOrderBook ob;
public:
    /**** constructors ****/
    ZeroIntelligence(); ~ZeroIntelligence(){};
    ZeroIntelligence(int numOrder, int priceBnd, int limPriceBnd,
        double limOrderArvRate, double mktOrderArvRate, double cclOrderArvRate);
    ZeroIntelligence(const ZeroIntelligence& zi);
    ZeroIntelligence* copy() const;
    /**** accessors ****/
    int getId() const {return id;}
    int getTime() const {return time;}
    int getNumOrder() const {return numOrder;}
    int getNumOrderSent() const {return numOrderSent;}
    int getPriceBnd() const {return priceBnd;}
    int getLimPriceBnd() const {return limPriceBnd;}
    double getMktOrderArvRate() const {return mktOrderArvRate;}
    double getLimOrderArvRate() const {return limOrderArvRate;}
    double getCclOrderArvRate() const {return cclOrderArvRate;}
    LimitOrderBook* getLimitOrderBook() {return &ob;}
    void printBook(int bookLevels=0, int tradeLevels=0, bool summarizeDepth=true) const;
    /**** mutators ****/
    int setNumOrder(int numOrder);
    int setPriceBnd(int priceBnd);
    int setLimPriceBnd(int limPriceBnd);
    double setMktOrderArvRate(double arvRate);
    double setLimOrderArvRate(double arvRate);
    double setCclOrderArvRate(double arvRate);
    /**** main ****/
    void initOrderBook(vector<int> sizes={});
    void sendLimitOrder(Side side);
    void sendMarketOrder(Side side);
    void sendCancelOrder(Side side);
    void generateOrder();
    void simulate();
};

/******************************************************************************/

ZeroIntelligence::ZeroIntelligence(): id(0), time(0), mktOrderArvRate(0), limOrderArvRate(0), cclOrderArvRate(0), numOrder(0), numOrderSent(0), priceBnd(0), limPriceBnd(0) {}

ZeroIntelligence::ZeroIntelligence(int numOrder, int priceBnd, int limPriceBnd, double limOrderArvRate, double mktOrderArvRate, double cclOrderArvRate): id(0), time(0), limOrderArvRate(limOrderArvRate), mktOrderArvRate(mktOrderArvRate), cclOrderArvRate(cclOrderArvRate), numOrder(numOrder), numOrderSent(0), priceBnd(priceBnd), limPriceBnd(limPriceBnd) {}

ZeroIntelligence::ZeroIntelligence(const ZeroIntelligence& zi): id(zi.id), time(zi.time), limOrderArvRate(zi.limOrderArvRate), mktOrderArvRate(zi.mktOrderArvRate), cclOrderArvRate(zi.cclOrderArvRate), numOrder(zi.numOrder), numOrderSent(zi.numOrderSent), priceBnd(zi.priceBnd), limPriceBnd(zi.limPriceBnd), ob(zi.ob) {}

ZeroIntelligence* ZeroIntelligence::copy() const {
    return new ZeroIntelligence(*this);
}

void ZeroIntelligence::printBook(int bookLevels, int tradeLevels, bool summarizeDepth) const {
    ob.printBook(bookLevels, tradeLevels, summarizeDepth);
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
    if (!sizes.size()) sizes = {1,2,2,3,3,4,4,5};
    int limit = 1, idx = 0;
    while (limit <= priceBnd) {
        for (int i=0; i<sizes[idx]; i++) {
            ob.process(LimitOrder(id++,time++,"ZI",BID,1,-limit));
            ob.process(LimitOrder(id++,time++,"ZI",ASK,1,+limit));
        }
        idx = min(idx+1,(int)sizes.size()-1);
        limit++;
    }
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

void ZeroIntelligence::sendCancelOrder(Side side) {
    // ob.process(CancelOrder(id++,time++,"ZI",idRef));
}

void ZeroIntelligence::generateOrder() {
    int event = 0;
    int a = ob.getTopAsk();
    int b = ob.getTopBid();
    int L = limPriceBnd;
    double p = uniformRand();
    vector<double> prob{
        limPriceBnd*limOrderArvRate,
        limPriceBnd*limOrderArvRate,
        mktOrderArvRate/2,
        mktOrderArvRate/2,
        ob.getBidDepthBetween(a-L,a-1)*cclOrderArvRate,
        ob.getAskDepthBetween(b+1,b+L)*cclOrderArvRate,
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
        case 4: sendCancelOrder(BID); break;
        case 5: sendCancelOrder(ASK); break;
        default: return;
    }
}

void ZeroIntelligence::simulate() {
    while (numOrderSent < numOrder) {
        generateOrder();
        numOrderSent++;
    }
}

#endif
