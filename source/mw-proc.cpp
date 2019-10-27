
#include "mw-proc.h"

namespace mw {

ProcHolder::ProcHolder(Facade* facade) {
    this->facade = facade;
}

bool ProcHolder::add(std::string const& eventName, Proc const& p) {
    this->mProcMap[eventName] = p;
    return true;
}

bool ProcHolder::remove(std::string const& eventName) {
    this->mProcMap.erase(eventName);
    return true;
}

void ProcHolder::handle(Event const& e) {
    auto iter = this->mProcMap.find(e.Name());
    if (iter == this->mProcMap.end()) {
        return;
    }
    iter->second(*this->facade, e);
}

}