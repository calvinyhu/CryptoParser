// CryptoParser.cpp

#include "stdafx.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "History.h"
#include "CoinHistory.h"
#include "Categories.h"

namespace fs = std::experimental::filesystem;

#pragma region Binance Parsing

// Expecting yyyy-mm-ddhr:min:sec (UTC = PST + 8 hr)
Date ExtractBinanceDate(std::string cell) {
    std::istringstream is(cell);
    char delim;
    Date date;
    is >> date.year >> delim >> date.month >> delim >> date.day >> date.hour >> delim >> date.min >> delim >> date.sec;
    return date;
}

void parseBinanceFile(std::ifstream &infile, History &history) {
    std::string line;

    // Skip first line
    std::getline(infile, line);

    // Read the rest line by line
    while (std::getline(infile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        Transaction tx;
        Categories::Category nextCategory = Categories::Date;

        // Read the line cell by cell and construct transaction
        while (std::getline(lineStream, cell, ',')) {
            switch (nextCategory) {
            case Categories::ID:
                tx.id = cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Date:
                tx.date = ExtractBinanceDate(cell);
                nextCategory = Categories::Pair;
                break;
            case Categories::Exchange:
                tx.exchange = cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Order:
                tx.order = cell;
                nextCategory = Categories::Price;
                break;
            case Categories::Pair:
                tx.pair = cell;
                nextCategory = Categories::Order;
                break;
            case Categories::Amount:
                tx.amount = cell;
                nextCategory = Categories::Cost;
                break;
            case Categories::Price:
                tx.price = cell;
                nextCategory = Categories::Amount;
                break;
            case Categories::Cost:
                tx.cost = cell;
                nextCategory = Categories::Fee;
                break;
            case Categories::Fee:
                tx.fee = cell;
                nextCategory = Categories::FeeCoin;
                break;
            case Categories::FeeCoin:
                nextCategory = Categories::Invalid;
                break;
            default:
                break;
            }
        }

        // Adjustments
        if (tx.order != "SEND")
            tx.exchange = "Binance";

        if (tx.order == "BUY")
            tx.amount = std::to_string(std::stod(tx.amount) - std::stod(tx.fee));

        // Insert transaction into history
        history.Insert(tx);
    }
}

#pragma endregion

#pragma region Coinbase Parsing

void parseCoinbaseFile(std::ifstream &infile, History &history) {
    std::string line;

    // Skip first 4 lines
    for (int i = 0; i < 4; i++)
        std::getline(infile, line);

    // Extract pair left unit
    std::stringstream categories(line);
    std::string pairLeftUnit;
    for (int i = 0; i < 3; i++)
        std::getline(categories, pairLeftUnit, ',');

    // Read the rest line by line
    while (std::getline(infile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        Transaction tx;
        Categories::Category nextCategory = Categories::Date;

        // Read the line cell by cell and construct transaction
        while (std::getline(lineStream, cell, ',')) {
            switch (nextCategory) {
            case Categories::ID:
                //tx.id = cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Date:
                tx.date = ExtractBinanceDate(cell);
                nextCategory = Categories::Order;
                break;
            case Categories::Exchange:
                tx.exchange = cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Order:
                tx.order = cell;
                std::transform(tx.order.begin(), tx.order.end(), tx.order.begin(), ::toupper);
                nextCategory = Categories::Amount;
                break;
            case Categories::Pair:
                tx.pair = pairLeftUnit + cell;
                nextCategory = Categories::Price;
                break;
            case Categories::Amount:
                tx.amount = cell;
                nextCategory = Categories::Subtotal;
                break;
            case Categories::Price:
                tx.price = cell;
                nextCategory = Categories::PaymentMethod;
                break;
            case Categories::Cost:
                tx.cost = cell;
                nextCategory = Categories::Pair;
                break;
            case Categories::Fee:
                nextCategory = Categories::Cost;
                break;
            case Categories::FeeCoin:
                nextCategory = Categories::Invalid;
                break;
            case Categories::AmountUnit:
                nextCategory = Categories::Invalid;
                break;
            case Categories::Subtotal:
                nextCategory = Categories::Fee;
                break;
            case Categories::PaymentMethod:
                nextCategory = Categories::ID;
                break;
            default:
                break;
            }
        }

        // Insert transaction into history
        tx.exchange = "Coinbase";
        history.Insert(tx);
    }
}

#pragma endregion

#pragma region GDAX Parsing

// Expecting yyyy-mm-ddThr:min:sec.091Z
Date ExtractGdaxDate(std::string cell) {
    std::istringstream is(cell);
    char delim;
    Date date;
    is >> date.year >> delim >> date.month >> delim >> date.day >> delim >> date.hour >> delim >> date.min >> delim >> date.sec;
    return date;
}

std::string ExtractGdaxPair(std::string cell) {
    auto it = find(cell.begin(), cell.end(), '-');
    cell.erase(it);
    return cell;
}

void parseGdaxFile(std::ifstream &infile, History &history) {
    std::string line;

    // Skip first line
    std::getline(infile, line);

    // Read the rest line by line
    while (std::getline(infile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        Transaction tx;
        Categories::Category nextCategory = Categories::ID;

        // Read the line cell by cell and construct transaction
        while (std::getline(lineStream, cell, ',')) {
            switch (nextCategory) {
            case Categories::ID:
                //tx.id = cell;
                nextCategory = Categories::Pair;
                break;
            case Categories::Date:
                tx.date = ExtractGdaxDate(cell);
                nextCategory = Categories::Amount;
                break;
            case Categories::Exchange:
                tx.exchange = cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Order:
                tx.order = cell;
                nextCategory = Categories::Date;
                break;
            case Categories::Pair:
                tx.pair = ExtractGdaxPair(cell);
                nextCategory = Categories::Order;
                break;
            case Categories::Amount:
                tx.amount = cell;
                nextCategory = Categories::AmountUnit;
                break;
            case Categories::Price:
                tx.price = cell;
                nextCategory = Categories::Fee;
                break;
            case Categories::Cost:
                tx.cost = (cell[0] == '-') ? std::to_string(abs(std::stod(cell))) : cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Fee:
                nextCategory = Categories::Cost;
                break;
            case Categories::FeeCoin:
                nextCategory = Categories::Invalid;
                break;
            case Categories::AmountUnit:
                nextCategory = Categories::Price;
                break;
            default:
                break;
            }
        }

        // Insert transaction into history
        tx.exchange = "GDAX";
        history.Insert(tx);
    }
}

#pragma endregion

#pragma region Kraken Parsing

// Expecting yyyy-mm-ddThr:min:sec.091Z
Date ExtractKrakenDate(std::string cell) {
    std::istringstream is(cell);
    char delim;
    Date date;
    is >> date.year >> delim >> date.month >> delim >> date.day >> delim >> date.hour >> delim >> date.min >> delim >> date.sec;
    return date;
}

std::string ExtractKrakenPair(std::string cell) {
    auto it = find(cell.begin(), cell.end(), '-');
    cell.erase(it);
    return cell;
}

void parseKrakenFile(std::ifstream &infile, History &history) {
    std::string line;

    // Skip first line
    std::getline(infile, line);

    // Read the rest line by line
    while (std::getline(infile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        Transaction tx;
        Categories::Category nextCategory = Categories::ID;

        // Read the line cell by cell and construct transaction
        while (std::getline(lineStream, cell, ',')) {
            switch (nextCategory) {
            case Categories::ID:
                //tx.id = cell;
                nextCategory = Categories::Pair;
                break;
            case Categories::Date:
                tx.date = ExtractKrakenDate(cell);
                nextCategory = Categories::Amount;
                break;
            case Categories::Exchange:
                tx.exchange = cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Order:
                tx.order = cell;
                nextCategory = Categories::Date;
                break;
            case Categories::Pair:
                tx.pair = ExtractKrakenPair(cell);
                nextCategory = Categories::Order;
                break;
            case Categories::Amount:
                tx.amount = cell;
                nextCategory = Categories::AmountUnit;
                break;
            case Categories::Price:
                tx.price = cell;
                nextCategory = Categories::Fee;
                break;
            case Categories::Cost:
                tx.cost = (cell[0] == '-') ? std::to_string(abs(std::stod(cell))) : cell;
                nextCategory = Categories::Invalid;
                break;
            case Categories::Fee:
                nextCategory = Categories::Cost;
                break;
            case Categories::FeeCoin:
                nextCategory = Categories::Invalid;
                break;
            case Categories::AmountUnit:
                nextCategory = Categories::Price;
                break;
            default:
                break;
            }
        }

        // Insert transaction into history
        tx.exchange = "Kraken";
        history.Insert(tx);
    }
}

#pragma endregion

void parseFile(std::string file, std::string folder, History &history) {
    std::ifstream infile(file);

    if (folder == "resource\\binance") {
        parseBinanceFile(infile, history);
    }
    else if (folder == "resource\\coinbase") {
        parseCoinbaseFile(infile, history);
    }
    else if (folder == "resource\\gdax") {
        parseGdaxFile(infile, history);
    }
    else if (folder == "resource\\kraken") {
        parseKrakenFile(infile, history);
    }
}

int main() {
    History history;
    std::string path = "resource";

    // For each folder in the path
    for (auto &folder : fs::directory_iterator(path)) {
        std::ostringstream os1;
        os1 << folder;

        if (os1.str() == "resource\\live") continue;

        // For each file in the folder
        for (auto &file : fs::directory_iterator(folder)) {
            std::ostringstream os2;
            os2 << file;

            parseFile(os2.str(), os1.str(), history);
        }
    }

    // Sort complete history
    history.Sort(Categories::Date);
    //history.Sort(Categories::Pair);
    history.Print("Porfolio");

    // Individual coin history and stats
    std::string coin = "XRP";
    CoinHistory xrp(coin, &history);
    xrp.Print(coin);

    // Exporting
    history.Export("Porfolio History.csv");
    xrp.Export("XRP History.csv");
    return 0;
}
