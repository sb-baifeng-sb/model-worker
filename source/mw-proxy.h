
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
	Proxy& getProxy(std::string const& proxyName);
	template <typename T>
	T& get(std::string const& proxyName) {
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
	virtual void onAttach(){}
	virtual void onDetach(){}
protected:
	template <typename T>
	T& get(std::string const& proxyName) {
		return context->proxy().get<T>(proxyName);
	}
private:
	std::string mProxyName;
};

} // lite2d

#endif // LITE2D_PROXY_H
