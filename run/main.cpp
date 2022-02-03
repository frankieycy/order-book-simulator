#include "orderBook.cpp"
using namespace std;

int main() {
    int id = 0;
    LimitOrderBook ob;
    for (int i=0; i<20; i++) {
        Side side = (uniformRand()<.5)?BID:ASK;
        int size = (int)uniformRand(5,20);
        double price = (int)((side==BID)?uniformRand(70,105):uniformRand(95,130));
        if (uniformRand()<.7) {
            LimitOrder* o = new LimitOrder(id++,0,"TEST",side,size,price);
            ob.process(*o);
            cout << "submitted: " << o << endl;
        } else {
            MarketOrder* o = new MarketOrder(id++,0,"TEST",side,size);
            ob.process(*o);
            cout << "submitted: " << o << endl;
        }
        ob.printBook(0,5,false);
        cout << "Bid Mkt: " << ob.getBidMktQueue() << endl;
        cout << "Ask Mkt: " << ob.getAskMktQueue() << endl;
        cout << endl;
    }
    // ob.printBook(0,5);
    return 0;
}
