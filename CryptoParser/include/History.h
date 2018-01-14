#pragma once

#include "stdafx.h"
#include <vector>
#include <string>

#include "Categories.h"

class Date {
public:
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short min;
    unsigned short sec;

public:
    Date();
    ~Date();
    friend bool operator<(const Date &lhs, const Date &rhs);
};

class Transaction {
public:
    std::string id;
    Date date;
    std::string exchange;
    std::string order;
    std::string pair;
    std::string amount;
    std::string price;
    std::string cost;
    std::string fee;
    std::string amountValue;
    std::string alpha;

public:
    Transaction();
    ~Transaction();
    void Print();
    void Export(std::ofstream &, int);
    friend std::ostream& operator<<(std::ostream &, const Date &);
};

class History {
protected:
    std::vector<Transaction> txs;

public:
    History();
    std::vector<Transaction>* GetTxs();
    void Insert(Transaction);
    void Export(std::string);

public:
    virtual void Sort(Categories::Category);
    virtual void Print(std::string);
};
