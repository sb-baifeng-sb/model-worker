
#ifndef __MW_WORKER_H__
#define __MW_WORKER_H__

#include "mw-event.h"
#include "mw-context.h"
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
	std::string const& Name() const {
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
	T& get(std::string const& proxyName) const {
		return context->proxy().get<T>(proxyName);
	}
private:
	std::string mWorkerName;
};

class WorkerHolder {
public:
	typedef std::map<std::string, Worker*> WorkerMap;
public:
	WorkerHolder(Context* context);
	~WorkerHolder();
public:
	bool add(Worker* worker);
	bool add(std::string const& workerName, Worker* worker);
	bool remove(std::string const& workerName);
	Worker& getWorker(std::string const& workerName) const;
    template <typename T>
    T& get(std::string const& workerName) const {
        return *dynamic_cast<T*>(&getWorker(workerName));
    }
private:
	Context* context;
	WorkerMap mWorkerMap;
};

}

#endif // __MW_WORKER_H__
