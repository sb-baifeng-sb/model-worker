
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
	if (has(proxy->Name())) {
		delete proxy;
		return false;
	}
	proxy->setCtx(this->context);
	this->mProxyMap.insert(std::make_pair(proxy->Name(), proxy));
	proxy->onAttach();
	return true;
}

bool ProxyHolder::add(std::string const& proxyName, Proxy* proxy) {
	proxy->mProxyName = proxyName;
	return this->add(proxy);
}

bool ProxyHolder::remove(std::string const& proxyName) {
	auto iter = this->mProxyMap.find(proxyName);
	if (iter == this->mProxyMap.end()) {
		return false;
	}
	Proxy* proxy = iter->second;
	proxy->onDetach();
	this->mProxyMap.erase(proxyName);
	// last to delete, because proxyName belong to proxy
	delete proxy;
	return true;
}

bool ProxyHolder::has(std::string const& proxyName) {
	return this->mProxyMap.find(proxyName) != this->mProxyMap.end();
}

Proxy& ProxyHolder::getProxy(std::string const& proxyName) const {
	auto iter = this->mProxyMap.find(proxyName);
	assert(iter != this->mProxyMap.end());
	return *iter->second;
}

}
