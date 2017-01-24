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

namespace uActors{
class Props{
private:
	Producer	actorProducer 	= nullptr;
	SpawnFunc 	spawner 		= defaultSpawner;
/*
	mailboxProducer     mailbox.Producer
	supervisionStrategy SupervisorStrategy
	middleware          []func(next ActorFunc) ActorFunc
	middlewareChain     ActorFunc
	dispatcher          mailbox.Dispatcher
 */
public:

	Props(Producer ap): actorProducer(ap){};
	Props(SpawnFunc sp): spawner(sp){};

	PID* spawn(std::string id, PID* parent){
		if(spawner)
			return spawner(id, parent);
		else
			return defaultSpawner(id, parent);
	};

	static Props fromProducer(Producer actorProducer){
		Props props(actorProducer);
		return props;

	};

	static Props FromSpawnFunc(SpawnFunc spawn){
		Props props(spawn);
		return props;
	}
};
}; //namespace uActors



#endif /* SRC_ACTOR_PROPS_H_ */
