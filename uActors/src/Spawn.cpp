/*
 * Spawn.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */
#include "../include/Props.h"
#include "../include/Spawn.h"
#include "../include/Process.h"
#include "../include/ProcessRegistry.h"
#include "../include/LocalContext.h"
#include <iostream>
#include <memory>

using namespace uActors;
Spawner *Spawner::defaultSpawner = new DefaultSpawner();
// Spawn starts a new actor based on props and named with a unique id
PID* Spawner::spawn(Props* props){
	std::string name = ProcessRegistry::getInstance()->NextId();
	return props->spawn(name, nullptr ); //TODO: generate a name
};
// SpawnNamed starts a new actor based on props and named using the specified name
PID* Spawner::spawnNamed(Props* props, std::string name){
	return props->spawn(name, nullptr);
};
//Default spawner
PID* DefaultSpawner::spawner(std::string id, Props* props, PID* parent){
	LocalProcess* lp = new LocalProcess();
	PID* pid = ProcessRegistry::getInstance()->add(*lp, id); //TODO: check if exists

	LocalContext* cell = new LocalContext(props->getProducer(), nullptr);
	Inbound* mailbox = props->produceMailbox(*cell, *props->getDispatcher());

	lp->mailbox = mailbox;
	pid->p = lp;
	cell->self = pid;

	mailbox->start();
	mailbox->postSystemMessage(Message::startedMessage);

//	std::cout << "Spawning" << std::endl;

	return pid;
};




