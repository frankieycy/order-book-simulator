#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP
#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "side.hpp"
#include "orderType.hpp"
using namespace std;

/**** global variables ********************************************************/

extern int TRADES_CLOCK; // clock for trades log

/**** helper functions ********************************************************/

bool match(Side side, double limit, double price);
int getTradesClock();
int setTradesClock(int time);

/**** class declarations ******************************************************/

class Order {
private:
    int id;
    int time;
    string name;
    OrderType type;
public:
    /**** constructors ****/
    Order(){}; virtual ~Order(){};
    Order(int id, int time, string name, OrderType type);
    Order(const Order& order);
    virtual Order* copy() const;
    /**** accessors ****/
    int getId() const {return id;}
    int getTime() const {return time;}
    string getName() const {return name;}
    OrderType getType() const {return type;}
    virtual string read() const;
    virtual string getAsJson() const;
    /**** mutators ****/
    int setId(int id);
    int setTime(int time);
    string setName(string name);
    OrderType setType(OrderType type);
    /**** operators ****/
    friend ostream& operator<<(ostream& out, const Order& order);
};

class LimitOrder : public Order {
private:
    Side side;
    int size;
    double price;
public:
    /**** constructors ****/
    LimitOrder(){}; ~LimitOrder(){}
    LimitOrder(int id, int time, string name, Side side, int size, double price);
    LimitOrder(const LimitOrder& order);
    LimitOrder* copy() const;
    /**** accessors ****/
    Side getSide() const {return side;}
    int getSize() const {return size;}
    double getPrice() const {return price;}
    string read() const;
    string getAsJson() const;
    /**** mutators ****/
    Side setSide(Side side);
    int setSize(int size);
    int reduceSize(int size);
    double setPrice(double price);
};

class MarketOrder : public Order {
private:
    Side side;
    int size;
public:
    /**** constructors ****/
    MarketOrder(){}; ~MarketOrder(){}
    MarketOrder(int id, int time, string name, Side side, int size);
    MarketOrder(const MarketOrder& order);
    MarketOrder* copy() const;
    /**** accessors ****/
    Side getSide() const {return side;}
    int getSize() const {return size;}
    string read() const;
    string getAsJson() const;
    /**** mutators ****/
    Side setSide(Side side);
    int setSize(int size);
};

class CancelOrder : public Order {
private:
    int idRef;
public:
    /**** constructors ****/
    CancelOrder(){}; ~CancelOrder(){};
    CancelOrder(int id, int time, string name, int idRef);
    CancelOrder(const CancelOrder& order);
    CancelOrder* copy() const;
    /**** accessors ****/
    int getIdRef() const {return idRef;}
    string read() const;
    string getAsJson() const;
    /**** mutators ****/
    int setIdRef(int idRef);
};

class ModifyOrder : public Order {
private:
    int idRef;
    Order* newOrder;
public:
    /**** constructors ****/
    ModifyOrder(){}; ~ModifyOrder();
    ModifyOrder(int id, int time, string name, int idRef, const Order& newOrder);
    ModifyOrder(const ModifyOrder& order);
    ModifyOrder* copy() const;
    /**** accessors ****/
    int getIdRef() const {return idRef;}
    Order* getNewOrder() const {return newOrder;}
    string read() const;
    string getAsJson() const;
    /**** mutators ****/
    int setIdRef(int idRef);
    Order* setNewOrder(const Order& newOrder);
    /**** operators ****/
    Order& operator=(const ModifyOrder& order); // TO-DO
};

class Trade {
private:
    int time;
    Side side;
    int size;
    double price;
    Order* bookOrder;
    Order* matchOrder;
public:
    /**** constructors ****/
    Trade(){}; ~Trade();
    Trade(int time, Side side, int size, double price,
        const Order& bookOrder, const Order& matchOrder);
    Trade(const Trade& trade);
    Trade* copy() const;
    /**** accessors ****/
    int getTime() const {return time;}
    int getId() const {return bookOrder->getId();}
    int getSize() const {return size;}
    double getPrice() const {return price;}
    Side getSide() const {return side;}
    string read() const;
    string getAsJson() const;
    /**** operators ****/
    Trade& operator=(const Trade& trade); // TO-DO
};

