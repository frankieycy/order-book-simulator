import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
plt.switch_backend("Agg")

dataFolder = '../test/'
plotFolder = '../plot/'

def main(t0,t1):
    trades = pd.read_csv(dataFolder+"trades.csv")
    depths = pd.read_csv(dataFolder+"depths.csv")
    TT = trades['TIME']
    TP = trades['PRICE']
    T = depths['TIME']
    B = depths['BID1_PRICE']
    A = depths['ASK1_PRICE']
    Sb = depths['BID1_SIZE']
    Sa = depths['ASK1_SIZE']
    mid = (A+B)/2
    mic = (A*Sb+B*Sa)/(Sa+Sb)
    t = (depths['TIME']>=t0)&(depths['TIME']<=t1)
    tt = (trades['TIME']>=t0)&(trades['TIME']<=t1)

    fig = plt.figure(figsize=(8,4))
    plt.plot(T[t],B[t],c='b',linestyle='--',linewidth=0.5,label='bid')
    plt.plot(T[t],A[t],c='r',linestyle='--',linewidth=0.5,label='ask')
    plt.plot(T[t],mid[t],c='0',linewidth=0.5,label='mid')
    plt.plot(T[t],mic[t],c='0.6',linewidth=0.5,label='micro')
    plt.scatter(TT[tt],TP[tt],c='k',s=10)
    plt.title('Zero-Intelligence Simulated Price Path')
    plt.xlabel('time')
    plt.ylabel('price')
    plt.xlim([t0,t1])
    plt.legend(loc='upper right')
    fig.tight_layout()
    plt.savefig(plotFolder+'price.png', dpi=150)
    plt.close()

if __name__ == '__main__':
    main(10000,10100)
