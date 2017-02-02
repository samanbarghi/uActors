/*
 * Actor.h
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_ACTOR_H_
#define SRC_ACTOR_ACTOR_H_

#include "basics.h"
#include "Context.h"

namespace uActors{

class Actor{
public:
	virtual void receive(Context&){};
	virtual ~Actor(){};
};
} //namespace uActors




#endif /* SRC_ACTOR_ACTOR_H_ */
