/*
 * ProcessRegistry.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: saman
 */

#ifndef SRC_UACTORS_INCLUDE_PROCESSREGISTRY_CPP_
#define SRC_UACTORS_INCLUDE_PROCESSREGISTRY_CPP_

#include <atomic>
#include <string>
#include "Process.h"
#include "memory"
#include <libcuckoo/cuckoohash_map.hh>
#include <libcuckoo/city_hasher.hh>

namespace uActors{
class PID;
typedef  cuckoohash_map<std::string, Process*, CityHasher<std::string>> ConcurrentMap;

class ProcessRegistry{
	std::string		address;
	ConcurrentMap	localPIDs;
	static ProcessRegistry *instance;
	const std::string localAddress = "nohost";
	std::atomic<size_t>	sequenceId;
	//RemoteHandlers []AddressResolver
	ProcessRegistry():address(localAddress), sequenceId(0){

	};
	const std::string digits = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~+";

public:
	static ProcessRegistry* getInstance(){
		if(!instance)
			instance = new ProcessRegistry();
		return instance;
	};

	std::string uint64ToId(size_t u);
	std::string NextId();

	PID* add(Process& process, std::string id);
	bool remove(PID* pid);
	Process* get(PID* pid);

};


};




#endif /* SRC_UACTORS_INCLUDE_PROCESSREGISTRY_CPP_ */
