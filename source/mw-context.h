
#ifndef __MW_CONTEXT_H__
#define __MW_CONTEXT_H__

#include <string>
#include "mw-event.h"

namespace mw {

class ProxyHolder;
class WorkerHolder;
class HandlerHolder;
class ProcHolder;
class Context {
public:
    Context();
    virtual ~Context();
public:
    ProxyHolder& proxy() {
        return *this->mProxyHolder;
    }
    WorkerHolder& worker() {
        return *this->mWorkerHolder;
    }
    HandlerHolder& event() {
        return *this->mHandlerHolder;
    }
    ProcHolder& proc() {
        return *this->mProcHolder;
    }
public:
    void notify(Event const& e);
    void notify(std::string const& name);
    template <typename T>
    void notify(std::string const& name, T& value) {
        this->notify(DataEvent<T>(name, value));
    }
public:
    ProxyHolder* mProxyHolder;
    WorkerHolder* mWorkerHolder;
    HandlerHolder* mHandlerHolder;
    ProcHolder* mProcHolder;
};

}

#endif
