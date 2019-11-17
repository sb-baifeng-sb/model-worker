
#include "mw-event.h"
#include "mw-context.h"

namespace mw {

HandlerHolder::HandlerHolder(Context* c):context(c) {

}

HandlerHolder::~HandlerHolder() {
    for (HandlerMap::iterator iter = this->mHandlerMap.begin(); iter != this->mHandlerMap.end(); ++iter) {
        auto& list = iter->second;
        for (HandlerList::iterator i = list.begin(); i != list.end(); ++i) {
            delete *i;
        }
    }
	this->mHandlerMap.clear();
}

bool HandlerHolder::add(std::string const& eventName, HandlerImp* handler) {
	auto& list = this->mHandlerMap[eventName];
	list.push_back(handler);
	return true;
}

bool HandlerHolder::remove(std::string const& eventName, void* target) {
	auto& list = this->mHandlerMap[eventName];
	for (HandlerList::iterator i = list.begin(); i != list.end(); ++i) {
		if ((*i)->match(target)) {
            delete *i;
			list.erase(i);
			return true;
		}
	}
	return false;
}

void HandlerHolder::setListener(Listener const& l) {
    this->mListener = l;
}

void HandlerHolder::notify(Event const& e) {
    assert(e.context == nullptr && "HandlerHolder::notify event.context must nullptr.");
    ((Event&)e).context = this->context;
	auto& list = this->mHandlerMap[e.Name()];
	for (HandlerList::iterator i = list.begin(); i != list.end(); ++i) {
		(*i)->handler(e);
	}
    if (this->mListener != nullptr) {
        this->mListener(e);
    }
}

void HandlerHolder::notify(std::string const& name) {
    this->notify(Event(name));
}

void Notifer::notify(Event const& e) {
    this->context->notify(e);
}

void Notifer::notify(std::string const& name) {
    this->context->notify(name);
}

} // lite2d
