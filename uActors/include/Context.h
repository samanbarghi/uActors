/*
 * Context.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_CONTEXT_H_
#define SRC_ACTOR_CONTEXT_H_

#include <string>
#include <vector>
#include "basics.h"
#include "PID.h"
#include "Message.h"
#include "Props.h"


namespace uActors{

class Actor;
class Context{

/*
	// Watch registers the actor as a monitor for the specified PID
	Watch(pid *PID)

	// Unwatch unregisters the actor as a monitor for the specified PID
	Unwatch(pid *PID)


	// SetReceiveTimeout sets the inactivity timeout, after which a ReceiveTimeout message will be sent to the actor.
	// A duration of less than 1ms will disable the inactivity timer.
	//
	// If a message is received before the duration d, the timer will be reset. If the message conforms to
	// the NotInfluenceReceiveTimeout interface, the timer will not be reset
	SetReceiveTimeout(d time.Duration)

	// ReceiveTimeout returns the current timeout
	ReceiveTimeout() time.Duration


	// SetBehavior replaces the actors current behavior stack with the new behavior
	SetBehavior(behavior ActorFunc)

	// PushBehavior pushes the current behavior on the stack and sets the current Receive handler to the new behavior
	PushBehavior(behavior ActorFunc)

	// PopBehavior reverts to the previous Receive handler
	PopBehavior()



	// Stash stashes the current message on a stack for reprocessing when the actor restarts
	Stash()

	// Respond sends a response to the to the current `Sender`
	//
	// If the Sender is nil, the actor will panic
	Respond(response interface{})

	*/
public:

	// Message returns the current message to be processed
	virtual const Message& getMessage(){};

	// Parent returns the PID for the current actors parent
	virtual PID* getParent(){};

	// Sender returns the PID of actor that sent currently processed message
	virtual PID* getSender(){};

	// Actor returns the actor associated with this context
	virtual Actor& getActor(){};

	// Self returns the PID for the current actor
	virtual PID* getPID(){};

	// Spawn spawns a child actor using the given Props
	virtual PID* spawn(Props& props){};

	// SpawnNamed spawns a named child actor using the given Props
	//
	// ErrNameExists will be returned if id already exists
	virtual PID* spawnNamed(Props& props, std::string name){}; //TODO: report error (*PID, error)

	// Returns a slice of the actors children
	virtual std::vector<PID*> getChildren(){};


};
}



#endif /* SRC_ACTOR_CONTEXT_H_ */
