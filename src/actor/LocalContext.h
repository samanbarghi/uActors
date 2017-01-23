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
namespace uActors{

class LocalContext : public Context{
	Message*	 		message;
	PID*				parent;
	PID* 				self;
	Actor*				actor;
	ActorFunc			receiveFunc;
	std::vector<PID*>	children;
	/*
	supervisor     SupervisorStrategy
	producer       Producer
	middleware     ActorFunc
	behavior       behaviorStack
	receive        ActorFunc
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
	LocalContext(PID* parent):parent(parent){};

	Message& getMessage(){
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
		return spawnNamed(props, "Test");
	};

	PID* spawnNamed(Props& props, std::string name){

		return nullptr;
	};

	std::vector<PID*> getChildren(){};


	/*** local Functions **/
	void InvokeUserMessage(Message& msg){
		processMessage(msg);
	};

	void InvokeSystemMessage(Message& msg){

	};

	void processMessage(Message& msg){
		message = msg;
		if(msg.isType<PoisonPill>()){

		}else{
			receiveFunc(*this);
		}
	}

	void receive(Message& msg){
		processMessage(msg);
	}

};
}



#endif /* SRC_ACTOR_LOCALCONTEXT_H_ */
