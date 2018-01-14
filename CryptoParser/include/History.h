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

public:
    Transaction();
    ~Transaction();
};

class History {
private:
    std::vector<Transaction> txs;

public:
    History();
    void Insert(Transaction);
    void Sort(Categories::Category);
    void Print();
    void Export();
    friend std::ostream& operator<<(std::ostream &, const Date &);
};
