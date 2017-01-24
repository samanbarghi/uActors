/*
 * Spawn.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */
#include "Props.h"

namespace uActors{
// Spawn starts a new actor based on props and named with a unique id
PID* spawn(Props* props){
	return props->spawn(nullptr, nullptr);
};
// SpawnNamed starts a new actor based on props and named using the specified name
PID* spawnNamed(Props* props, std::string name){
	return props->spawn(name, nullptr);
};
//Default spawner
PID* _spawn(Props* props){
return nullptr;
};
}




