
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

void Facade::notify(Event const& args) {
    this->hh().notify(args);
}

void Facade::notify(std::string const& name) {
    this->hh().notify(name);
}

void Facade::notify(std::string const& name, int v) {
    this->hh().notify(name, v);
}

void Facade::notify(std::string const& name, std::string const& v) {
    this->hh().notify(name, v);
}

}