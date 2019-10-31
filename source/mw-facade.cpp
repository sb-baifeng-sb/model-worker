
#include "mw-facade.h"
#include "mw-proxy.h"
#include "mw-worker.h"
#include "mw-event.h"
#include "mw-proc.h"

namespace mw {

Facade::Facade() {
    this->mProxyHolder = new ProxyHolder(this);
    this->mWorkerHolder = new WorkerHolder(this);
    this->mProcHolder = new ProcHolder(this);
    this->mHandlerHolder = new HandlerHolder();
    this->mHandlerHolder->setListener(std::bind(&ProcHolder::handle, this->mProcHolder, std::placeholders::_1));
}

Facade::~Facade() {
    delete this->mHandlerHolder;
    delete this->mProcHolder;
    delete this->mWorkerHolder;
    delete this->mProxyHolder;
}

void Facade::notify(Event const& e) {
    this->event().notify(e);
}

void Facade::notify(std::string const& name) {
    this->event().notify(name);
}

void Facade::notify(std::string const& name, int v) {
    this->event().notify(name, v);
}

void Facade::notify(std::string const& name, std::string const& v) {
    this->event().notify(name, v);
}

}