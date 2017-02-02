/*
 * Message.cpp
 *
 *  Created on: Jan 22, 2017
 *      Author: saman
 */


#include "../include/Message.h"
#include "../include/Mailbox.h"
#include "../include/Dispatcher.h"
#include <typeinfo>


using namespace uActors;
//Set the type id for the specific type
Stop uActors::Message::stopMessage;
Started uActors::Message::startedMessage;

Dispatcher* Dispatcher::defaultDispatcher = new DefaultDispatcher(DISPATCHER_THROUGHPUT);
std::atomic<size_t> Dispatcher::dispatched(0);
