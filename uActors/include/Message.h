/*
 * Message.h
 *
 *  Created on: Jan 14, 2017
 *      Author: saman
 */

#ifndef SRC_ACTOR_MESSAGE_H_
#define SRC_ACTOR_MESSAGE_H_


#include <cstddef>
#include <memory>
#include <typeinfo>
#include <uThreads/uThreads.h>

namespace uActors {

class Stop;
class Started;
class DefaultMailbox;

class Message: public Link<Message>{
	friend class DefaultMailbox;
private:
	//Type of this message
	size_t type;
	//Make the class virtual for typeid hierarchy to work
	virtual void noop(){};
protected:
	Message():type(0){};
	void setType(size_t t){type = t;};
public:
	const size_t getType() const {return type;};
	//T should be of type _Message
	template<typename T> const T* const isa() const{
		T const *t = nullptr;
		if(type == T::getMessageType())
			t = (T* const)(this);
		return t;
	}
	//T should be of type _Message
	template<typename T> bool isType() const{
		return (type == T::getMessageType());
	}

	virtual Message* clone() const{};
	//default message inctances
	static Stop	stopMessage;
	static Started startedMessage;
	virtual ~Message(){};
};

//intermediate class for static polymorphism
template<typename Derived>
class _Message : public Message {
private:
	void noop(){};
protected:
	_Message<Derived>():Message(){
		setType(_Message<Derived>::getMessageType());
	};
public:
	Message* clone() const{
		return new Derived(static_cast<const Derived&>(*this));
	};
	static size_t getMessageType(){
		static size_t messageType(typeid(Derived).hash_code());
		return messageType;
	};
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
struct Restaring	: 	public AutoReceiveMessage<Restaring>{};
struct Stopping		: 	public AutoReceiveMessage<Stopping>{};
struct Stopped		: 	public AutoReceiveMessage<Stopped>{};
struct PoisonPill	: 	public AutoReceiveMessage<PoisonPill>{};

/** System Messages **/
struct Started		:	public SystemMessage<Started>{};
struct Stop			: 	public SystemMessage<Stop>{};
struct Watch		: 	public SystemMessage<Watch>{};
struct Unwatch		: 	public SystemMessage<Unwatch>{};
struct Terminated	: 	public SystemMessage<Terminated>{};
struct Failure		: 	public SystemMessage<Failure>{};
struct Restart		: 	public SystemMessage<Restart>{};
//mailbox
struct SuspendMailbox : public SystemMessage<SuspendMailbox>{};
struct ResumeMailbox  :	public SystemMessage<ResumeMailbox>{};
};  // namespace uActors




#endif /* SRC_ACTOR_MESSAGE_H_ */
