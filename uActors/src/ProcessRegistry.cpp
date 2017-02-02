/*
 * ProcessRegistry.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: saman
 */
#include "../include/ProcessRegistry.h"
#include "../include/PID.h"

using namespace uActors;

ProcessRegistry *ProcessRegistry::instance = new ProcessRegistry();

std::string ProcessRegistry::uint64ToId(size_t u){
	char buf[13] = {0};
	size_t i = 13;

	while(u >= 64){
		i--;
		buf[i] = digits[u&0x3f];
		u >>=6;
	};
	i--;
	buf[i] = digits[u];
	i--;
	buf[i] = '$';

	//copy from i to end
	return std::string(buf + i, buf+13);


};

std::string ProcessRegistry::NextId(){
	return uint64ToId(sequenceId.fetch_add(1, std::memory_order_relaxed));
};
PID* ProcessRegistry::add(Process &process, std::string id){
	PID* pid = new PID(address, id);
	if(localPIDs.insert(id, &process)){
		return pid;
	}else
		return nullptr;
};


bool ProcessRegistry::remove(PID* pid){
	return localPIDs.erase(pid->id);
};

Process* ProcessRegistry::get(PID* pid){
	Process* spp = nullptr;
	if(pid == nullptr)
		return spp; //deadletter
	//TODO: figure out remote addresses

	if(localPIDs.find(pid->id, spp)){
		return spp;
	}else{
		//error
		return nullptr;
	}
};


