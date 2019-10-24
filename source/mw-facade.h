
#ifndef __MW_FACADE_H__
#define __MW_FACADE_H__

#include <string>

namespace mw {

class ProxyHolder;
class WorkerHolder;
class HandlerHolder;
class ProcHolder;
class Event;

class Facade {
public:
    Facade();
    virtual ~Facade();
public:
    ProxyHolder& ph() {
        return *this->mProxyHolder;
    }
    WorkerHolder& wh() {
        return *this->mWorkerHolder;
    }
    HandlerHolder& hh() {
        return *this->mHandlerHolder;
    }
    ProcHolder& proc() {
        return *this->mProcHolder;
    }
public:
    void notify(Event const& args);
    void notify(std::string const& name);
    void notify(std::string const& name, int v);
    void notify(std::string const& name, std::string const& v);
public:
    ProxyHolder* mProxyHolder;
    WorkerHolder* mWorkerHolder;
    HandlerHolder* mHandlerHolder;
    ProcHolder* mProcHolder;
};

typedef Facade App;

}

#endif
