
#ifndef __MW_FACADE_H__
#define __MW_FACADE_H__

namespace mw {

class ProxyHolder;
class WorkerHolder;
class HandlerHolder;

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
public:
    ProxyHolder* mProxyHolder;
    WorkerHolder* mWorkerHolder;
    HandlerHolder* mHandlerHolder;
};

}

#endif
