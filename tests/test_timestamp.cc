#include"../study/base/Timestamp.h"
#include<iostream>

int main() {
    study::Timestamp timestamp(study::Timestamp::now());
    std::cout << "its " << timestamp.microSecondsSinceEpoch() <<
         " microseconds from Epoch" << std::endl;
    return 0;
}