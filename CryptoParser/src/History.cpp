#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "History.h"

struct cmpId {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (tx1.id < tx2.id);
    }
};

struct cmpDate {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (tx1.date < tx2.date);
    }
};

struct cmpExchange {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (tx1.exchange < tx2.exchange);
    }
};

struct cmpOrder {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (tx1.order < tx2.order);
    }
};

struct cmpPair {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (tx1.pair < tx2.pair);
    }
};

struct cmpAmount {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (std::stod(tx1.amount) < std::stod(tx2.amount));
    }
};

struct cmpPrice {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (std::stod(tx1.price) < std::stod(tx2.price));
    }
};

struct cmpCost {
    inline bool operator() (const Transaction &tx1, const Transaction &tx2) {
        return (std::stod(tx1.cost) < std::stod(tx2.cost));
    }
};

History::History() {

}

std::vector<Transaction>* History::GetTxs()
{
    return &txs;
}

void History::Insert(Transaction tx) {
    txs.push_back(tx);
}

void History::Sort(Categories::Category category) {
    switch (category) {
    case Categories::ID:
        std::sort(txs.begin(), txs.end(), cmpId());
        break;
    case Categories::Date:
        std::sort(txs.begin(), txs.end(), cmpDate());
        break;
    case Categories::Exchange:
        std::sort(txs.begin(), txs.end(), cmpExchange());
        break;
    case Categories::Order:
        std::sort(txs.begin(), txs.end(), cmpOrder());
        break;
    case Categories::Pair:
        std::sort(txs.begin(), txs.end(), cmpPair());
        break;
    case Categories::Amount:
        std::sort(txs.begin(), txs.end(), cmpAmount());
        break;
    case Categories::Price:
        std::sort(txs.begin(), txs.end(), cmpPrice());
        break;
    case Categories::Cost:
        std::sort(txs.begin(), txs.end(), cmpCost());
        break;
    default:
        std::cout << "Error: Not a valid category" << std::endl;
        break;
    }
}

void History::Print(std::string type) {
    if (!type.empty())
        std::cout << type << " History" << std::endl;

    std::cout
        << std::setw(5) << std::left << "ID"
        << std::setw(24) << std::left << "Date"
        << std::setw(10) << std::left << "Exchange"
        << std::setw(10) << std::left << "Order"
        << std::setw(10) << std::left << "Pair"
        << std::setw(18) << std::left << "Amt Rcvd/Sold"
        << std::setw(15) << std::left << "Price [R]"
        << std::setw(15) << std::left << "Cost [R]"
        << '\n' << std::endl;

    for (Transaction tx : txs)
        tx.Print();

    std::cout << '\n';
}

void History::Export(std::string fileName) {
    std::ofstream outFile(fileName);

    outFile << "Id,Date,Exchange,Order,Pair,Amt Rcvd/Sold,Price [R],Cost [R]\n\n";

    for (int i = 0; i < txs.size(); i++)
        txs[i].Export(outFile, i);

    outFile.close();
}
