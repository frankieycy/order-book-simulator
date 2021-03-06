#ifndef ORDERBOOK_CPP
#define ORDERBOOK_CPP
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include "util.cpp"
#include "side.hpp"
#include "orderType.hpp"
#include "orderBook.hpp"
using namespace std;

/**** global variables ********************************************************/

int TRADES_CLOCK = 0; // clock for trades log

/**** helper functions ********************************************************/

bool match(Side side, double limit, double price) {
    return (side==BID)?(price<=limit):((side==ASK)?(price>=limit):false);
}

int getTradesClock() {
    return TRADES_CLOCK;
}

int setTradesClock(int time) {
    TRADES_CLOCK = time;
    return TRADES_CLOCK;
}

/**** class functions *********************************************************/
//### Order class ##############################################################

Order::Order(int id, int time, string name, OrderType type): id(id), time(time), name(name), type(type) {}

Order::Order(const Order& order): id(order.id), time(order.time), name(order.name), type(order.type) {}

Order* Order::copy() const {
    return new Order(*this);
}

string Order::read() const {
    ostringstream oss;
    oss << getName() << " " << getType();
    return oss.str();
}

string Order::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()   << "," <<
    "\"time\":"   << getTime() << "," <<
    "\"name\":\"" << getName() << "\"," <<
    "\"type\":\"" << getType() << "\"" <<
    "}";
    return oss.str();
}

int Order::setId(int id) {
    this->id = id;
    return this->id;
}

int Order::setTime(int time) {
    this->time = time;
    return this->time;
}

string Order::setName(string name) {
    this->name = name;
    return this->name;
}

OrderType Order::setType(OrderType type) {
    this->type = type;
    return this->type;
}

//### LimitOrder class #########################################################

LimitOrder::LimitOrder(int id, int time, string name, Side side, int size, double price): Order(id, time, name, LIMIT), side(side), size(size), price(price) {}

LimitOrder::LimitOrder(const LimitOrder& order): Order(order), side(order.side), size(order.size), price(order.price) {}

LimitOrder* LimitOrder::copy() const {
    return new LimitOrder(*this);
}

string LimitOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " " << getSide() << " " << getSize() << " @ $" << getPrice();
    return oss.str();
}

string LimitOrder::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()    << "," <<
    "\"time\":"   << getTime()  << "," <<
    "\"name\":\"" << getName()  << "\"," <<
    "\"type\":\"" << getType()  << "\"," <<
    "\"side\":\"" << getSide()  << "\"," <<
    "\"size\":"   << getSize()  << "," <<
    "\"price\":"  << getPrice() <<
    "}";
    return oss.str();
}

Side LimitOrder::setSide(Side side) {
    this->side = side;
    return this->side;
}

int LimitOrder::setSize(int size) {
    this->size = size;
    return this->size;
}

int LimitOrder::reduceSize(int size) {
    this->size -= size;
    return this->size;
}

double LimitOrder::setPrice(double price) {
    this->price = price;
    return this->price;
}

//### MarketOrder class ########################################################

MarketOrder::MarketOrder(int id, int time, string name, Side side, int size): Order(id, time, name, MARKET), side(side), size(size) {}

MarketOrder::MarketOrder(const MarketOrder& order): Order(order), side(order.side), size(order.size) {}

MarketOrder* MarketOrder::copy() const {
    return new MarketOrder(*this);
}

string MarketOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " " << getSide() << " " << getSize();
    return oss.str();
}

string MarketOrder::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()   << "," <<
    "\"time\":"   << getTime() << "," <<
    "\"name\":\"" << getName() << "\"," <<
    "\"type\":\"" << getType() << "\"," <<
    "\"side\":\"" << getSide() << "\"," <<
    "\"size\":"   << getSize() <<
    "}";
    return oss.str();
}

Side MarketOrder::setSide(Side side) {
    this->side = side;
    return this->side;
}

int MarketOrder::setSize(int size) {
    this->size = size;
    return this->size;
}

//### CancelOrder class ########################################################

CancelOrder::CancelOrder(int id, int time, string name, int idRef): Order(id, time, name, CANCEL), idRef(idRef) {}

