/*
 * Message.cpp
 *
 *  Created on: Jan 22, 2017
 *      Author: saman
 */


#include "Message.h"

namespace uActors{
//Set the type id for the specific type
template<typename T> const size_t _Message<T>::messageType = (typeid(T).hash_code());
}
