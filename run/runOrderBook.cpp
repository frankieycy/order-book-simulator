#include "orderBook.cpp"
using namespace std;
using namespace chrono;

void runNaive(int n, bool showProcess=false, bool showFinalBook=false) {
    int id = 0;
    LimitOrderBook ob;
    for (int i=0; i<n; i++) {
        Side side = (uniformRand()<0.5)?BID:ASK;
        int size = (int)uniformRand(5,20);
        int ccl = (int)uniformRand(0,id);
        double u = uniformRand();
        double price = (int)((side==BID)?uniformRand(70,105):uniformRand(95,130));
        Order* o;
        /**** Limit & Market Order ********************************************/
        // if (u < .6) o = new LimitOrder(id++,0,"TEST",side,size,price);
        // else o = new MarketOrder(id++,0,"TEST",side,size);
        /**** Limit, Market & Cancel Order ************************************/
        if (u<0.6) o = new LimitOrder(id++,0,"TEST",side,size,price);
        else if (u<0.8) o = new MarketOrder(id++,0,"TEST",side,size);
        else o = new CancelOrder(id++,0,"TEST",ccl);
        ob.processOrder(*o);
        if (showProcess) {
            cout << "submitted: " << o << endl;
            ob.printBook(0,5,false);
            cout << "Top: "
                 << "Bid " << ob.getBidDepthAt(ob.getTopBid()) << " @ $" << ob.getTopBid() << " "
                 << "Ask " << ob.getAskDepthAt(ob.getTopAsk()) << " @ $" << ob.getTopAsk() << endl;
            cout << "Mkt Bid: " << ob.getBidMktQueue() << endl;
            cout << "Mkt Ask: " << ob.getAskMktQueue() << endl;
            cout << endl << endl;
        }
    }
    if (showFinalBook) ob.printBook(0,5);
}

int main() {
    srand(0);
    /**** single runNaive *****************************************************/
    int n = 100;
    auto t1 = high_resolution_clock::now();
    runNaive(n,true,true);
    auto t2 = high_resolution_clock::now();
    auto t = duration_cast<microseconds>(t2-t1);
    cout << "processing time per order: " << (float)t.count()/n << "μs" << endl;
    /**** speed test **********************************************************/
    // for (int m=10; m<25; m++) {
    //     int n = pow(2,m);
    //     auto t1 = high_resolution_clock::now();
    //     runNaive(n,false,false);
    //     auto t2 = high_resolution_clock::now();
    //     auto t = duration_cast<microseconds>(t2-t1);
    //     cout << "(m = " << m << ") processing time per order: " << (float)t.count()/n << "μs" << endl;
    // }
    return 0;
}
