#include "zeroIntelligence.cpp"
using namespace std;
using namespace chrono;

int main() {
    int n = 1e4;
    ZeroIntelligence zi(n,1000,30,1,50,0.2);
    zi.initOrderBook();
    auto t1 = high_resolution_clock::now();
    zi.simulate();
    auto t2 = high_resolution_clock::now();
    zi.printBook(30,10);
    auto t = duration_cast<microseconds>(t2-t1);
    cout << "processing time per order: " << (float)t.count()/n << "μs" << endl;
    // cout << *zi.getBidDepthsLogPtr() << endl;
    // cout << *zi.getAskDepthsLogPtr() << endl;
    return 0;
}
