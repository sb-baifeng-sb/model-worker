
#ifndef __MW_PROXY_H__
#define __MW_PROXY_H__

#include "mw-event.h"
#include "mw-context.h"

namespace mw {

class Proxy;
class ProxyHolder {
public:
	typedef std::map<std::string, Proxy*> ProxyMap;
public:
	ProxyHolder(Context* c);
	~ProxyHolder();
public:
	bool add(Proxy* proxy);
	bool add(std::string const& proxyName, Proxy* proxy);
	bool remove(std::string const& proxyName);
	bool has(std::string const& proxyName);
	Proxy& getProxy(std::string const& proxyName) const;
	template <typename T>
	T& get(std::string const& proxyName) const {
		return *dynamic_cast<T*>(&getProxy(proxyName));
	}
private:
	Context* context;
	ProxyMap mProxyMap;
};

class Proxy : public Notifer {
	friend class ProxyHolder;
public:
	Proxy() {}
	Proxy(std::string const& name) {
		this->mProxyName = name;
	}
	virtual ~Proxy() {}
public:
	std::string const& Name() const {
		return this->mProxyName;
	}
protected:
	void setName(std::string const& name) {
		this->mProxyName = name;
	}
protected:
	virtual void onAttach(){}
	virtual void onDetach(){}
protected:
	template <typename T>
	T& get(std::string const& proxyName) const {
		return context->proxy().get<T>(proxyName);
	}
private:
	std::string mProxyName;
};

template <typename T>
class DataOwner {
public:
	typedef T Data;
public:
	DataOwner() {}
	DataOwner(Data const& data):mData(data) {}
	virtual ~DataOwner() {}
public:
	void setData(Data const& data) {
		this->mData = data;
	}
	Data const& getData() const {
		return this->mData;
	}
	Data& getData() {
		return this->mData;
	}
protected:
	Data mData;
};

template <typename T>
class DataProxy : public Proxy, public DataOwner<T> {
public:
	typedef typename DataOwner<T>::Data Data;
public:
	DataProxy() {}
	DataProxy(Data const& data):DataOwner<T>(data) {}
};

}

#endif // __MW_PROXY_H__
