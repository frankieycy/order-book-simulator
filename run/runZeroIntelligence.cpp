#include "zeroIntelligence.cpp"
using namespace std;
using namespace chrono;

int main() {
    srand(0);
    /**** parameters **********************************************************/
    int n       = 1e4;
    int L       = 30;
    int LL      = 1000;
    int snpInt  = 1;
    int snpLvl  = 40;
    double lda  = 1;
    double mu   = 50;
    double nu   = 0.2;
    string dataFolder = "test/";
    /**** ZI simulation *******************************************************/
    ZeroIntelligence zi(n,LL,L,lda,mu,nu,snpInt,snpLvl);
    zi.initOrderBook();
    auto t1 = high_resolution_clock::now();
    zi.simulate();
    auto t2 = high_resolution_clock::now();
    zi.printBook(30,10);
    auto t = duration_cast<microseconds>(t2-t1);
    cout << "processing time per order: " << (float)t.count()/n << "μs" << endl;
    /**** outputs *************************************************************/
    zi.printTradesToJson(dataFolder+"trades.json");
    zi.printDepthsLogToJson(dataFolder+"depths.json");
    zi.printTradesToCsv(dataFolder+"trades.csv");
    zi.printDepthsLogToCsv(dataFolder+"depths.csv");
    return 0;
}
