
#include "mw-proxy.h"

namespace mw {

ProxyHolder::ProxyHolder(Context* c):context(c) {
}

ProxyHolder::~ProxyHolder() {
	for (ProxyMap::iterator iter = this->mProxyMap.begin(); iter != this->mProxyMap.end(); ++iter) {
        delete iter->second;
    }
	this->mProxyMap.clear();
}

bool ProxyHolder::add(Proxy* proxy) {
	if (this->mProxyMap.find(proxy->getName()) != this->mProxyMap.end()) {
		delete proxy;
		return false;
	}
	proxy->context = this->context;
	this->mProxyMap.insert(std::make_pair(proxy->getName(), proxy));
	proxy->onAttach();
	return true;
}

bool ProxyHolder::add(std::string const& proxyName, Proxy* proxy) {
	proxy->mProxyName = proxyName;
	return this->add(proxy);
}

bool ProxyHolder::remove(std::string const& proxyName) {
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
	assert(this->mProxyMap.find(proxyName) != this->mProxyMap.end());
	return *this->mProxyMap[proxyName];
}

}
