#include"LogStream.h"

#include<algorithm>
#include<type_traits>
#include<limits>
#include<assert.h>


namespace study {
namespace detail {

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

static_assert(sizeof(digits) == 20, "wrong number of digits");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof(digitsHex) == 17, "wrong number of digitsHex");

template<typename T>
size_t convert(char buf[], T value) {
    T i = value;
    char* p = buf;
    do {
        int lsd =  static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while(i != 0);

    if(value < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

size_t convertHex(char buf[], uintptr_t value) {
    uintptr_t i = value;
    char* p = buf;

    do {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        *p++ = digitsHex[lsd];
    } while(i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;

} // namespace detail

} // namespace study


void study::LogStream::staticCheck() {
    static_assert(kMaxNumericeSize - 10 > std::numeric_limits<double>::digits10, 
                  "kMaxNumericSize is large enough");
    static_assert(kMaxNumericeSize - 10 > std::numeric_limits<long double>::digits10, 
                  "kMaxNumericSize is large enough");
    static_assert(kMaxNumericeSize - 10 > std::numeric_limits<long>::digits10, 
                  "kMaxNumericSize is large enough");
    static_assert(kMaxNumericeSize - 10 > std::numeric_limits<long long>::digits10, 
                  "kMaxNumericSize is large enough");
}

template<typename T>
void study::LogStream::formatInteger(T v) {
    if(buffer_.avail() >= kMaxNumericeSize) {
        size_t len = study::detail::convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

study::LogStream& study::LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(int v) {
    formatInteger(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(unsigned int v) {
    formatInteger(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(long v) {
    formatInteger(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(unsigned long v) {
    formatInteger(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(long long v) {
    formatInteger(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(unsigned long long v) {
    formatInteger(v);
    return *this;
}

study::LogStream& study::LogStream::operator<<(const void* p) {
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if(buffer_.avail() >= kMaxNumericeSize) {
        char* buf = buffer_.current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = study::detail::convertHex(buf + 2, v);
        buffer_.add(len + 2);
    }   
    return *this;
}

study::LogStream& study::LogStream::operator<<(double v) {
    if(buffer_.avail() >= kMaxNumericeSize) {
        int len = snprintf(buffer_.current(), kMaxNumericeSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}

template<typename T>
study::Fmt::Fmt(const char* fmt, T val) {
    length_ = snprintf(buf_, sizeof(buf_), fmt, val);
    assert(static_cast<size_t>(length_) < sizeof(buf_));
}

template study::Fmt::Fmt(const char* fmt, char);

template study::Fmt::Fmt(const char* fmt, short);
template study::Fmt::Fmt(const char* fmt, unsigned short);
template study::Fmt::Fmt(const char* fmt, int);
template study::Fmt::Fmt(const char* fmt, unsigned int);
template study::Fmt::Fmt(const char* fmt, long);
template study::Fmt::Fmt(const char* fmt, unsigned long);
template study::Fmt::Fmt(const char* fmt, long long);
template study::Fmt::Fmt(const char* fmt, unsigned long long);

template study::Fmt::Fmt(const char* fmt, float);
template study::Fmt::Fmt(const char* fmt, double);