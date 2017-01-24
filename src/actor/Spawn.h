/*
 * Spawn.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_SPAWN_H_
#define SRC_ACTOR_SPAWN_H_
#include "basics.h"

namespace uActors{
// Spawn starts a new actor based on props and named with a unique id
PID* spawn(Props* props);
// SpawnNamed starts a new actor based on props and named using the specified name
PID* spawnNamed(Props* props, std::string name);
//Default spawner
PID* _spawn(Props* props);

const SpawnFunc defaultSpawner = _spawn;
};




#endif /* SRC_ACTOR_SPAWN_H_ */
