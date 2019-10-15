
#include "mw-worker.h"

namespace mw {

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
	if (this->mWorkerMap.find(worker->getName()) != this->mWorkerMap.end()) {
		delete worker;
		return false;
	}
	worker->facade = this->facade;
	{
		auto& holder = this->facade->hh();
		Worker::WorkList list = worker->worklist();
		for (int i = 0; i < (int)list.size(); ++i) {
			holder.registerHandler(list[i], new Handler<Worker>(worker, &Worker::handle));
		}
	}
	this->mWorkerMap.insert(std::make_pair(worker->getName(), worker));
	worker->onAttach();
	return true;
}

bool WorkerHolder::registerWorker(std::string const& workerName, Worker* worker) {
	worker->mWorkerName = workerName;
	return this->registerWorker(worker);
}

bool WorkerHolder::removeWorker(std::string const& workerName) {
	Worker* worker = this->mWorkerMap[workerName];
	if (worker != NULL) {
		worker->onDetach();
		this->mWorkerMap.erase(workerName);
		{
			auto& holder = facade->hh();
			Worker::WorkList list = worker->worklist();
			for (int i = 0; i < (int)list.size(); ++i) {
				holder.removeHandler(list[i], worker);
			}
		}
		delete worker;
		return true;
	}
	return false;
}

Worker& WorkerHolder::getWorker(std::string const& workerName) {
	return *this->mWorkerMap[workerName];
}

}
