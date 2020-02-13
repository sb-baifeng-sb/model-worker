
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
	if (has(worker->Name())) {
		delete worker;
		return false;
	}
	worker->setCtx(this->context);
	{
		auto& event = this->context->event();
		Worker::WorkList list = worker->worklist();
		for (int i = 0; i < (int)list.size(); ++i) {
			event.add(list[i], &Worker::handle, worker);
		}
	}
	this->mWorkerMap.insert(std::make_pair(worker->Name(), worker));
	worker->onAttach();
	return true;
}

bool WorkerHolder::add(std::string const& workerName, Worker* worker) {
	worker->mWorkerName = workerName;
	return this->add(worker);
}

bool WorkerHolder::remove(std::string const& workerName) {
	auto iter = this->mWorkerMap.find(workerName);
	if (iter == this->mWorkerMap.end()) {
		return false;
	}
	Worker* worker = iter->second;
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

bool WorkerHolder::has(std::string const& workerName) {
	return this->mWorkerMap.find(workerName) != this->mWorkerMap.end();
}

Worker& WorkerHolder::getWorker(std::string const& workerName) const {
	auto iter = this->mWorkerMap.find(workerName);
	assert(iter != this->mWorkerMap.end());
	return *iter->second;
}

}
