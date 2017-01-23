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
// The ActorFunc type is an adapter to allow the use of ordinary functions as actors to process messages
typedef void (*ActorFunc)(Context&);
};



#endif /* SRC_ACTOR_BASICS_H_ */
