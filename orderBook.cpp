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
    Order(){}; virtual ~Order(){};
    Order(const Order& order);
    Order(int id, int time, string name, OrderType type);
    virtual Order* copy() const;
    /**** accessors ****/
    int getId() const {return id;}
    int getTime() const {return time;}
    string getName() const {return name;}
    OrderType getType() const {return type;}
    virtual string getAsJson() const;
    virtual string read() const;
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
    int size;
    Side side;
    double price;
public:
    /**** constructors ****/
    LimitOrder(){}; ~LimitOrder(){}
    LimitOrder(const LimitOrder& order);
    LimitOrder(int id, int time, string name, Side side, int size, double price);
    LimitOrder* copy() const;
    /**** accessors ****/
    int getSize() const {return size;}
    Side getSide() const {return side;}
    double getPrice() const {return price;}
    string getAsJson() const;
    string read() const;
    /**** mutators ****/
    int setSize(int size);
    Side setSide(Side side);
    double setPrice(double price);
};

class MarketOrder : public Order {
private:
    int size;
    Side side;
public:
    /**** constructors ****/
    MarketOrder(){}; ~MarketOrder(){}
    MarketOrder(const MarketOrder& order);
    MarketOrder(int id, int time, string name, Side side, int size);
    MarketOrder* copy() const;
    /**** accessors ****/
    int getSize() const {return size;}
    Side getSide() const {return side;}
    string getAsJson() const;
    string read() const;
    /**** mutators ****/
    int setSize(int size);
    Side setSide(Side side);
};

class CancelOrder : public Order {
private:
    int idRef;
public:
    /**** constructors ****/
    CancelOrder(){}; ~CancelOrder();
    CancelOrder(const CancelOrder& order);
    CancelOrder(int id, int time, string name, int idRef);
    CancelOrder* copy() const;
    /**** accessors ****/
    int getIdRef() const {return idRef;}
    string getAsJson() const;
    string read() const;
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
    ModifyOrder(const ModifyOrder& order);
    ModifyOrder(int id, int time, string name, int idRef, const Order& newOrder);
    ModifyOrder* copy() const;
    /**** accessors ****/
    int getIdRef() const {return idRef;}
    Order* getNewOrder() const {return newOrder;}
    string getAsJson() const;
    string read() const;
    /**** mutators ****/
    int setIdRef(int idRef);
    Order* setNewOrder(const Order& newOrder);
    /**** operators ****/
    Order& operator=(const ModifyOrder& order);
};

class Trade {
private:
    Side side;
    double price;
    int size;
    Order* bookOrder;
    Order* matchOrder;
public:
    Trade(){}
    Trade(const Trade& trade);
    Trade(Side side, double price, int size, const Order& bookOrder, const Order& matchOrder);
    Trade* copy() const;
    string getAsJson() const;
    string read() const;
    Trade& operator=(const Trade& trade);
};

class LimitOrderBook {
private:
    string name;
    vector<Trade*> trades;
    map<double,vector<LimitOrder*>> bids, asks;
public:
    /**** constructors ****/
    LimitOrderBook(){}; ~LimitOrderBook();
    LimitOrderBook(string name);
    LimitOrderBook(const LimitOrderBook& book);
    LimitOrderBook* copy() const;
    /**** accessors ****/
    string getName() const {return name;}
    vector<Trade*> getTrades() const {return trades;}
    map<double,vector<LimitOrder*>> getBids() const {return bids;}
    map<double,vector<LimitOrder*>> getAsks() const {return asks;}
    double bestBid() const;
    double bestAsk() const;
    vector<double> getBidPrices() const;
    vector<double> getAskPrices() const;
    vector<LimitOrder*> getBidOrders(double price) const;
    vector<LimitOrder*> getAskOrders(double price) const;
    map<double,int> getBidDepth() const;
    map<double,int> getAskDepth() const;
    string getAsJson() const;
    string read() const;
    void printBook() const;
    /**** main ****/
    void process(const LimitOrder& order);
    void process(const MarketOrder& order);
    void process(const CancelOrder& order);
    void process(const ModifyOrder& order);
    /**** operators ****/
    Trade& operator=(const Trade& trade);
};

