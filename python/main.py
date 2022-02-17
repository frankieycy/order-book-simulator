import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from tqdm import tqdm
plt.switch_backend("Agg")

#### load data #################################################################

dataFolder = '../test/'
plotFolder = '../test/'
tradesFile='trades.csv'
depthsFile='depths.csv'
trades = pd.read_csv(dataFolder+tradesFile)
depths = pd.read_csv(dataFolder+depthsFile)

nSnp = len(depths)
nLvl = (len(depths.columns)-1)//2
prxCols = depths.columns[range(1,2*nLvl+1,2)].tolist()
sizCols = depths.columns[range(2,2*nLvl+1,2)].tolist()
TT = trades['TIME']
TP = trades['PRICE']
T = depths['TIME']
B = depths['BID1_PRICE']
A = depths['ASK1_PRICE']
Sb = depths['BID1_SIZE']
Sa = depths['ASK1_SIZE']
M = (A+B)/2
N = (A*Sb+B*Sa)/(Sa+Sb)
Q = (Sb-Sa)/(Sa+Sb)

#### visualization #############################################################

def plotPricePath(t0,t1,midOnly=False,tradeOnly=False):
    i = (depths['TIME']>=t0)&(depths['TIME']<=t1)
    j = (trades['TIME']>=t0)&(trades['TIME']<=t1)
    fig = plt.figure(figsize=(8,4))
    if midOnly:
        plt.step(T[i],M[i],c='0',lw=1,label='mid')
    elif tradeOnly:
        plt.step(TT[j],TP[j],c='k',label='trade')
    else:
        plt.step(T[i],B[i],c='b',ls='--',lw=0.5,zorder=0,label='bid')
        plt.step(T[i],A[i],c='r',ls='--',lw=0.5,zorder=0,label='ask')
        plt.step(T[i],M[i],c='0',lw=1,zorder=0,label='mid')
        plt.step(T[i],N[i],c='0.6',lw=0.5,zorder=0,label='micro')
        plt.scatter(TT[j],TP[j],c='k',s=10,zorder=1)
    plt.title('Zero-Intelligence Simulated Price Path')
    plt.xlabel('time')
    plt.ylabel('price')
    plt.xlim([t0,t1])
    plt.legend(loc='upper right')
    fig.tight_layout()
    plt.savefig(plotFolder+'price.png',dpi=150)
    plt.close()

def plotAvgBookShape(B,n=1):
    book = pd.DataFrame({b:[0] for b in range(-B,B+1)})
    for t,bk in tqdm(depths.iterrows()):
        if not t % n:
            prx = bk[prxCols].to_numpy()
            siz = bk[sizCols].to_numpy()
            m = (bk['BID1_PRICE']+bk['ASK1_PRICE'])//2
            prx = prx-m # relative to mid
            idx = (prx>=-B)&(prx<=B)
            prx = prx[idx]
            siz = siz[idx]
            book[prx] += siz/nSnp*n
    fig = plt.figure(figsize=(8,4))
    plt.scatter(book.columns,book.loc[0],c='k',s=20)
    plt.title('Zero-Intelligence Average Book Shape')
    plt.xlabel('relative price')
    plt.ylabel('shares')
    plt.xlim([-B,B])
    fig.tight_layout()
    plt.savefig(plotFolder+'book.png',dpi=150)
    plt.close()

#### main ######################################################################

if __name__ == '__main__':
    plotPricePath(50,150)
    plotAvgBookShape(40,50)
