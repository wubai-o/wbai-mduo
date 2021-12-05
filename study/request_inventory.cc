#include"my_mutex.h"
#include<set>
#include<boost/shared_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>
#include<stdio.h>


namespace study {

class Request;
typedef boost::shared_ptr<Request> RequestPtr;


class Inventory {
public:
    Inventory()
        :requests_(new RequestList) {}

    void add(const Request& req) {
        MutexLockGuard lock(mutex_);
        if(!requests_.unique()) {
            requests_.reset(new RequestList(*requests_));
            printf("Inventory::add() copy the whole list\n");
        }
        assert(requests_.unique());
        requests_->insert(req);
    }

    void remove(const Request& req) {
        MutexLockGuard lock(mutex_);
        if(!requests_.unique()) {
            requests_.reset(new RequestList(*requests_));
            printf("Inventory::remove() copy the whole list\n");
        }
        assert(requests_.unique());
        requests_->erase(req);
    }

    void printAll();
private:
    typedef std::set<boost::shared_ptr<Request> > RequestList;
    typedef boost::shared_ptr<RequestList> RequestListPtr;
    
    RequestListPtr getData() {
        MutexLockGuard lock(mutex_);
        return requests_;
    }

    MutexLock mutex_;
    RequestListPtr requests_;

};

Inventory g_inventory;

class Request : public boost::enable_shared_from_this<Request> {
public:
    Request() 
        :x_(0) {}

    ~Request() {
        x_ = -1;
    }

    void cancel() {
        MutexLockGuard lock(mutex_);
        x_ = 1;
        sleep(1);
        printf("cancel()\n");
        g_inventory.remove(shared_from_this());
    }

    void process() {
        MutexLockGuard lock(mutex_);
        g_inventory.add(shared_from_this());
        // ...
    }

    void print() {
        MutexLockGuard lock(mutex_);
        // ...
        printf("print Request %p x=%d\n", this, x_);
    }
private:
    mutable MutexLock mutex_;
    int x_;
};

void Inventory::printAll() const {
    MutexLockGuard lock(mutex_);
    for(std::set<RequestPtr>::const_iterator it = requests_.begin(); it != requests_.end(); ++it) {
        (*it)->print();
    }
}

}


void ThreadFunc() {

}

int main() {
    Thread thread(ThreadFunc);
    
}