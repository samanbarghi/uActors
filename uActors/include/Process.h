/*
 * Process.h
 *
 *  Created on: Jan 25, 2017
 *      Author: saman
 */

#ifndef SRC_PROCESS_H_
#define SRC_PROCESS_H_

#include "basics.h"
#include "Message.h"
#include "Mailbox.h"
#include <iostream>

namespace uActors{

class DefaultSpawner;
class Process{
public:
	virtual void sendUserMessage(PID* pid, Message &msg, PID* sender){};
	virtual void sendSystemMessage(PID* pid, Message &msg){};
	virtual void stop(PID* pid){};

	virtual ~Process(){};
};


class LocalProcess : public Process{
	friend class DefaultSpawner;
private:
	Inbound*	mailbox;
	bool dead = false;


public:
	void sendUserMessage(PID* pid, Message &msg, PID* sender){
		mailbox->postUserMessage(msg);		//TODO: include the sender with envelop
//		std::cout << pid <<  ":Sending by reference UserMessage:" << &msg << std::endl;
	};


	void sendSystemMessage(PID* pid, Message &msg){
		mailbox->postSystemMessage(msg);		//TODO: include the sender with envelop

//		std::cout << pid << ":Sending UserMessage:" << &msg << std::endl;
	};
	void stop(PID* pid){
		dead = true;
		sendSystemMessage(pid, Message::stopMessage);
	};
	~LocalProcess(){};
};
}; //namespace uActors



#endif /* SRC_PROCESS_H_ */