CancelOrder::CancelOrder(const CancelOrder& order): Order(order), idRef(order.idRef) {}

CancelOrder* CancelOrder::copy() const {
    return new CancelOrder(*this);
}

string CancelOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " id: " << getIdRef();
    return oss.str();
}

string CancelOrder::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()    << "," <<
    "\"time\":"   << getTime()  << "," <<
    "\"name\":\"" << getName()  << "\"," <<
    "\"type\":\"" << getType()  << "\"," <<
    "\"idRef\":"  << getIdRef() <<
    "}";
    return oss.str();
}

int CancelOrder::setIdRef(int idRef) {
    this->idRef = idRef;
    return this->idRef;
}

//### ModifyOrder class ########################################################

ModifyOrder::ModifyOrder(int id, int time, string name, int idRef, const Order& newOrder): Order(id, time, name, MODIFY), idRef(idRef), newOrder(newOrder.copy()) {}

ModifyOrder::ModifyOrder(const ModifyOrder& order): Order(order), idRef(order.idRef), newOrder(order.copy()) {}

ModifyOrder::~ModifyOrder() {
    delete newOrder;
}

ModifyOrder* ModifyOrder::copy() const {
    return new ModifyOrder(*this);
}

string ModifyOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " id: " << getIdRef() << " " << *getNewOrder();
    return oss.str();
}

string ModifyOrder::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"       << getId()        << "," <<
    "\"time\":"     << getTime()      << "," <<
    "\"name\":\""   << getName()      << "\"," <<
    "\"type\":\""   << getType()      << "\"," <<
    "\"idRef\":"    << getIdRef()     << "," <<
    "\"newOrder\":" << *getNewOrder() <<
    "}";
    return oss.str();
}

int ModifyOrder::setIdRef(int idRef) {
    this->idRef = idRef;
    return this->idRef;
}

Order* ModifyOrder::setNewOrder(const Order& newOrder) {
    delete this->newOrder;
    this->newOrder = newOrder.copy();
    return this->newOrder;
}

//### Trade class ##############################################################

Trade::Trade(int time, Side side, int size, double price, const Order& bookOrder, const Order& matchOrder): time(time), side(side), size(size), price(price), bookOrder(bookOrder.copy()), matchOrder(matchOrder.copy()) {}

Trade::Trade(const Trade& trade): time(trade.time), side(trade.side), size(trade.size), price(trade.price), bookOrder(trade.bookOrder->copy()), matchOrder(trade.matchOrder->copy()) {}

Trade::~Trade() {
    delete bookOrder;
    delete matchOrder;
}

Trade* Trade::copy() const {
    return new Trade(*this);
}

string Trade::read() const {
    ostringstream oss;
    oss << "trade " << matchOrder->getName() << " " << side << " " << size << " @ $" << price;
    return oss.str();
}

string Trade::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"time\":"       << time                    << "," <<
    "\"side\":\""     << side                    << "\"," <<
    "\"size\":"       << size                    << "," <<
    "\"price\":"      << price                   << "," <<
    "\"bookOrder\":"  << bookOrder->getAsJson()  << "," <<
    "\"matchOrder\":" << matchOrder->getAsJson() <<
    "}";
    return oss.str();
}

//### LimitOrderBook class #####################################################

LimitOrderBook::LimitOrderBook(): name(""), topBid(0), topAsk(0), bidTotalDepth(0), askTotalDepth(0) {}

LimitOrderBook::LimitOrderBook(string name): name(name), topBid(0), topAsk(0), bidTotalDepth(0), askTotalDepth(0) {}

LimitOrderBook::LimitOrderBook(const LimitOrderBook& book): name(book.name), topBid(book.topBid), topAsk(book.topAsk), bidTotalDepth(book.bidTotalDepth), askTotalDepth(book.askTotalDepth), bidPrices(book.bidPrices), askPrices(book.askPrices), bidsLog(book.bidsLog), asksLog(book.asksLog), bidDepths(book.bidDepths), askDepths(book.askDepths) {
    // TO-DO: deep copy ptr
}

