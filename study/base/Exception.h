#ifndef __STUDY_EXCEPTION_H__
#define __STUDY_EXCEPTION_H__

#include<exception>
#include<string>

namespace study {
class Exception : public std::exception {
public:
    Exception(std::string what);
    ~Exception() noexcept = default;

    const char* what() const noexcept override {
        return message_.c_str();
    }

    const char* stackTrace() const noexcept {
        return stack_.c_str();
    }

private:
    std::string message_;
    std::string stack_;

};


}   // study

#endif // __STUDY_EXCEPTION_H__