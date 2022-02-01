#ifndef ORDERBOOK_CPP
#define ORDERBOOK_CPP
#include "util.cpp"
using namespace std;

enum Side {BID, ASK};
enum OrderType {UNDEF, LIMIT, MARKET, CANCEL, MODIFY};

/******************************************************************************/

class Order {
private:
    int id;
    int time;
    string name;
    OrderType type;
public:
    /**** constructors ****/
    Order(){}; ~Order(){}
    Order(int id, int time, string name, OrderType type);
    Order(const Order& order);
    virtual Order* copy() const;
    /**** accessors ****/
    int getId() const {return id;}
    int getTime() const {return time;}
    string getName() const {return name;}
    OrderType getType() const {return type;}
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
    double price;
public:
    /**** constructors ****/
    LimitOrder(){}; ~LimitOrder(){}
    LimitOrder(int id, int time, string name, Side side, double price);
    LimitOrder(const LimitOrder& order);
    Order* copy() const;
    /**** accessors ****/
    Side getSide() const {return side;}
    double getPrice() const {return price;}
    string getAsJson() const;
    /**** mutators ****/
    Side setSide(Side side);
    double setPrice(double price);
};

class MarketOrder : public Order {
private:
    Side side;
public:
    /**** constructors ****/
    MarketOrder(){}; ~MarketOrder(){}
    MarketOrder(int id, int time, string name, Side side);
    MarketOrder(const MarketOrder& order);
    Order* copy() const;
    /**** accessors ****/
    Side getSide() const {return side;}
    string getAsJson() const;
    /**** mutators ****/
    Side setSide(Side side);
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
    Order* copy() const;
    /**** accessors ****/
    int getIdRef() const {return idRef;}
    Order* getNewOrder() const {return newOrder;}
    string getAsJson() const;
    /**** mutators ****/
    int setIdRef(int idRef);
    Order* setNewOrder(const Order& newOrder);
};

class Trade {
public:
    Side side;
    double price;
    int size;
    Order* bookOrder;
    Order* matchOrder;
    Trade(){}
    Trade(Side side, double price, int size, const Order& bookOrder, const Order& matchOrder);
    string getAsJson() const;
};

class OrderBook {
private:
    map<int,vector<Order*>> bids, asks;
    vector<Trade*> trades;
public:
    OrderBook(){}
    map<int,vector<Order*>> getBids() const {return bids;}
    map<int,vector<Order*>> getAsks() const {return asks;}
    vector<Trade*> getTrades() const {return trades;}
    vector<int> getBidPrices() const;
    vector<int> getAskPrices() const;
    vector<Order*> getBidOrders(double price) const;
    vector<Order*> getAskOrders(double price) const;
    vector<int> getBidDepth() const;
    vector<int> getAskDepth() const;
    double bestBid() const;
    double bestAsk() const;
    void process(const Order& order);
    void printBook() const;
};

/******************************************************************************/

ostream& operator<<(ostream& out, const Side& side) {
    switch(side) {
        case BID: out << "BID"; break;
        case ASK: out << "ASK"; break;
        default:  out << "NULL";
    }
    return out;
}

ostream& operator<<(ostream& out, const OrderType& type) {
    switch(type) {
        case UNDEF:  out << "UNDEF"; break;
        case LIMIT:  out << "LIMIT"; break;
        case MARKET: out << "MARKET"; break;
        case CANCEL: out << "CANCEL"; break;
        case MODIFY: out << "MODIFY"; break;
        default:     out << "NULL";
    }
    return out;
}

ostream& operator<<(ostream& out, const Order& order) {
    out << order.getAsJson();
    return out;
}

/******************************************************************************/

Order::Order(int id, int time, string name, OrderType type): id(id), time(time), name(name), type(type) {}

Order::Order(const Order& order) {
    this->id = order.id;
    this->time = order.time;
    this->name = order.name;
    this->type = order.type;
}

Order* Order::copy() const {
    return new Order(*this);
}

string Order::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()   << "," <<
    "\"time\":"   << getTime() << "," <<
    "\"name\":\"" << getName() << "\"," <<
    "\"type\":\"" << getType()  << "\"" <<
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

/******************************************************************************/

LimitOrder::LimitOrder(int id, int time, string name, Side side, double price): Order(id, time, name, LIMIT), side(side), price(price) {}

LimitOrder::LimitOrder(const LimitOrder& order): Order(order) {
    this->side = order.side;
    this->price = order.price;
}

Order* LimitOrder::copy() const {
    return new LimitOrder(*this);
}

string LimitOrder::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()    << "," <<
    "\"time\":"   << getTime()  << "," <<
    "\"name\":\"" << getName()  << "\"," <<
    "\"type\":\"" << getType()  << "\"," <<
    "\"side\":\"" << getSide()  << "\"," <<
    "\"price\":"  << getPrice() <<
    "}";
    return oss.str();
}

Side LimitOrder::setSide(Side side) {
    this->side = side;
    return this->side;
}

double LimitOrder::setPrice(double price) {
    this->price = price;
    return this->price;
}

/******************************************************************************/

MarketOrder::MarketOrder(int id, int time, string name, Side side): Order(id, time, name, MARKET), side(side) {}

MarketOrder::MarketOrder(const MarketOrder& order): Order(order) {
    this->side = order.side;
}

Order* MarketOrder::copy() const {
    return new MarketOrder(*this);
}

string MarketOrder::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()   << "," <<
    "\"time\":"   << getTime() << "," <<
    "\"name\":\"" << getName() << "\"," <<
    "\"type\":\"" << getType() << "\"," <<
    "\"side\":\"" << getSide() << "\"" <<
    "}";
    return oss.str();
}

Side MarketOrder::setSide(Side side) {
    this->side = side;
    return this->side;
}

/******************************************************************************/

ModifyOrder::ModifyOrder(int id, int time, string name, int idRef, const Order& newOrder): Order(id, time, name, MODIFY), idRef(idRef), newOrder(newOrder.copy()) {}

ModifyOrder::ModifyOrder(const ModifyOrder& order): Order(order) {
    this->idRef = order.idRef;
    delete this->newOrder;
    this->newOrder = order.newOrder->copy();
}

ModifyOrder::~ModifyOrder() {
    delete newOrder;
}

Order* ModifyOrder::copy() const {
    return new ModifyOrder(*this);
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

/******************************************************************************/

Trade::Trade(Side side, double price, int size, const Order& bookOrder, const Order& matchOrder): side(side), price(price), size(size), bookOrder(bookOrder.copy()), matchOrder(matchOrder.copy()) {}

string Trade::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"side\":\""     << side  << "\"," <<
    "\"price\":"      << price << "," <<
    "\"size\":"       << size  << "," <<
    "\"bookOrder\":"  << bookOrder->getAsJson() << "," <<
    "\"matchOrder\":" << matchOrder->getAsJson() <<
    "}";
    return oss.str();
}

#endif