LimitOrderBook::~LimitOrderBook() {
    for (auto o : bidMktQueue) delete o;
    for (auto o : askMktQueue) delete o;
    for (auto t : trades) delete t;
    for (auto o : ordersLog) delete o.second;
    for (auto b : bids)
        for (auto o : b.second) delete o;
    for (auto a : asks)
        for (auto o : a.second) delete o;
}

LimitOrderBook* LimitOrderBook::copy() const {
    return new LimitOrderBook(*this);
}

deque<Trade*> LimitOrderBook::getTrades() const {
    deque<Trade*> tradesCopy;
    for (auto t : trades) tradesCopy.push_back(t->copy());
    return tradesCopy;
}

deque<LimitOrder*> LimitOrderBook::getBidOrders(double price) const {
    auto i = bids.find(price);
    if (i != bids.end()) {
        deque<LimitOrder*> orders;
        for (auto o : i->second) orders.push_back(o->copy());
        return orders;
    } else return {};
}

deque<LimitOrder*> LimitOrderBook::getAskOrders(double price) const {
    auto i = asks.find(price);
    if (i != asks.end()) {
        deque<LimitOrder*> orders;
        for (auto o : i->second) orders.push_back(o->copy());
        return orders;
    } else return {};
}

deque<MarketOrder*> LimitOrderBook::getBidMktQueue() const {
    deque<MarketOrder*> bidMktQueueCopy;
    for (auto o : bidMktQueue) bidMktQueueCopy.push_back(o->copy());
    return bidMktQueueCopy;
}

deque<MarketOrder*> LimitOrderBook::getAskMktQueue() const {
    deque<MarketOrder*> askMktQueueCopy;
    for (auto o : askMktQueue) askMktQueueCopy.push_back(o->copy());
    return askMktQueueCopy;
}

map<int,Order*> LimitOrderBook::getOrdersLog() const {
    map<int,Order*> ordersLogCopy;
    for (auto o : ordersLog) ordersLogCopy[o.first] = o.second->copy();
    return ordersLogCopy;
}

map<double,deque<LimitOrder*>> LimitOrderBook::getBids() const {
    map<double,deque<LimitOrder*>> bidsCopy;
    for (auto b : bids)
        for (auto o : b.second) bidsCopy[b.first].push_back(o->copy());
    return bidsCopy;
}

map<double,deque<LimitOrder*>> LimitOrderBook::getAsks() const {
    map<double,deque<LimitOrder*>> asksCopy;
    for (auto a : asks)
        for (auto o : a.second) asksCopy[a.first].push_back(o->copy());
    return asksCopy;
}

int LimitOrderBook::getBidDepthBetween(double price0, double price1) const{
    int cumDepth = 0;
    auto i0 = lower_bound(bidPrices.begin(), bidPrices.end(), price1, greater<double>());
    auto i1 = upper_bound(bidPrices.begin(), bidPrices.end(), price0, greater<double>());
    for (auto i=i0; i!=i1; i++) cumDepth += bidDepths.at(*i);
    return cumDepth;
}

int LimitOrderBook::getAskDepthBetween(double price0, double price1) const{
    int cumDepth = 0;
    auto i0 = lower_bound(askPrices.begin(), askPrices.end(), price0);
    auto i1 = upper_bound(askPrices.begin(), askPrices.end(), price1);
    for (auto i=i0; i!=i1; i++) cumDepth += askDepths.at(*i);
    return cumDepth;
}

map<double,int> LimitOrderBook::snapBidDepths(int bookLevels) const {
    if (!bookLevels) bookLevels = bidPrices.size();
    map<double,int> bidDepthsSnap;
    for (auto p : bidPrices) {
        bidDepthsSnap[p] = bidDepths.at(p);
        if (bidDepthsSnap.size() == bookLevels) break;
    }
    return bidDepthsSnap;
}

map<double,int> LimitOrderBook::snapAskDepths(int bookLevels) const {
    if (!bookLevels) bookLevels = askPrices.size();
    map<double,int> askDepthsSnap;
    for (auto p : askPrices) {
        askDepthsSnap[p] = askDepths.at(p);
        if (askDepthsSnap.size() == bookLevels) break;
    }
    return askDepthsSnap;
}

