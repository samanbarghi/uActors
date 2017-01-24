/*
 * basics.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_BASICS_H_
#define SRC_ACTOR_BASICS_H_

namespace uActors{
class Context;
class Actor;
class PID;
class Props;

// The ActorFunc type is an adapter to allow the use of ordinary functions as actors to process messages
typedef void (*ActorFunc)(Context&);

// The Producer type is a function that creates a new actor
typedef Actor* (*Producer)();

typedef PID* (*SpawnFunc)(std::string id, Props* props, PID* parent); //TODO: how to return error

};



#endif /* SRC_ACTOR_BASICS_H_ */
