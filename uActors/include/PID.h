/*
 * PID.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_PID_H_
#define SRC_ACTOR_PID_H_

#include "Process.h"
#include "ProcessRegistry.h"
#include <string>
#include <memory>

namespace uActors{
class DefaultSpawner;
class ProcessRegistry;
class PID{
	friend class DefaultSpawner;
	friend class ProcessRegistry;
	std::string address;
	std::string id;

	Process* p = nullptr;
public:

	PID(std::string address, std::string id): id(id), address(address){};

	Process* ref(){
		//TODO: make this atomic
		if(p != nullptr)
			return p;

		p = ProcessRegistry::getInstance()->get(this);
        assert( p != nullptr);
		//if(p == nullptr) error could not find, or should the error be propagated to who ever called the function?
		return p;

	};

	std::string& getId(){
		return id;
	}
	//copy the message
	void tell(Message& msg){
//		Message* msg_cpy = msg.clone();
		Process* p = ref();
//        std::cout << "TELL    :\t"  << p << std::endl;
        assert(p!= nullptr);
		if(p != nullptr)
			(*p).sendUserMessage(this, msg, nullptr);
        else
            std::cerr << "cannot tell to a nonexistent actor" << std::endl;

		//else error
	};

    //copy the rvalue
    void tell(Message&& rvmsg){
        Message* msg = rvmsg.clone();
        tell(*msg);
    }

	//send by reference
	/*void tell(const Message* msg){
		Message* msg_ncnst = (Message*)msg;
		Process* p = ref();
		if(p != nullptr)
			(*p).sendUserMessage(this, *msg_ncnst, nullptr);
		//else error

	}*/
};
}



#endif /* SRC_ACTOR_PID_H_ */