LimitOrder* LimitOrderBook::peekBidOrderAt(double price) const {
    auto i = bids.find(price);
    if (i != bids.end()) return i->second.front()->copy();
    else return 0;
}

LimitOrder* LimitOrderBook::peekAskOrderAt(double price) const {
    auto i = asks.find(price);
    if (i != asks.end()) return i->second.front()->copy();
    else return 0;
}

string LimitOrderBook::read() const {
    // TO-DO
    return "";
}

string LimitOrderBook::getAsJson() const {
    ostringstream oss;
    oss << "{";
    oss << "\"asks\":{";
    for (auto i=askPrices.begin(); i!=askPrices.end(); i++)
        oss << *i << ":" << asks.at(*i) << ((i==askPrices.end()-1)?"":",");
    oss << "},";
    oss << "\"bids\":{";
    for (auto i=bidPrices.begin(); i!=bidPrices.end(); i++)
        oss << *i << ":" << bids.at(*i) << ((i==bidPrices.end()-1)?"":",");
    oss << "}";
    oss << "}";
    return oss.str();
}

double LimitOrderBook::updateTopBid() {
    topBid = (bidPrices.size()>0)?bidPrices[0]:0;
    return topBid;
}

double LimitOrderBook::updateTopAsk() {
    topAsk = (askPrices.size()>0)?askPrices[0]:0;
    return topAsk;
}

deque<double> LimitOrderBook::updateBidPrices() {
    bidPrices.clear();
    for (auto i=bids.begin(); i!=bids.end(); i++) bidPrices.push_back(i->first);
    reverse(bidPrices.begin(), bidPrices.end());
    return bidPrices;
}

deque<double> LimitOrderBook::updateAskPrices() {
    askPrices.clear();
    for (auto i=asks.begin(); i!=asks.end(); i++) askPrices.push_back(i->first);
    return askPrices;
}

void LimitOrderBook::updateBidAskPrices() {
    updateBidPrices();
    updateAskPrices();
    updateTopBid();
    updateTopAsk();
}

void LimitOrderBook::process(const LimitOrder& order) {
    int id = order.getId();
    Side side = order.getSide();
    if (side == NULL_SIDE) return;
    double limit = order.getPrice();
    int unfilledSize = order.getSize();
    int* sameSideTotalDepth = (side==BID)?&bidTotalDepth:&askTotalDepth;
    int* oppSideTotalDepth = (side==BID)?&askTotalDepth:&bidTotalDepth;
    deque<double>* sameSidePrices = (side==BID)?&bidPrices:&askPrices;
    deque<double>* oppSidePrices = (side==BID)?&askPrices:&bidPrices;
    map<double,int>* sameSideDepths = (side==BID)?&bidDepths:&askDepths;
    map<double,int>* oppSideDepths = (side==BID)?&askDepths:&bidDepths;
    map<double,deque<LimitOrder*>>* sameSide = (side==BID)?&bids:&asks;
    map<double,deque<LimitOrder*>>* oppSide = (side==BID)?&asks:&bids;
    map<int,double>* sameSideLOLog = (side==BID)?&bidsLog:&asksLog;
    map<int,double>* oppSideLOLog = (side==BID)?&asksLog:&bidsLog;
    ordersLog[id] = order.copy();
    while (unfilledSize && oppSidePrices->size() && match(side, limit, oppSidePrices->front())) {
        int* oppSideDepthAtPrice = &oppSideDepths->at(oppSidePrices->front());
        deque<LimitOrder*>* orders = &oppSide->at(oppSidePrices->front());
        while (unfilledSize && orders->size()) {
            int matchedSize = min(unfilledSize, orders->front()->getSize());
            Trade* trade = new Trade(getTradesClock(), side, matchedSize, orders->front()->getPrice(), *orders->front(), order);
            trades.push_back(trade);
            unfilledSize -= matchedSize;
            orders->front()->reduceSize(matchedSize);
            *oppSideDepthAtPrice -= matchedSize;
            *oppSideTotalDepth -= matchedSize;
            if (!orders->front()->getSize()) {
                oppSideLOLog->erase(orders->front()->getId());
                orders->pop_front();
            }
        }
        if (!orders->size()) {
            oppSide->erase(oppSidePrices->front());
            oppSideDepths->erase(oppSidePrices->front());
            oppSidePrices->pop_front();
        }
    }
    if (unfilledSize) {
        LimitOrder* updatedOrder = order.copy();
        updatedOrder->setSize(unfilledSize);
        (*sameSide)[limit].push_back(updatedOrder);
        (*sameSideLOLog)[id] = limit;
        (*sameSideDepths)[limit] += unfilledSize;
        *sameSideTotalDepth += unfilledSize;
        auto i = find(sameSidePrices->begin(), sameSidePrices->end(), limit);
        auto c = [side,limit](double p){return (side==BID)?limit>p:limit<p;};
        if (i == sameSidePrices->end())
            sameSidePrices->insert(find_if(sameSidePrices->begin(), sameSidePrices->end(), c), limit);
    }
    updateTopBid();
    updateTopAsk();
    processMktQueue((side==BID)?ASK:BID);
}

