/*
 * basics.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_BASICS_H_
#define SRC_ACTOR_BASICS_H_
#include <cstddef>
namespace uActors{
class Context;
class Actor;
class PID;
class Props;
class Inbound;
class MessageInvoker;
class Dispatcher;


// The ActorFunc type is an adapter to allow the use of ordinary functions as actors to process messages
typedef void (Actor::*ActorFunc)(Context&);

// The Producer type is a function that creates a new actor
typedef Actor* (*Producer)();

//function with no arguments
typedef void (*ProcessMailBoxFunc)(void*);

//mailbox producer type
typedef Inbound* (*MailboxProducer)(MessageInvoker&, Dispatcher&);

//typedef PID* (*SpawnFunc)(std::string id, Props* props, PID* parent); r

/*** static functions **/
//null deleter for shared_ptr
static void null_deleter(void *){
    // null_deleter
}

/*** CONST VARIABLES ***/
const size_t DISPATCHER_THROUGHPUT = 300;

};



#endif /* SRC_ACTOR_BASICS_H_ */
