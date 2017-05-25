/*
 * Dispatcher.h
 *
 *  Created on: Jan 31, 2017
 *      Author: saman
 */

#ifndef SRC_UACTORS_INCLUDE_DISPATCHER_H_
#define SRC_UACTORS_INCLUDE_DISPATCHER_H_

#include "uThreads/uThreads.h"
#include "basics.h"
#include <memory>
#include <iostream>
namespace uActors{

class DefaultMailbox;

class Dispatcher{
protected:
	size_t throughput;
	static std::atomic<size_t> dispatched;
public:
	Dispatcher(size_t thrp):throughput(thrp){};
	virtual void schedule(ProcessMailBoxFunc func, DefaultMailbox& m){};
	virtual void yield(){};
	virtual ~Dispatcher(){};

	size_t getThroughput(){return throughput;};

	static Dispatcher*	defaultDispatcher;
};

class uThreadsDispatcher: public Dispatcher{
public:
	uThreadsDispatcher(size_t thrp): Dispatcher(thrp){ };
	void schedule(ProcessMailBoxFunc func, DefaultMailbox& m){
//		dispatched.fetch_add(1, std::memory_order_relaxed);
		uThread* ut = uThread::create();
        ut->start(Cluster::getDefaultCluster(), (void*)func, (void*)&m, nullptr, nullptr);
//        ut->setHomekthread(uThread::currentUThread()->getHomekThread());
//        std::cout << "DISPATCH:\t" << &m << "\tCOUNT:\t" << dispatched << "\tHomekThread:\t" << ut->getHomekThread() <<   std::endl;
	};
	void yield(){
		uThread::yield();
	};
};

typedef uThreadsDispatcher DefaultDispatcher;
};



#endif /* SRC_UACTORS_INCLUDE_DISPATCHER_H_ */