void LimitOrderBook::process(const MarketOrder& order, bool isNew) {
    int id = order.getId();
    Side side = order.getSide();
    if (side == NULL_SIDE) return;
    int unfilledSize = order.getSize();
    int* oppSideTotalDepth = (side==BID)?&askTotalDepth:&bidTotalDepth;
    deque<double>* oppSidePrices = (side==BID)?&askPrices:&bidPrices;
    map<double,int>* oppSideDepths = (side==BID)?&askDepths:&bidDepths;
    map<double,deque<LimitOrder*>>* oppSide = (side==BID)?&asks:&bids;
    map<int,double>* oppSideLOLog = (side==BID)?&asksLog:&bidsLog;
    ordersLog[id] = order.copy();
    while (unfilledSize && oppSidePrices->size()) {
        int* oppSideDepthAtPrice = &oppSideDepths->at(oppSidePrices->front());
        deque<LimitOrder*>* orders = &oppSide->at(oppSidePrices->front());
        while (unfilledSize && orders->size()) {
            int matchedSize = min(unfilledSize, orders->front()->getSize());
            Trade* trade = new Trade(getTradesClock(), side, matchedSize, orders->front()->getPrice(), *orders->front(), order);
            trades.push_back(trade);
            unfilledSize -= matchedSize;
            orders->front()->reduceSize(matchedSize);
            *oppSideDepthAtPrice -= matchedSize;
            *oppSideTotalDepth -= matchedSize;
            if (!orders->front()->getSize()) {
                oppSideLOLog->erase(orders->front()->getId());
                orders->pop_front();
            }
        }
        if (!orders->size()) {
            oppSide->erase(oppSidePrices->front());
            oppSideDepths->erase(oppSidePrices->front());
            oppSidePrices->pop_front();
        }
    }
    if (unfilledSize) {
        MarketOrder* updatedOrder = order.copy();
        updatedOrder->setSize(unfilledSize);
        deque<MarketOrder*>* mktQueue = (side==BID)?&bidMktQueue:&askMktQueue;
        if (isNew) mktQueue->push_back(updatedOrder);
        else mktQueue->push_front(updatedOrder);
    }
    updateTopBid();
    updateTopAsk();
}

