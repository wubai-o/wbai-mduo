#include"TimeZone.h"
#include"noncopyable.h"

#include<vector>
#include<stdio.h>
#include<stdexcept>


namespace study {

namespace detail {

struct Transition {
    time_t gmttime;
    time_t localtime;
    int localtimeIdx;

    Transition(time_t t, time_t l, int localIdx)
        :gmttime(t)
        ,localtime(l)
        ,localtimeIdx(localIdx) {}
};

struct Comp {
    bool compareGmt;

    Comp(bool gmt)
        :compareGmt(gmt) {}
    
    bool operator()(const Transition& lhs, const Transition& rhs) const {
        if(compareGmt)
            return lhs.gmttime < rhs.gmttime;
        else
            return lhs.localtime < rhs.localtime;
    }

    bool equal(const Transition& lhs, const Transition& rhs) const {
        if(compareGmt)
            return lhs.gmttime == rhs.gmttime;
        else
            return lhs.localtime == rhs.localtime;
    }
};

struct Localtime {
    time_t gmtOffset;
    bool isDst;
    int arrbIdx;

    Localtime(time_t offset, bool dst, int arrb)
        :gmtOffset(offset), isDst(dst), arrbIdx(arrb) {}
};

inline void fillHMS(unsigned seconds, struct tm* utc) {
    utc->tm_sec = seconds % 60;
    unsigned minutes = seconds / 60;
    utc->tm_min = minutes % 60;
    utc->tm_hour = minutes / 60;
}

} // namespace detail

} // namespace study

struct study::TimeZone::Data {
    std::vector<detail::Transition> transitions;
    std::vector<detail::Localtime> localtimes;
    std::vector<std::string> names;
    std::string abbreviation;
};

namespace study {
namespace detail {

class File : noncopyable {
public:
    File(const char* file)
        :fp_(::fopen(file, "rb")) {}

    ~File() {
        if(fp_) {
            ::fclose(fp_);
        }
    }

    bool valid() const {return fp_;}

    std::string readBytes(int n) {
        char buf[n];
        ssize_t nr = ::fread(buf, 1, n, fp_);
        if(nr != n) {
            throw std::logic_error("no enough data");
        }
        return std::string(buf, n);
    }

    int32_t readInt32() {
        int32_t x = 0;
        ssize_t nr = ::fread(&x, sizeof(int32_t), fp_);
        if(nr != sizeof(int32_t)) {
            throw std::logic_error("bad int32_t data");
        }
        return be32toh(x);
    }

    uint8_t readUInt8() {
        uint8_t x = 0;
        ssize_t nr = fread(&x, 1, sizeof(uint8_t), fp_);
        if(nr != sizeof(uint8_t)) {
            throw std::logic_error("bad uint8_t data");
        }
        return x;
    }

private:
    FILE* fp_;
};

bool readTimeZoneFile(const char* zonefile, struct TimeZone::Data* data) {
    File f(zonefile);
    if(f.valid()) {
        try {
            std::string head = f.readBytes(4);
            if(head != "TZif")
                throw std::logic_error("bad head");
            std::string version = f.readBytes(1);
            f.readBytes(15);
            int32_t isgmtcnt = f.readInt32();
            int32_t isstdcnt = f.readInt32();
            int32_t leapcnt = f.readInt32();
            int32_t timecnt = f.readInt32();
            int32_t typecnt = f.readInt32();
            int32_t charcnt = f.readInt32();

            std::vector<int32_t> trans;
            std::vector<int> localtimes;
            trans.reserve(timecnt);
            for(int i = 0; i < timecnt; ++i) {
                
            }
        } catch
    }

}


} // namespace detail
} // namespace study