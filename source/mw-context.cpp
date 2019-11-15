
#include "mw-context.h"
#include "mw-proxy.h"
#include "mw-worker.h"
#include "mw-event.h"
#include "mw-proc.h"

namespace mw {

Context::Context() {
    this->mProxyHolder = new ProxyHolder(this);
    this->mWorkerHolder = new WorkerHolder(this);
    this->mProcHolder = new ProcHolder(this);
    this->mHandlerHolder = new HandlerHolder();
    this->mHandlerHolder->setListener(std::bind(&ProcHolder::handle, this->mProcHolder, std::placeholders::_1));
}

Context::~Context() {
    delete this->mHandlerHolder;
    delete this->mProcHolder;
    delete this->mWorkerHolder;
    delete this->mProxyHolder;
}

void Context::notify(Event const& e) {
    this->event().notify(e);
}

void Context::notify(std::string const& name) {
    this->event().notify(name);
}

}