void LimitOrderBook::process(const CancelOrder& order) {
    int id = order.getIdRef();
    ordersLog[id] = order.copy();
    /**** Implementation 1 ****/
    bool onBidBook = bidsLog.find(id)!=bidsLog.end();
    bool onAskBook = asksLog.find(id)!=asksLog.end();
    if (onBidBook || onAskBook) {
        Side side = (onBidBook)?BID:ASK;
        int* sameSideTotalDepth = (side==BID)?&bidTotalDepth:&askTotalDepth;
        deque<double>* sameSidePrices = (side==BID)?&bidPrices:&askPrices;
        map<double,int>* sameSideDepths = (side==BID)?&bidDepths:&askDepths;
        map<double,deque<LimitOrder*>>* sameSide = (side==BID)?&bids:&asks;
        map<int,double>* sameSideLOLog = (side==BID)?&bidsLog:&asksLog;
        double limit = sameSideLOLog->at(id);
        deque<LimitOrder*>* orders = &sameSide->at(limit);
        auto i = lower_bound(orders->begin(), orders->end(), id, [](LimitOrder* o, int id){return o->getId()<id;});
        if (i != orders->end() && (*i)->getId()==id) {
            delete *i;
            orders->erase(i);
            sameSideLOLog->erase(id);
            (*sameSideDepths)[limit] -= (*i)->getSize();
            *sameSideTotalDepth -= (*i)->getSize();
        }
        if (!orders->size()) {
            sameSide->erase(limit);
            sameSideDepths->erase(limit);
            auto i = find(sameSidePrices->begin(), sameSidePrices->end(), limit);
            if (i != sameSidePrices->end()) sameSidePrices->erase(i);
        }
    } else {
        for (auto orders : {&bidMktQueue, &askMktQueue}) {
            auto i = lower_bound(orders->begin(), orders->end(), id, [](MarketOrder* o, int id){return o->getId()<id;});
            if (i != orders->end() && (*i)->getId()==id) {
                delete *i;
                orders->erase(i);
                break;
            }
        }
    }
    updateTopBid();
    updateTopAsk();
    /**** Implementation 2 ****/
    // auto i = ordersLog.find(id); // SLOW!
    // if (i == ordersLog.end()) return;
    // Order* origOrder = i->second;
    // OrderType type = origOrder->getType();
    // ordersLog[id] = order.copy();
    // if (type == LIMIT) {
    //     LimitOrder* refOrder = dynamic_cast<LimitOrder*>(origOrder);
    //     Side side = refOrder->getSide();
    //     double limit = refOrder->getPrice();
    //     deque<double>* sameSidePrices = (side==BID)?&bidPrices:&askPrices;
    //     map<double,deque<LimitOrder*>>* sameSide = (side==BID)?&bids:&asks;
    //     map<int,double>* sameSideLOLog = (side==BID)?&bidsLog:&asksLog;
    //     bool onBook = sameSide->find(limit)!=sameSide->end();
    //     if (onBook) {
    //         deque<LimitOrder*>* orders = &sameSide->at(limit);
    //         for (auto i=orders->begin(); i!=orders->end(); i++)
    //             if ((*i)->getId()==id) {
    //                 delete *i;
    //                 orders->erase(i);
    //                 sameSideLOLog->erase(id);
    //                 break;
    //             }
    //         if (!orders->size()) {
    //             sameSide->erase(limit);
    //             auto i = find(sameSidePrices->begin(), sameSidePrices->end(), limit);
    //             if (i != sameSidePrices->end()) sameSidePrices->erase(i);
    //         }
    //     }
    // } else if (type == MARKET) {
    //     MarketOrder* refOrder = dynamic_cast<MarketOrder*>(origOrder);
    //     Side side = refOrder->getSide();
    //     deque<MarketOrder*>* orders = (side==BID)?&bidMktQueue:&askMktQueue;
    //     for (auto i=orders->begin(); i!=orders->end(); i++)
    //         if ((*i)->getId()==id) {
    //             delete *i;
    //             orders->erase(i);
    //             break;
    //         }
    // }
    // updateTopBid();
    // updateTopAsk();
}

void LimitOrderBook::process(const ModifyOrder& order) {

}

void LimitOrderBook::processMktQueue(Side side) {
    if (side == NULL_SIDE) return;
    deque<double>* oppSidePrices = (side==BID)?&askPrices:&bidPrices;
    deque<MarketOrder*>* mktQueue = (side==BID)?&bidMktQueue:&askMktQueue;
    map<double,deque<LimitOrder*>>* sameSide = (side==BID)?&bids:&asks;
    map<double,deque<LimitOrder*>>* oppSide = (side==BID)?&asks:&bids;
    while (oppSidePrices->size() && mktQueue->size()) {
        MarketOrder* topMktOrder = mktQueue->front();
        mktQueue->pop_front();
        process(*topMktOrder, false);
    }
}

