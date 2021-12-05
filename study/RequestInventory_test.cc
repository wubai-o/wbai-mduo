
#include<boost/shared_ptr.hpp>
#include<set>
#include"my_mutex.h"

namespace study {

class Request;

class Inventory {
public:
    typedef boost::shared_ptr<Request> RequestPtr;
    
    Inventory() 
        :requests_(new RequestListPtr){}
    
    void add(RequestPtr& req) {
        MutexLockGuard lock(mutex_);
        if(!requests_.unique()) {
            requests_.reset(new RequestsPtr(*requests_));
        }
        assert(requests_.unique());
        requests_.push_back(req);
    }
    
    void remove(Request& req) {
        MutexLockGuard lock(mutex_);
        if(!requests_.unique()) {
            requests_.reset(new RequestsPtr(*requests_));
        }
        assert(requests_.unque());
        requests_.erase(req);
    }
    
    void getData() const {
        MutexLockGuard lock(mutex_);
        return requests_;
    }
    
    void printAll() const {
        RequestListPtr requests = getData();
        for(RequestList::const_iterator it = requests->begin(); it != requests->end(); ++it) {
            (*it)->print();
        }
    }
private:
    typedef std::set<RequestPtr> RequestList;
    typedef boost::shared_ptr<RequestList> RequestListPtr;

    mutable MutexLock mutex_;
    RequestListPtr requests_;
};

}