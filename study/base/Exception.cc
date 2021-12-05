#include"Exception.h"


namespace study {
    Exception::Exception(std::string msg)
        :message_(msg)
        ,stack_(CurrentThread::stackTrace(false)) {}
}