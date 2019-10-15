
#ifndef __MW_WORKER_H__
#define __MW_WORKER_H__

#include "mw-event.h"
#include "mw-facade.h"
#include "vector"

namespace mw {

class Worker {
	friend class WorkerHolder;
public:
	typedef std::vector<std::string> WorkList;
public:
	Worker() {}
	Worker(std::string const& name) {
		this->mWorkerName = name;
	}
	virtual ~Worker() {
	}
public:
	std::string const& getWorkerName() const {
		return this->mWorkerName;
	}
	void notify(Event const& args) {
		this->facade->hh().notify(args);
	}
public:
	virtual void onAttach(){}
	virtual void onDetach(){}
	virtual void handle(Event const& args){}
	virtual WorkList worklist() {
		WorkList list;
		return list;
	}
private:
	Facade* facade;
	std::string mWorkerName;
};

class WorkerHolder {
public:
	typedef std::map<std::string, Worker*> WorkerMap;
public:
	WorkerHolder(Facade* facade);
	~WorkerHolder();
public:
	bool registerWorker(Worker* worker);
	bool registerWorker(std::string const& workerName, Worker* worker);
	bool removeWorker(std::string const& workerName);
	Worker& getWorker(std::string const& workerName);
private:
	Facade* facade;
	WorkerMap mWorkerMap;
};

}

#endif // LITE2D_WORKER_H
