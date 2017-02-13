/*
 * Main.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#include <iostream>
#include <unistd.h>

#include <uThreads/uThreads.h>
#include "LocalContext.h"
#include "ProcessRegistry.h"
#include "Actor.h"
#include "Message.h"

using namespace std;
using namespace uActors;
size_t MAX_FORWARD = 10000;
size_t NUM_ACTORS = 100;

Semaphore sem;

std::chrono::time_point<std::chrono::high_resolution_clock> tbegin;

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
	PID* next = nullptr;
	void receive(Context& c){
		if(const MyMessage* mm = c.getMessage().isa<MyMessage>()){
			if(mm->value > 0){
                MyMessage *msg = new MyMessage(mm->value - 1);
				next->tell(*msg);
			}else{
                auto elapsed = std::chrono::high_resolution_clock::now() - tbegin;
                long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                cout << "Elapsed time: " << microseconds << " useconds " << endl;
                exit(0);
			}
//			cout << counter << ": Here we are receiving: " << mm->value << endl;
		}else if(const NextNodeMessage* mm = c.getMessage().isa<NextNodeMessage>()){
			next = mm->next;
		}
	};
};


Actor* MyActorProducer(){
	return new MyActor();
};

int main(int argc, const char * argv[]){
    if(argc < 3){
        std::cout << "Format: ./app thread_no actor_no round_no" << std::endl;
        exit(0);
    }
    size_t number_of_threads = atoi(argv[1]);
    NUM_ACTORS = atoi(argv[2]);
    MAX_FORWARD= atoi(argv[3]);
    for(size_t i=1; i < number_of_threads; i++)
        kThread *kt = new kThread(Cluster::getDefaultCluster());

//    kThread kt1(Cluster::getDefaultCluster());

	PID* myactors[NUM_ACTORS];
	auto p = Props::fromProducer(MyActorProducer);
	for(size_t i =0; i < NUM_ACTORS; i++){
		myactors[i] = Spawner::spawn(&p);
	};
	for(size_t i = 0 ; i < NUM_ACTORS; i++){
		myactors[i]->tell(NextNodeMessage(myactors[(i+1)%NUM_ACTORS]));
	}
	MyMessage *mm = new MyMessage(MAX_FORWARD);
	myactors[0]->tell(*mm);
	sem.P();
	return 0;
}


