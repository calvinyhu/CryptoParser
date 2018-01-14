#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include "CoinHistory.h"

CoinHistory::CoinHistory() : coinName("COINNAME") {

}

CoinHistory::CoinHistory(std::string coinName, History *history) : coinName(coinName) {
    for (Transaction tx : *(history->GetTxs())) {
        if (tx.pair.substr(0,coinName.length()) == coinName) {
            txs.push_back(tx);
        }
    }
}

void CoinHistory::Insert(Stats_t stats) {
    coinStats.push_back(stats);
}
