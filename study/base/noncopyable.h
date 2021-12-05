#ifndef __STUDY_NONCOPYABLE_H__
#define __STUDY_NONCOPYABLE_H__

namespace study {

class noncopyable {
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
    
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};


}

#endif // __STUDY_NONCOPYABLE_H__