#ifndef ORDERBOOK_CPP
#define ORDERBOOK_CPP
#include "util.cpp"
using namespace std;

enum Side       {NULL_SIDE, BID, ASK};
enum OrderType  {NULL_ORD, LIMIT, MARKET, CANCEL, MODIFY};

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
    int reduceSize(int size);
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
    CancelOrder(){}; ~CancelOrder(){};
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
    double topBid, topAsk;
    deque<Trade*> trades;
    deque<double> bidPrices, askPrices;
    deque<MarketOrder*> mktQueue;
    map<double,deque<LimitOrder*>> bids, asks;
public:
    /**** constructors ****/
    LimitOrderBook(){}; ~LimitOrderBook();
    LimitOrderBook(string name);
    LimitOrderBook(const LimitOrderBook& book);
    LimitOrderBook* copy() const;
    /**** accessors ****/
    string getName() const {return name;}
    deque<Trade*> getTrades() const {return trades;}
    map<double,deque<LimitOrder*>> getBids() const {return bids;}
    map<double,deque<LimitOrder*>> getAsks() const {return asks;}
    double getTopBid() const {return topBid;}
    double getTopAsk() const {return topAsk;}
    deque<double> getBidPrices() const {return bidPrices;}
    deque<double> getAskPrices() const {return askPrices;}
    deque<LimitOrder*> getBidOrders(double price) const;
    deque<LimitOrder*> getAskOrders(double price) const;
    map<double,int> getBidDepth() const;
    map<double,int> getAskDepth() const;
    double getBidDepthAt(double price) const;
    double getAskDepthAt(double price) const;
    string getAsJson() const;
    string read() const;
    void printBook(int bookLevels=0, int tradeLevels=0, bool summarizeDepth=true) const;
    /**** main ****/
    double updateTopBid();
    double updateTopAsk();
    deque<double> updateBidPrices();
    deque<double> updateAskPrices();
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
    out << order->getAsJson();
    return out;
}

ostream& operator<<(ostream& out, const Order& order) {
    out << order.getAsJson();
    return out;
}

ostream& operator<<(ostream& out, Trade* const trade) {
    out << trade->getAsJson();
    return out;
}

ostream& operator<<(ostream& out, const Trade& trade) {
    out << trade.getAsJson();
    return out;
}

bool match(Side side, double limit, double price) {
    if (side == BID) return price <= limit;
    else if (side == ASK) return price >= limit;
    return false;
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

LimitOrder::LimitOrder(const LimitOrder& order): Order(order), side(order.side), size(order.size), price(order.price) {}

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
    oss << getName() << " " << getType() << " " << getSide() << " " << getSize() << " @ $" << getPrice();
    return oss.str();
}

int LimitOrder::setSize(int size) {
    this->size = size;
    return this->size;
}

int LimitOrder::reduceSize(int size) {
    this->size -= size;
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

CancelOrder::CancelOrder(const CancelOrder& order): Order(order), idRef(order.idRef) {}

CancelOrder::CancelOrder(int id, int time, string name, int idRef): Order(id, time, name, CANCEL), idRef(idRef) {}

CancelOrder* CancelOrder::copy() const {
    return new CancelOrder(*this);
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

string CancelOrder::read() const {
    ostringstream oss;
    oss << getName() << " " << getType() << " id: " << getIdRef();
    return oss.str();
}

int CancelOrder::setIdRef(int idRef) {
    this->idRef = idRef;
    return this->idRef;
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
    "\"side\":\""     << side                    << "\"," <<
    "\"price\":"      << price                   << "," <<
    "\"size\":"       << size                    << "," <<
    "\"bookOrder\":"  << bookOrder->getAsJson()  << "," <<
    "\"matchOrder\":" << matchOrder->getAsJson() <<
    "}";
    return oss.str();
}

string Trade::read() const {
    ostringstream oss;
    oss << "trade " << bookOrder->getName() << " " << side << " " << size << " @ $" << price;
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

double LimitOrderBook::getBidDepthAt(double price) const {
    int d = 0;
    auto i = bids.find(price);
    for (auto o : i->second) d += o->getSize();
    return d;
}

double LimitOrderBook::getAskDepthAt(double price) const {
    int d = 0;
    auto i = asks.find(price);
    for (auto o : i->second) d += o->getSize();
    return d;
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

// string LimitOrderBook::read() const {
//
// }

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
        cout << "-------------------------------------------" << endl;
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
    sort(bidPrices.rbegin(), bidPrices.rend());
    return bidPrices;
}

deque<double> LimitOrderBook::updateAskPrices() {
    askPrices.clear();
    for (auto i=asks.begin(); i!=asks.end(); i++) askPrices.push_back(i->first);
    sort(askPrices.begin(), askPrices.end());
    return askPrices;
}

void LimitOrderBook::process(const LimitOrder& order) {
    Side side = order.getSide();
    double limit = order.getPrice();
    int unfilledSize = order.getSize();
    deque<double>* oppPrices;
    map<double,deque<LimitOrder*>>* sameSide;
    map<double,deque<LimitOrder*>>* oppSide;
    if (side == BID) {sameSide = &bids; oppSide = &asks; oppPrices = &askPrices;}
    else if (side == ASK) {sameSide = &asks; oppSide = &bids; oppPrices = &bidPrices;}
    while (unfilledSize && oppPrices->size() && match(side, limit, oppPrices->front())) {
        deque<LimitOrder*>* orders = &oppSide->at(oppPrices->front());
        while (unfilledSize && orders->size()) {
            int matchedSize = min(unfilledSize, orders->front()->getSize());
            Trade* trade = new Trade(side, orders->front()->getPrice(), matchedSize, *orders->front(), order);
            trades.push_back(trade);
            unfilledSize -= matchedSize;
            orders->front()->reduceSize(matchedSize);
            if (orders->front()->getSize() == 0) orders->pop_front();
        }
        if (orders->size() == 0) {
            oppSide->erase(oppPrices->front());
            oppPrices->pop_front();
        }
    }
    if (unfilledSize) {
        LimitOrder* updatedOrder = new LimitOrder(order);
        updatedOrder->setSize(unfilledSize);
        (*sameSide)[limit].push_back(updatedOrder);
    }
    updateBidPrices();
    updateAskPrices();
    updateTopBid();
    updateTopAsk();
}

void LimitOrderBook::process(const MarketOrder& order) {

}

void LimitOrderBook::process(const CancelOrder& order) {

}

void LimitOrderBook::process(const ModifyOrder& order) {

}

#endif
