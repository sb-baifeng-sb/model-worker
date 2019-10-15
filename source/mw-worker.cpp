
#include "mw-worker.h"

namespace mw {

static void workstart(Facade* facade, Worker* worker) {
	auto holder = facade->hh();
	Worker::WorkList list = worker->worklist();
	for (int i = 0; i < (int)list.size(); ++i) {
		holder.registerHandler(list[i], new Handler<Worker>(worker, &Worker::handle));
	}
}

static void workstop(Facade* facade, Worker* worker) {
	auto holder = facade->hh();
	Worker::WorkList list = worker->worklist();
	for (int i = 0; i < (int)list.size(); ++i) {
		holder.removeHandler(list[i], worker);
	}
}

WorkerHolder::WorkerHolder(Facade* facade) {
	this->facade = facade;
}

WorkerHolder::~WorkerHolder() {
	for (WorkerMap::iterator iter = this->mWorkerMap.begin(); iter != this->mWorkerMap.end(); ++iter) {
        delete iter->second;
    }
	this->mWorkerMap.clear();
}

bool WorkerHolder::registerWorker(Worker* worker) {
	if (this->mWorkerMap[worker->getWorkerName()]!=NULL) {
		delete worker;
		return false;
	}
	workstart(facade, worker);
	this->mWorkerMap[worker->getWorkerName()] = worker;
	worker->onAttach();
	return true;
}

bool WorkerHolder::removeWorker(std::string const& workerName) {
	Worker* worker = this->mWorkerMap[workerName];
	if (worker != NULL) {
		worker->onDetach();
		this->mWorkerMap.erase(workerName);
		workstop(facade, worker);
		delete worker;
		return true;
	}
	return false;
}

Worker& WorkerHolder::getWorker(std::string const& workerName) {
	return *this->mWorkerMap[workerName];
}

}
