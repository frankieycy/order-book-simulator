#include "zeroIntelligence.cpp"
using namespace std;
using namespace chrono;

int main() {
    /**** parameters **********************************************************/
    int n       = 1e4;
    int L       = 30;
    int LL      = 1000;
    int snpInt  = 1;
    int snpLvl  = 5;
    double lda  = 1;
    double mu   = 50;
    double nu   = 0.2;
    /**** ZI simulation *******************************************************/
    ZeroIntelligence zi(n,LL,L,lda,mu,nu,snpInt,snpLvl);
    zi.initOrderBook();
    auto t1 = high_resolution_clock::now();
    zi.simulate();
    auto t2 = high_resolution_clock::now();
    zi.printBook(30,10);
    auto t = duration_cast<microseconds>(t2-t1);
    cout << "processing time per order: " << (float)t.count()/n << "μs" << endl;
    zi.printTradesToCsv("test_trades.csv");
    zi.printDepthsLogToCsv("test_depths.csv");
    return 0;
}
