#include "orderBook.cpp"
using namespace std;

int main() {
    Order* o1 = new LimitOrder(0,0,"AAPL",BID,3,10);
    Order* o2 = new MarketOrder(1,1,"AAPL",ASK,2);
    LimitOrderBook ob;
    cout << o1->read() << endl;
    cout << o2->read() << endl;
    cout << ob.getAsJson() << endl;
    return 0;
}
