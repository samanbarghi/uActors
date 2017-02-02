/*
 * Props.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_PROPS_H_
#define SRC_ACTOR_PROPS_H_
#include "basics.h"
#include "Spawn.h"
#include "Dispatcher.h"
#include "Mailbox.h"
#include <iostream>
#include <memory>

namespace uActors{

class Props{
	friend class Spawner;
private:
	Producer							actorProducer 	= nullptr;
	Spawner 							&spawner 		= Spawner::getDefault();
	MailboxProducer						mailboxProducer = DefaultMailbox::producer;
	Dispatcher*							dispatcher 		= Dispatcher::defaultDispatcher;
/*
	supervisionStrategy SupervisorStrategy
	middleware          []func(next ActorFunc) ActorFunc
	middlewareChain     ActorFunc
 */
public:

	Props(){};
	Props(Producer ap): actorProducer(ap){};
	Props(Spawner& sp): spawner(sp){};

	Dispatcher* getDispatcher(){ return dispatcher;};
	Producer	getProducer(){return actorProducer;};
	Inbound* produceMailbox(MessageInvoker& mi, Dispatcher& disp){return mailboxProducer(mi, disp);};


	PID* spawn(std::string id, PID* parent){
			return spawner.spawner(id, this, parent);
	};

	//change to move semantics
	static Props fromProducer(Producer actorProducer){
		Props props(actorProducer);
		return props;

	};

	static Props fromSpawner(Spawner spawn){
		Props props(spawn);
		return props;
	};

};
}; //namespace uActors



#endif /* SRC_ACTOR_PROPS_H_ */
