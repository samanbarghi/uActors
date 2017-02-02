/*
 * uAQueue.h
 *
 *  Created on: Jan 31, 2017
 *      Author: saman
 */

#ifndef SRC_UACTORS_INCLUDE_UQUEUE_H_
#define SRC_UACTORS_INCLUDE_UQUEUE_H_

#include "Message.h"
namespace uActors{

class uQueue{
	virtual void push(Message&){};
	virtual Message& pop(){};
	virtual ~uQueue(){};
};
};


#endif /* SRC_UACTORS_INCLUDE_UQUEUE_H_ */
