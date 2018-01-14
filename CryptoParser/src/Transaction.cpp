#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include "History.h"

std::ostream& operator<<(std::ostream &, const Date &);

Transaction::Transaction() {
    id = "ID";
    date = Date();
    exchange = "EXCHANGE";
    order = "ORDER";
    pair = "PAIR";
    amount = "AMOUNT";
    price = "PRICE";
    cost = "COST";
    fee = "FEE";
    amountValue = "AMTVAL";
    alpha = "ALPHA";
}

Transaction::~Transaction() {

}

void Transaction::Print() {
    std::cout
        << std::setw(5) << std::left << id
        << date << "   "
        << std::setw(10) << std::left << exchange
        << std::setw(10) << std::left << order
        << std::setw(10) << std::left << pair
        << std::setw(18) << std::left << amount
        << std::setw(15) << std::left << price
        << std::setw(15) << std::left << cost
        << std::endl;
}

void Transaction::Export(std::ofstream &outFile, int i) {
    outFile
        << i + 1 << ","
        << date << ","
        << exchange << ","
        << order << ","
        << pair << ","
        << amount << ","
        << price << ","
        << cost << ","
        << "\n";
}

std::ostream& operator<<(std::ostream &os, const Date &date) {
    os << std::setw(4) << std::right << date.year
        << std::setw(1) << std::right << '-'
        << std::setw(2) << std::right << date.month
        << std::setw(1) << std::right << '-'
        << std::setw(2) << std::right << date.day
        << std::setw(1) << std::right << "   "
        << std::setw(2) << std::right << date.hour
        << std::setw(1) << std::right << ':'
        << std::setw(2) << std::right << date.min
        << std::setw(1) << std::right << ':'
        << std::setw(2) << std::right << date.sec;
    return os;
}
