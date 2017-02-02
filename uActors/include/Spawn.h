/*
 * Spawn.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_SPAWN_H_
#define SRC_ACTOR_SPAWN_H_
#include "basics.h"
#include <string>

namespace uActors{
class Spawner{
private:
	static Spawner *defaultSpawner;
public:
	static Spawner& getDefault(){return *defaultSpawner;};
	// Spawn starts a new actor based on props and named with a unique id
	static PID* spawn(Props* props);
	// SpawnNamed starts a new actor based on props and named using the specified name
	static PID* spawnNamed(Props* props, std::string name);
	//Default spawner
	virtual PID* spawner(std::string id, Props* props, PID* parent){};//TODO: return error

	virtual ~Spawner(){};
};

class DefaultSpawner : public Spawner{

	PID* spawner(std::string id, Props* props, PID* parent);
};
};




#endif /* SRC_ACTOR_SPAWN_H_ */
