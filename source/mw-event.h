
#ifndef __MW_EVENT_H__
#define __MW_EVENT_H__

#include "stdio.h"
#include "stdlib.h"
#include "string"
#include "map"
#include "list"
//#include "functional"

namespace mw {

class Event {
public:
    Event(std::string const& msgtype) {
        this->mMsgType = msgtype;
    }
	virtual ~Event(){}
public:
	std::string const& msgType() const {
		return this->mMsgType;
	}
private:
	std::string mMsgType;
};

class HandlerImp {
public:
	virtual ~HandlerImp(){}
public:
	virtual void handler(Event const& args){}
    virtual bool match(void* target) const {
        return false;
    }
};

class StringEvent : public Event {
public:
	StringEvent(std::string const& msgtype, std::string const& value):Event(msgtype) {
		this->mMsgValue = value;
	}
public:
	std::string const& msgValue() const {
		return this->mMsgValue;
	}
private:
	std::string mMsgValue;
};

class IntEvent : public Event {
public:
	IntEvent(std::string const& msgtype, int value):Event(msgtype) {
		this->mMsgValue = value;
	}
public:
	int msgValue() const {
		return this->mMsgValue;
	}
private:
	int mMsgValue;
};

class KVEvent : public Event {
public:
	typedef std::map<std::string, std::string> ValueMap;
public:
	KVEvent(std::string const& msgtype, ValueMap const& vm):Event(msgtype) {
		this->mValue = vm;
	}
public:
	ValueMap& msgValue() {
		return this->mValue;
	}
private:
	ValueMap mValue;
};


template<typename T>
class Handler : public HandlerImp {
public:
	typedef void (T::*FUNC)(Event const&);
public:
	Handler(T* target) {
		this->target = target;
		this->callfunc = NULL;
	}
	Handler(T* target, FUNC func) {
		this->target = target;
		this->callfunc = func;
	}
	Handler(Handler const& obj) {
		this->target = obj.target;
		this->callfunc = obj.callfunc;
	}
	virtual ~Handler() {
		this->target = NULL;
		this->callfunc = NULL;
	}
public:
	virtual bool match(void* target) const {
		return this->target==target;
	}
public:
	virtual void handler(Event const& args) {
		(this->target->*this->callfunc)(args);
	}
private:
	T* target;
	FUNC callfunc;
};

class HandlerHolder {
public:
	typedef std::list<HandlerImp*> HandlerList;
	typedef std::map<std::string, HandlerList> HandlerMap;
public:
	~HandlerHolder();
public:
	bool registerHandler(std::string const& eventName, HandlerImp* handler);
	bool removeHandler(std::string const& eventName, void* target);
	void notify(Event const& args);
private:
	HandlerMap mHandlerMap;
};

} // lite2d

#endif // LITE2D_EVENT_H
