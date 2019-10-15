
#include "mw-proxy.h"

namespace mw {

ProxyHolder::ProxyHolder(Facade* facade) {
	this->facade = facade;
}

ProxyHolder::~ProxyHolder() {
	for (ProxyMap::iterator iter = this->mProxyMap.begin(); iter != this->mProxyMap.end(); ++iter) {
        delete iter->second;
    }
	this->mProxyMap.clear();
}

bool ProxyHolder::registerProxy(Proxy* proxy) {
	if (this->mProxyMap.find(proxy->getName()) != this->mProxyMap.end()) {
		delete proxy;
		return false;
	}
	proxy->facade = this->facade;
	this->mProxyMap.insert(std::make_pair(proxy->getName(), proxy));
	proxy->onAttach();
	return true;
}

bool ProxyHolder::registerProxy(std::string const& proxyName, Proxy* proxy) {
	proxy->mProxyName = proxyName;
	return this->registerProxy(proxy);
}

bool ProxyHolder::removeProxy(std::string const& proxyName) {
	Proxy* proxy = this->mProxyMap[proxyName];
	if (proxy != NULL) {
		proxy->onDetach();
		delete proxy;
		this->mProxyMap.erase(proxyName);
		return true;
	}
	return false;
}

Proxy& ProxyHolder::getProxy(std::string const& proxyName) {
	return *this->mProxyMap[proxyName];
}

}
