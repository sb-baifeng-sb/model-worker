
#include "mw-worker.h"

namespace mw {

WorkerHolder::WorkerHolder(Context* c):context(c) {
}

WorkerHolder::~WorkerHolder() {
	for (WorkerMap::iterator iter = this->mWorkerMap.begin(); iter != this->mWorkerMap.end(); ++iter) {
        delete iter->second;
    }
	this->mWorkerMap.clear();
}

bool WorkerHolder::add(Worker* worker) {
	if (this->mWorkerMap.find(worker->getName()) != this->mWorkerMap.end()) {
		delete worker;
		return false;
	}
	worker->context = this->context;
	{
		auto& event = this->context->event();
		Worker::WorkList list = worker->worklist();
		for (int i = 0; i < (int)list.size(); ++i) {
			event.add(list[i], new Handler<Worker>(worker, &Worker::handle));
		}
	}
	this->mWorkerMap.insert(std::make_pair(worker->getName(), worker));
	worker->onAttach();
	return true;
}

bool WorkerHolder::add(std::string const& workerName, Worker* worker) {
	worker->mWorkerName = workerName;
	return this->add(worker);
}

bool WorkerHolder::remove(std::string const& workerName) {
	Worker* worker = this->mWorkerMap[workerName];
	if (worker != NULL) {
		worker->onDetach();
		this->mWorkerMap.erase(workerName);
		{
			auto& event = this->context->event();
			Worker::WorkList list = worker->worklist();
			for (int i = 0; i < (int)list.size(); ++i) {
				event.remove(list[i], worker);
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
