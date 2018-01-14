#pragma once

#include "stdafx.h"
#include "History.h"

// Statistics of a specific coin on a specific transaction
typedef struct Statistics {
    std::string balance;
    std::string balanceValue;
} Stats_t;

class CoinHistory : public History {
private:
    std::string coinName;
    std::vector<Stats_t> coinStats;

public:
    CoinHistory();
    CoinHistory(std::string, History*);
    void Insert(Stats_t);
};
