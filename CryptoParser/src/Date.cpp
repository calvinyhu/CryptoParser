#include "stdafx.h"
#include "History.h"

Date::Date() {
    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    min = 0;
    sec = 0;
}

Date::~Date() {

}

bool operator<(const Date &lhs, const Date &rhs) {

    if (lhs.year < rhs.year)
        return true;

    if (lhs.year == rhs.year && lhs.month < rhs.month)
        return true;

    if (lhs.year == rhs.year && lhs.month == rhs.month && lhs.day < rhs.day)
        return true;

    if (lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day && lhs.hour < rhs.hour)
        return true;

    if (lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day && lhs.hour == rhs.hour && lhs.min < rhs.min)
        return true;

    if (lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day && lhs.hour == rhs.hour && lhs.min == rhs.min && lhs.sec < rhs.sec)
        return true;

    return false;
}
