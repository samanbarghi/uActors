/*
 * Message.h
 *
 *  Created on: Jan 14, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_MESSAGE_H_
#define SRC_ACTOR_MESSAGE_H_


#include <typeinfo>
#include <cstddef>
#include <type_traits>

namespace uActors {

class Message{
private:
	//Type of this message
	const size_t type;
	//Make the class virtual for typeid hierarchy to work
	virtual void noop(){};
	Message();
protected:
	Message(size_t t):type(t){};
public:
	size_t getType() const {return type;};
	//T should be of type _Message
	template<typename T> T* isa(){
		T* t = nullptr;
		if(type == T::messageType)
			t = static_cast<T*>(this);
		return t;
	}
	virtual ~Message(){};
};

//intermediate class for static polymorphism
template<typename Derived>
class _Message : public Message {
private:
	void noop(){};
protected:
	_Message<Derived>():Message(_Message<Derived>::messageType){};
public:
	const static size_t messageType;
};

/** Categories **/
//AutoReceive Message
template<typename T>
struct AutoReceiveMessage: public _Message<T>{};

//System Message
template<typename T>
struct SystemMessage: public _Message<T>{};

//User Message
template<typename T>
struct uMessage: public _Message<T>{};

/** AutoReceive Message **/
struct Restaring : AutoReceiveMessage<Restaring>{};
struct Stopping 	: AutoReceiveMessage<Stopping>{};
struct Stopped	: AutoReceiveMessage<Stopped>{};
struct PoisonPill: AutoReceiveMessage<PoisonPill>{};

/** System Messages **/
struct Started	: SystemMessage<Started>{};
struct Stop		: SystemMessage<Stop>{};
struct Watch		: SystemMessage<Watch>{};
struct Unwatch 	: SystemMessage<Unwatch>{};
struct Terminated: SystemMessage<Terminated>{};
struct Failure	: SystemMessage<Failure>{};
struct Restart	: SystemMessage<Restart>{};
}  // namespace uActors


#endif /* SRC_ACTOR_MESSAGE_H_ */