class LimitOrderBooks {
private:
    vector<LimitOrderBook*> books;
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

ostream& operator<<(ostream& out, Order* const order) {
    out << order->getAsJson();
    return out;
}

ostream& operator<<(ostream& out, const Order& order) {
    out << order.getAsJson();
    return out;
}

ostream& operator<<(ostream& out, const Trade& trade) {
    out << trade.getAsJson();
    return out;
}

/******************************************************************************/

Order::Order(const Order& order): id(order.id), time(order.time), name(order.name), type(order.type) {}

Order::Order(int id, int time, string name, OrderType type): id(id), time(time), name(name), type(type) {}

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

string Order::read() const {
    ostringstream oss;
    oss << getName() << " " << getType();
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

LimitOrder::LimitOrder(const LimitOrder& order): Order(order), side(order.side), price(order.price) {}

LimitOrder::LimitOrder(int id, int time, string name, Side side, int size, double price): Order(id, time, name, LIMIT), side(side), size(size), price(price) {}

LimitOrder* LimitOrder::copy() const {
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
    "\"size\":"   << getSize()  << "," <<
    "\"price\":"  << getPrice() <<
    "}";
    return oss.str();
}

string LimitOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " " << getSide() << " " << getSize() << "@" << getPrice();
    return oss.str();
}

int LimitOrder::setSize(int size) {
    this->size = size;
    return this->size;
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

MarketOrder::MarketOrder(const MarketOrder& order): Order(order), side(order.side) {}

MarketOrder::MarketOrder(int id, int time, string name, Side side, int size): Order(id, time, name, MARKET), side(side), size(size) {}

MarketOrder* MarketOrder::copy() const {
    return new MarketOrder(*this);
}

string MarketOrder::getAsJson() const {
    ostringstream oss;
    oss << "{" <<
    "\"id\":"     << getId()   << "," <<
    "\"time\":"   << getTime() << "," <<
    "\"name\":\"" << getName() << "\"," <<
    "\"type\":\"" << getType() << "\"," <<
    "\"side\":\"" << getSide() << "\"," <<
    "\"size\":"   << getSize()  <<
    "}";
    return oss.str();
}

string MarketOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " " << getSide() << " " << getSize();
    return oss.str();
}

int MarketOrder::setSize(int size) {
    this->size = size;
    return this->size;
}

Side MarketOrder::setSide(Side side) {
    this->side = side;
    return this->side;
}

/******************************************************************************/

ModifyOrder::ModifyOrder(const ModifyOrder& order): Order(order), idRef(order.idRef), newOrder(order.copy()) {}

ModifyOrder::ModifyOrder(int id, int time, string name, int idRef, const Order& newOrder): Order(id, time, name, MODIFY), idRef(idRef), newOrder(newOrder.copy()) {}

ModifyOrder::~ModifyOrder() {
    delete newOrder;
}

ModifyOrder* ModifyOrder::copy() const {
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

string ModifyOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " id: " << getIdRef() << " " << *getNewOrder();
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

Trade::Trade(const Trade& trade): side(trade.side), price(trade.price), size(trade.size), bookOrder(trade.bookOrder->copy()), matchOrder(trade.matchOrder->copy()) {}

Trade::Trade(Side side, double price, int size, const Order& bookOrder, const Order& matchOrder): side(side), price(price), size(size), bookOrder(bookOrder.copy()), matchOrder(matchOrder.copy()) {}

Trade* Trade::copy() const {
    return new Trade(*this);
}

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

string Trade::read() const {
    ostringstream oss;
    oss << "trade " << bookOrder->getName() << " " << side << " " << size << "@" << price;
    return oss.str();
}

/******************************************************************************/

LimitOrderBook::LimitOrderBook(string name): name(name) {}

LimitOrderBook::LimitOrderBook(const LimitOrderBook& book): name(book.name) {
    //
}

LimitOrderBook::~LimitOrderBook() {
    for (auto trade : trades) delete trade;
    for (auto bid : bids)
        for (auto order : bid.second) delete order;
    for (auto ask : asks)
        for (auto order : ask.second) delete order;
    trades.clear();
    bids.clear();
    asks.clear();
}

LimitOrderBook* LimitOrderBook::copy() const {
    return new LimitOrderBook(*this);
}

double LimitOrderBook::bestBid() const {
    vector<double> prices = getBidPrices();
    return (prices.size()>0)?prices[0]:0;
}

double LimitOrderBook::bestAsk() const {
    vector<double> prices = getAskPrices();
    return (prices.size()>0)?prices[0]:0;
}

vector<double> LimitOrderBook::getBidPrices() const {
    vector<double> prices;
    for (auto i=bids.begin(); i!=bids.end(); i++) prices.push_back(i->first);
    sort(prices.rbegin(), prices.rend());
    return prices;
}

vector<double> LimitOrderBook::getAskPrices() const {
    vector<double> prices;
    for (auto i=bids.begin(); i!=bids.end(); i++) prices.push_back(i->first);
    sort(prices.begin(), prices.end());
    return prices;
}

vector<LimitOrder*> LimitOrderBook::getBidOrders(double price) const {
    auto i = bids.find(price);
    if (i != bids.end()) {
        vector<LimitOrder*> orders;
        for (auto o : i->second) orders.push_back(o->copy());
        return orders;
    } else return {};
}

vector<LimitOrder*> LimitOrderBook::getAskOrders(double price) const {
    auto i = asks.find(price);
    if (i != asks.end()) {
        vector<LimitOrder*> orders;
        for (auto o : i->second) orders.push_back(o->copy());
        return orders;
    } else return {};
}

map<double,int> LimitOrderBook::getBidDepth() const {
    map<double,int> depth;
    for (auto b : bids) {
        int d = 0;
        for (auto o : b.second) d += o->getSize();
        depth[b.first] = d;
    }
    return depth;
}

map<double,int> LimitOrderBook::getAskDepth() const {
    map<double,int> depth;
    for (auto b : asks) {
        int d = 0;
        for (auto o : b.second) d += o->getSize();
        depth[b.first] = d;
    }
    return depth;
}

string LimitOrderBook::getAsJson() const {
    ostringstream oss;
    vector<double> b = getBidPrices();
    vector<double> a = getAskPrices();
    oss << "{";
    oss << "\"bids\":{";
    for (auto i=b.begin(); i!=b.end(); i++)
        oss << *i << ":" << bids.at(*i) << ((i==b.end()-1)?"":",");
    oss << "},";
    oss << "\"asks\":{";
    for (auto i=a.begin(); i!=a.end(); i++)
        oss << *i << ":" << asks.at(*i) << ((i==a.end()-1)?"":",");
    oss << "}";
    oss << "}";
    return oss.str();
}

// string LimitOrderBook::read() const {
//
// }

// void LimitOrderBook::printBook() const {
//
// }

#endif
