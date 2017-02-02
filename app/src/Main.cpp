/*
 * Main.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#include <iostream>
#include <uThreads/uThreads.h>
#include "LocalContext.h"
#include "ProcessRegistry.h"
#include "Actor.h"
#include "Message.h"

using namespace std;
using namespace uActors;
const size_t MAX_FORWARD = 1000000;

Semaphore sem;
struct NextNodeMessage : public uMessage<NextNodeMessage>{
	PID* next;
	NextNodeMessage(PID* n) : next(n){};
};
struct MyMessage: public uMessage<MyMessage>{
public:
	int value;
	MyMessage(int v):value(v){};
};

class MyActor : public Actor{
public:
	PID* next;
	size_t counter =0;
	void receive(Context& c){
		if(const MyMessage* mm = c.getMessage().isa<MyMessage>()){
			counter++;
			if(counter <= MAX_FORWARD){
				//assert(next != nullptr);
				next->tell(*mm);
			}else{
				sem.V();
			}
//			cout << counter << ": Here we are receiving: " << mm->value << endl;
		}else if(const NextNodeMessage* mm = c.getMessage().isa<NextNodeMessage>()){
			//cout << "Next: " << mm->next << endl;
			next = mm->next;
		}
	};
};


Actor* MyActorProducer(){
	return new MyActor();
};

int main(){
	kThread kt(Cluster::getDefaultCluster());
//	kThread kt1(Cluster::getDefaultCluster());
	const int NUM_ACTORS = 100;
	PID* myactors[NUM_ACTORS];
	auto p = Props::fromProducer(MyActorProducer);
	for(size_t i =0; i < NUM_ACTORS; i++){
		myactors[i] = Spawner::spawn(&p);
	};
	for(size_t i = 0 ; i < NUM_ACTORS; i++){
		myactors[i]->tell(NextNodeMessage(myactors[(i+1)%NUM_ACTORS]));
	}
	MyMessage mm(10);
	myactors[0]->tell(mm);
	sem.P();
	return 0;
}


