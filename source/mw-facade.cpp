
#include "mw-facade.h"
#include "mw-proxy.h"
#include "mw-worker.h"
#include "mw-event.h"

namespace mw {

Facade::Facade() {
    this->mProxyHolder = new ProxyHolder(this);
    this->mWorkerHolder = new WorkerHolder(this);
    this->mHandlerHolder = new HandlerHolder();
}

Facade::~Facade() {
    delete this->mProxyHolder;
    delete this->mWorkerHolder;
    delete this->mHandlerHolder;
}

}