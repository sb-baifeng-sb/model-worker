
#include "mw-event.h"
#include "mw-facade.h"

namespace mw {

HandlerHolder::~HandlerHolder() {
    for (HandlerMap::iterator iter = this->mHandlerMap.begin(); iter != this->mHandlerMap.end(); ++iter) {
        auto& list = iter->second;
        for (HandlerList::iterator i = list.begin(); i != list.end(); ++i) {
            delete *i;
        }
    }
	this->mHandlerMap.clear();
}

bool HandlerHolder::registerHandler(std::string const& eventName, HandlerImp* handler) {
	auto& list = this->mHandlerMap[eventName];
	list.push_back(handler);
	return true;
}

bool HandlerHolder::removeHandler(std::string const& eventName, void* target) {
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

void HandlerHolder::notify(Event const& args) {
	auto& list = this->mHandlerMap[args.msgName()];
	for (HandlerList::iterator i = list.begin(); i != list.end(); ++i) {
		(*i)->handler(args);
	}
}

void HandlerHolder::notify(std::string const& name) {
    this->notify(Event(name));
}

void HandlerHolder::notify(std::string const& name, int v) {
    this->notify(IntEvent(name, v));
}

void HandlerHolder::notify(std::string const& name, std::string const& v) {
    this->notify(StringEvent(name, v));
}

void Notifer::notify(Event const& args) {
    this->facade->notify(args);
}

void Notifer::notify(std::string const& name) {
    this->facade->notify(name);
}

void Notifer::notify(std::string const& name, int v) {
    this->facade->notify(name, v);
}

void Notifer::notify(std::string const& name, std::string const& v) {
    this->facade->notify(name, v);
}

} // lite2d
