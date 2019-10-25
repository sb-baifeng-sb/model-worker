
#ifndef __MW_WORKER_H__
#define __MW_WORKER_H__

#include "mw-event.h"
#include "mw-facade.h"
#include "mw-proxy.h"
#include "vector"

namespace mw {

class Worker : public Notifer {
	friend class WorkerHolder;
public:
	typedef std::vector<std::string> WorkList;
public:
	Worker() {}
	Worker(std::string const& name) {
		this->mWorkerName = name;
	}
	virtual ~Worker() {}
public:
	std::string const& getName() const {
		return this->mWorkerName;
	}
protected:
	virtual WorkList worklist() const {
		WorkList list;
		return list;
	}
	virtual void handle(Event const& e) {
	    printf("default worker.\n");
	}
	virtual void onAttach(){}
	virtual void onDetach(){}
protected:
	template <typename T>
	T& get(std::string const& proxyName) {
		return facade->ph().get<T>(proxyName);
	}
private:
	std::string mWorkerName;
};

class WorkerHolder {
public:
	typedef std::map<std::string, Worker*> WorkerMap;
public:
	WorkerHolder(Facade* facade);
	~WorkerHolder();
public:
	bool add(Worker* worker);
	bool add(std::string const& workerName, Worker* worker);
	bool remove(std::string const& workerName);
	Worker& getWorker(std::string const& workerName);
    template <typename T>
    T& get(std::string const& workerName) {
        return *dynamic_cast<T*>(&getWorker(workerName));
    }
private:
	Facade* facade;
	WorkerMap mWorkerMap;
};

}

#endif // LITE2D_WORKER_H
