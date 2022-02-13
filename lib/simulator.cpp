#ifndef SIMULATOR_CPP
#define SIMULATOR_CPP
#include "orderBook.cpp"
using namespace std;

class ZeroIntelligence {
private:
    int id;
    double mktOrderArvRate;
    double limOrderArvRate;
    double cclOrderArvRate;
    LimitOrderBook ob;
public:
    /**** constructors ****/
    ZeroIntelligence(); ~ZeroIntelligence();
    ZeroIntelligence(const ZeroIntelligence& zi);
    ZeroIntelligence* copy() const;
    /**** accessors ****/
    double getMktOrderArvRate() const {return mktOrderArvRate;}
    double getLimOrderArvRate() const {return limOrderArvRate;}
    double getCclOrderArvRate() const {return cclOrderArvRate;}
    /**** mutators ****/
    double setMktOrderArvRate(double arvRate);
    double setLimOrderArvRate(double arvRate);
    double setCclOrderArvRate(double arvRate);
    /**** main ****/
    void initOrderBook();
    void sendLimitOrder();
    void sendMarketOrder();
    void sendCancelOrder();
    void simulate();
};

/******************************************************************************/

ZeroIntelligence::ZeroIntelligence(): id(0), mktOrderArvRate(0), limOrderArvRate(0), cclOrderArvRate(0) {}

ZeroIntelligence::ZeroIntelligence(const ZeroIntelligence& zi) {
    //
}

ZeroIntelligence* ZeroIntelligence::copy() const {
    return new ZeroIntelligence(*this);
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


#endif
