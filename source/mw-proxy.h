
#ifndef __MW_PROXY_H__
#define __MW_PROXY_H__

#include "mw-event.h"
#include "mw-facade.h"

namespace mw {

class Proxy : public Notifer {
	friend class ProxyHolder;
public:
	Proxy() {}
	Proxy(std::string const& name) {
		this->mProxyName = name;
	}
	virtual ~Proxy() {}
public:
	std::string const& getName() const {
		return this->mProxyName;
	}
protected:
	virtual void onAttach(){}
	virtual void onDetach(){}
private:
	std::string mProxyName;
};

class ProxyHolder {
public:
	typedef std::map<std::string, Proxy*> ProxyMap;
public:
	ProxyHolder(Facade* facade);
	~ProxyHolder();
public:
	bool registerProxy(Proxy* proxy);
	bool registerProxy(std::string const& proxyName, Proxy* proxy);
	bool removeProxy(std::string const& proxyName);
	Proxy& getProxy(std::string const& proxyName);
	template <typename T>
	T& get(std::string const& proxyName) {
		return *dynamic_cast<T*>(&getProxy(proxyName));
	}
private:
	Facade* facade;
	ProxyMap mProxyMap;
};

} // lite2d

#endif // LITE2D_PROXY_H
