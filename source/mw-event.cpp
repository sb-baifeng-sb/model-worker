
#include "mw-event.h"

namespace mw {

HandlerHolder::~HandlerHolder() {
    for (HandlerMap::iterator iter = this->mHandlerMap.begin(); iter != this->mHandlerMap.end(); ++iter) {
        HandlerList& list = iter->second;
        for (HandlerList::iterator i = list.begin(); i != list.end(); ++i) {
            delete *i;
        }
    }
	this->mHandlerMap.clear();
}

bool HandlerHolder::registerHandler(std::string const& eventName, HandlerImp* handler) {
	HandlerList& list = this->mHandlerMap[eventName];
	list.push_back(handler);
	return true;
}

bool HandlerHolder::removeHandler(std::string const& eventName, void* target) {
	HandlerList& list = this->mHandlerMap[eventName];
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
	HandlerList temp = this->mHandlerMap[args.msgType()];
	for (HandlerList::iterator i = temp.begin(); i != temp.end(); ++i) {
		(*i)->handler(args);
	}
}

} // lite2d
