/*
 * Mailbox.h
 *
 *  Created on: Jan 31, 2017
 *      Author: saman
 */

#ifndef SRC_UACTORS_INCLUDE_MAILBOX_H_
#define SRC_UACTORS_INCLUDE_MAILBOX_H_
#include "Message.h"
#include "Dispatcher.h"
#include "uQueue.h"
#include <atomic>
#include <memory>

namespace uActors{

class MessageInvoker{
public:
	virtual void invokeSystemMessage(Message* msg){};
	virtual void invokeUserMessage(Message* msg){};
	//virtual void escalateFailure(); //TODO
	virtual ~MessageInvoker(){};
};

class Inbound{
public:
	virtual void postUserMessage(Message& msg){};
	virtual void postSystemMessage(Message& msg){};
	virtual void start(){};
	virtual ~Inbound(){};
};

class DefaultMailbox : public Inbound{

public:
	BlockingMPSCQueue<Message> 	userMailbox;
	BlockingMPSCQueue<Message>	systemMailbox;
	MessageInvoker&				invoker;
	Dispatcher&					dispatcher;

	bool						suspended;
	std::atomic<bool>    	    dispatcherStatus;
	std::atomic<uint32_t>		systemMessageCount;
	std::atomic<uint32_t>		userMessageCount;


	DefaultMailbox(MessageInvoker& mi, Dispatcher& disp): dispatcher(disp), invoker(mi), suspended(false), dispatcherStatus(false),
			systemMessageCount(0), userMessageCount(0){};

	//process messages
	void run(){
		size_t i = 0;
		size_t t = dispatcher.getThroughput();
        const size_t MAX_COUNT = 1;
        size_t counter = 0;

		Message* msg = nullptr;

		for(;;){
			if(i > t ){
//				std::cout << "Yielding" << std::endl;
                i = 0;
				dispatcher.yield();
			}
			i++;

            msg = nullptr;
//            std::cout << "Sys:" << systemMessageCount <<  " usr: " << userMessageCount << std::endl;
			//keep processing system messages
			msg = systemMailbox.pop();
			if(msg != nullptr){
//				std::cout << "Receiving System Message"<< std::endl;
				systemMessageCount.fetch_sub(1, std::memory_order_relaxed);
				if(msg->isType<SuspendMailbox>()){
					suspended = true;
				}else if(msg->isType<ResumeMailbox>()){
					suspended = false;
				}else{
					invoker.invokeSystemMessage(msg);
				}
				continue;
			}

			//if suspended, stop processing
			if(suspended)	return;

			msg = userMailbox.pop();
			if(msg != nullptr){
//				std::cout << "Receiving User Message" << std::endl;
				userMessageCount.fetch_sub(1, std::memory_order_relaxed);
				invoker.invokeUserMessage(msg);

			}else if(++counter < MAX_COUNT){
               dispatcher.yield();
			}else{
                return;
            }
		}
	};

	static void processMessage(void* m){
		DefaultMailbox* mbox = (DefaultMailbox*)m;
		bool idleState = false;
//		std::cout << mbox << ":Processing: " << std::endl;
		while(true){
			mbox->run();
//            std::cout << "FINISHED RUN:\t" << mbox << std::endl;

			mbox->dispatcherStatus.store(false, std::memory_order_release);
			uint32_t sys = mbox->systemMessageCount.load(std::memory_order_relaxed);
			uint32_t user= mbox->userMessageCount.load(std::memory_order_relaxed);

//            std::cout << "MESSAGE STAT:\t" << mbox << "\tSys:" << sys <<  " usr: " << user << std::endl;
			if(sys > 0 || (!mbox->suspended && user > 0)){
//                std::cout << "RUN AGAIN:\t" << mbox << std::endl;
				if(!mbox->dispatcherStatus.compare_exchange_strong(idleState, true, std::memory_order_acquire)){
					break;
				};
			}else break;
			idleState = false;
		};

	};
	void schedule(){
		bool idleState = false;
//        std::cout << "SCHEDULE:\t" << this << "\tSTATUS:\t" << dispatcherStatus.load() << std::endl;
		if(dispatcherStatus.compare_exchange_strong(idleState, true, std::memory_order_acquire)){
			dispatcher.schedule(processMessage, *this);
		};
	};
	void postUserMessage(Message& msg){
		userMailbox.push(msg);
		userMessageCount.fetch_add(1, std::memory_order_relaxed);
		schedule();
	};

	void postSystemMessage(Message& msg){
		systemMailbox.push(msg);
		systemMessageCount.fetch_add(1, std::memory_order_relaxed);
		schedule();
	};

	void start(){
		//TODO: for now do nothing
	};


	~DefaultMailbox(){};

	static Inbound* producer(MessageInvoker& mi, Dispatcher& disp){
		return new DefaultMailbox(mi, disp);
	};


};

};


#endif /* SRC_UACTORS_INCLUDE_MAILBOX_H_ */