void LimitOrderBook::processOrder(const Order& order) {
    OrderType type = order.getType();
    switch(type) {
        case LIMIT: process(*dynamic_cast<LimitOrder*>(order.copy())); break;
        case MARKET: process(*dynamic_cast<MarketOrder*>(order.copy())); break;
        case CANCEL: process(*dynamic_cast<CancelOrder*>(order.copy())); break;
        default: return;
    }
}

void LimitOrderBook::printBook(int bookLevels, int tradeLevels, bool summarizeDepth) const {
    cout << "-------------------------------------------" << endl;
    if (summarizeDepth) {
        if (askPrices.size()) {
            for (auto i=((bookLevels>0)?askPrices.begin()+min(bookLevels,(int)askPrices.size()):askPrices.end())-1; i!=askPrices.begin()-1; i--)
                cout << "Level " << i-askPrices.begin()+1 << " : " << getAskDepthAt(*i) << " @ $" << *i << endl;
            cout << "--------------------ASK--------------------" << endl;
        }
        if (bidPrices.size()) {
            cout << "--------------------BID--------------------" << endl;
            for (auto i=bidPrices.begin(); i!=((bookLevels>0)?bidPrices.begin()+min(bookLevels,(int)bidPrices.size()):bidPrices.end()); i++)
                cout << "Level " << i-bidPrices.begin()+1 << " : " << getBidDepthAt(*i) << " @ $" << *i << endl;
        }
        if (trades.size()) {
            cout << "-------------------TRADE-------------------" << endl;
            for (auto i=trades.end()-1; i!=((tradeLevels>0)?trades.end()-min(tradeLevels,(int)trades.size()):trades.begin())-1; i--)
                cout << "Trade " << trades.end()-i << " : " << (*i)->read() << endl;
        }
    } else {
        if (askPrices.size()) {
            for (auto i=((bookLevels>0)?askPrices.begin()+min(bookLevels,(int)askPrices.size()):askPrices.end())-1; i!=askPrices.begin()-1; i--)
                cout << "Level " << i-askPrices.begin()+1 << " @ $" << *i << " : " << asks.at(*i) << endl;
            cout << "--------------------ASK--------------------" << endl;
        }
        if (bidPrices.size()) {
            cout << "--------------------BID--------------------" << endl;
            for (auto i=bidPrices.begin(); i!=((bookLevels>0)?bidPrices.begin()+min(bookLevels,(int)bidPrices.size()):bidPrices.end()); i++)
                cout << "Level " << i-bidPrices.begin()+1 << " @ $" << *i << " : " << bids.at(*i) << endl;
        }
        if (trades.size()) {
            cout << "-------------------TRADE-------------------" << endl;
            for (auto i=trades.end()-1; i!=((tradeLevels>0)?trades.end()-min(tradeLevels,(int)trades.size()):trades.begin())-1; i--)
                cout << "Trade " << trades.end()-i << " : " << *i << endl;
        }
    }
    cout << "-------------------------------------------" << endl;
}

/**** operators ***************************************************************/

ostream& operator<<(ostream& out, const Side& side) {
    switch(side) {
        case BID:       out << "BID"; break;
        case ASK:       out << "ASK"; break;
        case NULL_SIDE: out << "NULL"; break;
        default:        out << "NULL";
    }
    return out;
}

ostream& operator<<(ostream& out, const OrderType& type) {
    switch(type) {
        case LIMIT:    out << "LIMIT"; break;
        case MARKET:   out << "MARKET"; break;
        case CANCEL:   out << "CANCEL"; break;
        case MODIFY:   out << "MODIFY"; break;
        case NULL_ORD: out << "NULL"; break;
        default:       out << "NULL";
    }
    return out;
}

ostream& operator<<(ostream& out, Order* const order) {
    if (order != 0) out << order->getAsJson();
    return out;
}

ostream& operator<<(ostream& out, const Order& order) {
    out << order.getAsJson();
    return out;
}

ostream& operator<<(ostream& out, Trade* const trade) {
    if (trade != 0) out << trade->getAsJson();
    return out;
}

ostream& operator<<(ostream& out, const Trade& trade) {
    out << trade.getAsJson();
    return out;
}

#endif
