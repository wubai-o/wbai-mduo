#ifndef __STUDY_TIMESTAMP_H__
#define __STUDY_TIMESTAMP_H__

#include"copyable.h"
#include<boost/operators.hpp>
#include<stdint.h>
#include<string>

namespace study {

class Timestamp : public copyable,
                  public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp> {
public:
    Timestamp() 
        :microSecondsSinceEpoch_(0) {}

    explicit Timestamp(int64_t microSecondsSinceEpochArg) 
        : microSecondsSinceEpoch_(microSecondsSinceEpochArg) {
    }

    void swap(Timestamp& that) {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

    // std::string toString() const;
    std::string toFormattedString(bool showMicroseconds = true) const;
    bool valid() const {return microSecondsSinceEpoch_ > 0;}

    int64_t microSecondsSinceEpoch() const {return microSecondsSinceEpoch_;}

    static Timestamp now();
    static Timestamp invalid() {
        return Timestamp();
    }
    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t microSecondsSinceEpoch_;
};

inline Timestamp addTime(Timestamp timestamp, double seconds) {
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

inline bool operator<(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs) {
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

}




#endif