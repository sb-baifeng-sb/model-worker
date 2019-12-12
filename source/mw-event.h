
#ifndef __MW_EVENT_H__
#define __MW_EVENT_H__

#include <string>
#include <map>
#include <list>
#include <vector>
#include <functional>
#include <assert.h>

namespace mw {

class Context;
class Event {
	friend class HandlerHolder;
public:
    Event(std::string const& msgName):context(nullptr) {
        this->mName = msgName;
    }
	virtual ~Event(){}
public:
	std::string const& Name() const {
		return this->mName;
	}
	Context const& Ctx() const {
		return *this->context;
	}
	virtual bool Check(std::string const& TypeName) const {
		assert(false && "Event::Check must be override.");
		return false;
	}
private:
	Context* context;
	std::string mName;
};

class HandlerImp {
public:
	virtual ~HandlerImp(){}
public:
	virtual void handler(Event const& e){}
    virtual bool match(void* target) const {
        return false;
    }
};

template <typename T>
class DataEvent : public Event {
public:
	typedef T const& ObjectType;
public:
	DataEvent(std::string const& msgName, ObjectType target):Event(msgName), mObject(target) {}
public:
	ObjectType Data() const {
		return this->mObject;
	}
	bool Check(std::string const& TypeName) const override {
		return TypeName == typeid(mObject).name() || TypeName == typeid(DataEvent<T>).name();
	}
private:
	ObjectType mObject;
};

template<typename T>
class Handler : public HandlerImp {
public:
	typedef void (T::*FUNC)(Event const&);
public:
	Handler(T* target, FUNC func):target(target), callfunc(func) {}
	Handler(FUNC func, T* target):target(target), callfunc(func) {}
	Handler(Handler const& obj):target(obj.target), callfunc(obj.callfunc) {}
	virtual ~Handler() {
		this->target = NULL;
		this->callfunc = NULL;
	}
public:
	virtual bool match(void* target) const {
		return this->target==target;
	}
public:
	virtual void handler(Event const& e) {
		(this->target->*this->callfunc)(e);
	}
private:
	T* target;
	FUNC callfunc;
};

class HandlerHolder {
	friend class Context;
public:
	typedef std::list<HandlerImp*> HandlerList;
	typedef std::map<std::string, HandlerList> HandlerMap;
	typedef std::function<void(Event const& e)> Listener;
public:
	HandlerHolder(Context* c);
	~HandlerHolder();
public:
	bool add(std::string const& eventName, HandlerImp* handler);
	bool remove(std::string const& eventName, void* target);
	template <typename T>
	bool add(std::string const& eventName, typename Handler<T>::FUNC func, T* target) {
		return this->add(eventName, new Handler<T>(target, func));
	}
public:
	void notify(Event const& e);
	void notify(std::string const& name);
	template <typename T>
	void notify(std::string const& name, T const& value) {
		this->notify(DataEvent<T>(name, value));
	}
private:
	void setListener(Listener const& l);
private:
	Context* context;
	HandlerMap mHandlerMap;
	Listener mListener;
};

class Notifer {
public:
	virtual ~Notifer(){}
public:
	void notify(Event const& e);
	void notify(std::string const& name);
	template <typename T>
	void notify(std::string const& name, T const& value) {
		this->notify(DataEvent<T>(name, value));
	}
public:
	Context& Ctx() {
		return *this->context;
	}
protected:
	Context* context;
};

}

#endif // __MW_EVENT_H__
