#include "stdafx.h"
#include "History.h"

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
}

Transaction::~Transaction() {

}
