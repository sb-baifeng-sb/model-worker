
#ifndef __MW_CONTEXT_H__
#define __MW_CONTEXT_H__

#include <string>

namespace mw {

class ProxyHolder;
class WorkerHolder;
class HandlerHolder;
class ProcHolder;
class Event;

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
    void notify(std::string const& name, int v);
    void notify(std::string const& name, float v);
    void notify(std::string const& name, std::string const& v);
public:
    ProxyHolder* mProxyHolder;
    WorkerHolder* mWorkerHolder;
    HandlerHolder* mHandlerHolder;
    ProcHolder* mProcHolder;
};

}

#endif
