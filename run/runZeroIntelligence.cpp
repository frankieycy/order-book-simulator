#include "zeroIntelligence.cpp"
using namespace std;

int main() {
    ZeroIntelligence zi(1e4,1000,30,1,50,0.2);
    zi.initOrderBook();
    zi.simulate();
    zi.printBook(30,10);
    return 0;
}
