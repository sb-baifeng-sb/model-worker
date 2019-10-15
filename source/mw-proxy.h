
#ifndef __MW_PROXY_H__
#define __MW_PROXY_H__

#include "mw-event.h"
#include "mw-facade.h"

namespace mw {

class Proxy {
	friend class ProxyHolder;
public:
	Proxy() {}
	Proxy(std::string const& name):facade(nullptr) {
		this->mProxyName = name;
	}
	virtual ~Proxy() {
	}
public:
	std::string const& getProxyName() const {
		return this->mProxyName;
	}
	void notify(Event const& args) {
		this->facade->hh().notify(args);
	}
public:
	virtual void onAttach(){}
	virtual void onDetach(){}
private:
	Facade* facade;
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
private:
	Facade* facade;
	ProxyMap mProxyMap;
};

} // lite2d

#endif // LITE2D_PROXY_H
