
#ifndef __MW_EVENT_H__
#define __MW_EVENT_H__

#include "stdio.h"
#include "stdlib.h"
#include "string"
#include "map"
#include "list"
#include "vector"
//#include "functional"

namespace mw {

class Event {
public:
	enum {
		TYPE = 0xA000,
	};
public:
    Event(std::string const& msgName) {
        this->mName = msgName;
    }
	virtual ~Event(){}
public:
	std::string const& Name() const {
		return this->mName;
	}
	virtual int Type() const {
		return Event::TYPE;
	};
private:
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

class StringEvent : public Event {
public:
	enum {
		TYPE = 0xA001,
	};
public:
	StringEvent(std::string const& msgName, std::string const& value):Event(msgName) {
		this->mMsgValue = value;
	}
public:
	std::string const& Value() const {
		return this->mMsgValue;
	}
	int Type() const override {
		return StringEvent::TYPE;
	}
private:
	std::string mMsgValue;
};

class IntEvent : public Event {
public:
	enum {
		TYPE = 0xA002,
	};
public:
	IntEvent(std::string const& msgName, int value):Event(msgName) {
		this->mMsgValue = value;
	}
	int Type() const override {
		return IntEvent::TYPE;
	}
public:
	int Value() const {
		return this->mMsgValue;
	}
private:
	int mMsgValue;
};

class FloatEvent : public Event {
public:
	enum {
		TYPE = 0xA003,
	};
public:
	FloatEvent(std::string const& msgName, float value):Event(msgName) {
		this->mMsgValue = value;
	}
public:
	float Value() const {
		return this->mMsgValue;
	}
	int Type() const override {
		return FloatEvent::TYPE;
	}
private:
	float mMsgValue;
};

class KVEvent : public Event {
public:
	enum {
		TYPE = 0xA004,
	};
public:
	typedef std::map<std::string, std::string> ValueMap;
public:
	KVEvent(std::string const& msgName, ValueMap const& vm):Event(msgName) {
		this->mValue = vm;
	}
	KVEvent(std::string const& msgName):Event(msgName) {}
public:
	std::string const& operator[](std::string const& key) const {
		auto iter = this->mValue.find(key);
		if (iter == this->mValue.end()) {
			return this->mDef;
		}
		return iter->second;
	}
	std::string& operator[](std::string const& key) {
		return this->mValue[key];
	}
	ValueMap& Value() {
		return this->mValue;
	}
	int Type() const override {
		return KVEvent::TYPE;
	}
private:
	std::string mDef;
	ValueMap mValue;
};

class ParamsEvent : public Event {
public:
	enum {
		TYPE = 0xA005,
	};
public:
	typedef std::vector<std::string> ValueArray;
public:
	ParamsEvent(std::string const& msgName, ValueArray const& va):Event(msgName) {
		this->mValue = va;
	}
	ParamsEvent(std::string const& msgName):Event(msgName) {}
public:
	std::string const& operator[](unsigned int index) const {
		return this->mValue[index];
	}
	unsigned int size() const {
		return this->mValue.size();
	}
	ValueArray const& Value() const {
		return this->mValue;
	}
	ValueArray& Value() {
		return this->mValue;
	}
	int Type() const override {
		return ParamsEvent::TYPE;
	}
private:
	ValueArray mValue;
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
	virtual void handler(Event const& e) {
		(this->target->*this->callfunc)(e);
	}
private:
	T* target;
	FUNC callfunc;
};

class HandlerHolder {
public:
	typedef std::list<HandlerImp*> HandlerList;
	typedef std::map<std::string, HandlerList> HandlerMap;
	typedef std::function<void(Event const& e)> Listener;
public:
	~HandlerHolder();
public:
	bool add(std::string const& eventName, HandlerImp* handler);
	bool remove(std::string const& eventName, void* target);
public:
	void setListener(Listener const& l);
public:
	void notify(Event const& e);
	void notify(std::string const& name);
	void notify(std::string const& name, int v);
	void notify(std::string const& name, std::string const& v);
private:
	HandlerMap mHandlerMap;
	Listener mListener;
};

class Facade;
class Notifer {
public:
	virtual ~Notifer(){}
public:
	void notify(Event const& e);
	void notify(std::string const& name);
	void notify(std::string const& name, int v);
	void notify(std::string const& name, std::string const& v);
protected:
	Facade* facade;
};

} // lite2d

#endif // LITE2D_EVENT_H
