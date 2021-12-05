#include"../study/base/Date.h"
#include<iostream>

int main() {
    study::Date date(2021, 11, 27);
    std::cout << "julianday:" << date.julianDayNumber() << std::endl;
    std::cout << "toIsoString:" << date.toIsoString() << std::endl;
    struct study::Date::YearMonthDay ydm(date.yearMonthDay());
    std::cout << "year:" << ydm.year << " month:" << ydm.month << " day:" << ydm.day << std::endl;
    return 0;
}