#include "orderBook.cpp"
using namespace std;

int main() {
    int id = 0;
    vector<Order*> orders(10);
    for (int i=0; i<orders.size(); i++) {
        Order* tmp = new LimitOrder(0,0,"AAPL",BID,100);
        orders[i] = new ModifyOrder(id++,0,"AAPL",0,*tmp);;
        cout << orders[i]->getAsJson() << endl;
    }
    return 0;
}
