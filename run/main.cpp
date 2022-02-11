#include "orderBook.cpp"
using namespace std;
using namespace chrono;

int main() {
    int id = 0;
    int n = 1000000;
    LimitOrderBook ob;
    auto t1 = high_resolution_clock::now();
    for (int i=0; i<n; i++) {
        Side side = (uniformRand()<.5)?BID:ASK;
        int size = (int)uniformRand(5,20);
        int ccl = (int)uniformRand(0,id);
        double u = uniformRand();
        double price = (int)((side==BID)?uniformRand(70,105):uniformRand(95,130));
        Order* o;
        // if (u < .6) o = new LimitOrder(id++,0,"TEST",side,size,price);
        // else o = new MarketOrder(id++,0,"TEST",side,size);
        if (u < .6) o = new LimitOrder(id++,0,"TEST",side,size,price);
        else if (u < 0.8) o = new MarketOrder(id++,0,"TEST",side,size);
        else o = new CancelOrder(id++,0,"TEST",ccl);
        ob.processOrder(*o);
        // cout << "submitted: " << o << endl;
        // ob.printBook(0,5,false);
        // cout << "Bid Mkt: " << ob.getBidMktQueue() << endl;
        // cout << "Ask Mkt: " << ob.getAskMktQueue() << endl;
        // cout << endl << endl;
    }
    auto t2 = high_resolution_clock::now();
    auto t = duration_cast<microseconds>(t2-t1);
    cout << "processing time per order: " << (float)t.count()/n << "μs" << endl;
    // ob.printBook(0,5);
    return 0;
}