class LimitOrderBook {
private:
    string name;
    double topBid, topAsk;
    int bidTotalDepth, askTotalDepth;
    deque<Trade*> trades;
    deque<double> bidPrices, askPrices;
    deque<MarketOrder*> bidMktQueue, askMktQueue;
    map<int,Order*> ordersLog;
    map<int,double> bidsLog, asksLog;
    map<double,int> bidDepths, askDepths;
    map<double,deque<LimitOrder*>> bids, asks;
public:
    /**** constructors ****/
    LimitOrderBook(); ~LimitOrderBook();
    LimitOrderBook(string name);
    LimitOrderBook(const LimitOrderBook& book);
    LimitOrderBook* copy() const;
    /**** accessors ****/
    string getName() const {return name;}
    double getTopBid() const {return topBid;}
    double getTopAsk() const {return topAsk;}
    deque<Trade*> getTrades() const;
    deque<Trade*>* getTradesPtr() {return &trades;}
    deque<double> getBidPrices() const {return bidPrices;}
    deque<double> getAskPrices() const {return askPrices;}
    deque<double>* getBidPricesPtr() {return &bidPrices;}
    deque<double>* getAskPricesPtr() {return &askPrices;}
    deque<LimitOrder*> getBidOrders(double price) const;
    deque<LimitOrder*> getAskOrders(double price) const;
    deque<MarketOrder*> getBidMktQueue() const;
    deque<MarketOrder*> getAskMktQueue() const;
    map<int,Order*> getOrdersLog() const;
    map<int,Order*>* getOrdersLogPtr() {return &ordersLog;}
    map<int,double> getBidsLog() const {return bidsLog;}
    map<int,double> getAsksLog() const {return asksLog;}
    map<double,int> getBidDepths() const {return bidDepths;}
    map<double,int> getAskDepths() const {return askDepths;}
    map<double,int>* getBidDepthsPtr() {return &bidDepths;}
    map<double,int>* getAskDepthsPtr() {return &askDepths;}
    map<double,deque<LimitOrder*>> getBids() const;
    map<double,deque<LimitOrder*>> getAsks() const;
    map<double,deque<LimitOrder*>>* getBidsPtr() {return &bids;}
    map<double,deque<LimitOrder*>>* getAsksPtr() {return &asks;}
    int getBidTotalDepth() const {return bidTotalDepth;}
    int getAskTotalDepth() const {return askTotalDepth;}
    int getBidDepthAt(double price) const
        {return (bidDepths.count(price))?bidDepths.at(price):0;}
    int getAskDepthAt(double price) const
        {return (askDepths.count(price))?askDepths.at(price):0;}
    int getBidDepthBetween(double price0, double price1) const;
    int getAskDepthBetween(double price0, double price1) const;
    map<double,int> snapBidDepths(int bookLevels=0) const;
    map<double,int> snapAskDepths(int bookLevels=0) const;
    LimitOrder* peekBidOrderAt(double price) const;
    LimitOrder* peekAskOrderAt(double price) const;
    string read() const;
    string getAsJson() const;
    /**** main ****/
    double updateTopBid();
    double updateTopAsk();
    deque<double> updateBidPrices();
    deque<double> updateAskPrices();
    void updateBidAskPrices();
    void process(const LimitOrder& order);
    void process(const MarketOrder& order, bool isNew=true);
    void process(const CancelOrder& order);
    void process(const ModifyOrder& order);
    void processMktQueue(Side side);
    void processOrder(const Order& order);
    void printBook(int bookLevels=0, int tradeLevels=0,
        bool summarizeDepth=true) const;
};

class Exchange {
private:
    vector<LimitOrderBook*> books;
};

/**** operators ***************************************************************/

ostream& operator<<(ostream& out, const Side& side);
ostream& operator<<(ostream& out, const OrderType& type);
ostream& operator<<(ostream& out, Order* const order);
ostream& operator<<(ostream& out, const Order& order);
ostream& operator<<(ostream& out, Trade* const trade);
ostream& operator<<(ostream& out, const Trade& trade);

#endif
