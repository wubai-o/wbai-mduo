#ifndef __STUDY_TYPES_H__
#define __STUDY_TYPES_H__

#include<stdint.h>
#include<string.h>  // memset
#include<string>

namespace study {

//using std::string;

inline void memZero(void* p, size_t n) {
    memset(p, 0, n);
}

template<typename To, typename From>
inline To implicit_cast(From const &f) {
    return f;
}


}


#endif // __STUDY_TYPES_H__