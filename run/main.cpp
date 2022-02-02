#include "orderBook.cpp"
using namespace std;

int main() {
    int id = 0;
    LimitOrderBook ob;
    for (int i=0; i<20; i++) {
        Side side = (uniformRand()<.5)?BID:ASK;
        int size = (int)uniformRand(5,20);
        double price = (int)uniformRand(80,120);
        LimitOrder* o = new LimitOrder(id++,0,"TEST",side,size,price);
        cout << "submitted: " << o << endl;
        ob.process(*o);
        ob.printBook(0,0,false);
        cout << endl;
    }
    // ob.printBook(0,5);
    return 0;
}
