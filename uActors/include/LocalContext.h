/*
 * LocalContext.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_LOCALCONTEXT_H_
#define SRC_ACTOR_LOCALCONTEXT_H_

#include "basics.h"
#include "Context.h"
#include "Mailbox.h"
#include "Actor.h"
#include <memory>

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
namespace uActors{
class DefaultSpawner;
class LocalContext : public Context, public MessageInvoker{
	friend class DefaultSpawner;
	Message*					message 	= nullptr;
	PID*						parent		= nullptr;
	PID* 						self		= nullptr;
	Actor*						actor		= nullptr;
	ActorFunc					receiveFunc	= nullptr;
	std::vector<PID*>			children;
	Producer       				producer;
	bool 						stopping 	= false;
	bool 						restarting 	= false;
	/*
	supervisor     SupervisorStrategy
	middleware     ActorFunc
	behavior       behaviorStack
	children       PIDSet
	watchers       PIDSet
	watching       PIDSet
	stash          *linkedliststack.Stack
	stopping       bool
	restarting     bool
	receiveTimeout time.Duration
	t              *time.Timer
	restartStats   *RestartStatistics
	*/

public:
	LocalContext(Producer producer, PID* parent):producer(producer), parent(parent){
		incarnateActor();
	};

	void incarnateActor(){
		actor = producer();
		stopping = false;
		restarting = false;
		receiveFunc = &Actor::receive;
	}

	const Message& getMessage(){
		return *message;
	};
	PID* getParent(){
		return nullptr;
	};
	PID* getSender(){
		return nullptr;
	};

	Actor& getActor(){
		return *actor;
	};

	PID* getPID(){
		return self;
	};



	PID* spawn(Props& props){
		return spawnNamed(props, ProcessRegistry::getInstance()->NextId());
	};

	PID* spawnNamed(Props& props, std::string name){
		PID* pid = props.spawn(std::string(self->getId()+"/"+name), self);
		//if(pid == nullptr) //TODO:error
		return pid;

	};

	std::vector<PID*> getChildren(){};


	/*** local Functions **/
	void invokeUserMessage(Message* msg){
//		std::cout << "Invoking user Message" << std::endl;
		processMessage(*msg);
		delete msg;
		message = nullptr;
	};

	void invokeSystemMessage(Message* msg){
		//for now do nothing

	};

	void processMessage(Message& msg){
		message = &msg;
		if(msg.isType<PoisonPill>()){

		}else{
//			std::cout << "about to call receive func" << std::endl;
			//CALL_MEMBER_FN(actor, receiveFunc)(*this);
			actor->receive(*this);
			//now it's safe to remvoe the message
		}
	}

	void receive(Message& msg){
		processMessage(msg);
	}

    ~LocalContext(){
        delete actor;
    }

};
}



#endif /* SRC_ACTOR_LOCALCONTEXT_H_ */
