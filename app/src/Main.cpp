/*
 * Main.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#include <iostream>
#include <unistd.h>
#include <ctime>

#include <uThreads/uThreads.h>
#include "LocalContext.h"
#include "ProcessRegistry.h"
#include "Actor.h"
#include "Message.h"

using namespace std;
using namespace uActors;
size_t MAX_FORWARD = 10000;
size_t NUM_ACTORS = 100;

std::chrono::time_point<std::chrono::high_resolution_clock> tbegin;

Semaphore sem;
struct NextNodeMessage : public uMessage<NextNodeMessage>{
	PID* nextNode;
    int id;
	NextNodeMessage(PID* n, int i) : nextNode(n), id(i){};
};
struct MyMessage: public uMessage<MyMessage>{};

class MyActor : public Actor{
public:
	PID* nextNode = nullptr;
    int id = -1;
    size_t counter = 0;
	void receive(Context& c){
        if(const MyMessage* mm = c.getMessage().isa<MyMessage>()){
            //std::cout << id << ":" << uThread::getHomekThread() << std::endl;
            if(id == 0){
                if(counter >= MAX_FORWARD){
                    auto elapsed = std::chrono::high_resolution_clock::now() - tbegin;
                    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                    cout << "Elapsed time: " << microseconds << " useconds " << endl;
                    exit(0);

                }else ++counter;
            }
            MyMessage *msg = new MyMessage();
            nextNode->tell(*msg);
//			cout << counter << ": Here we are receiving: " << mm->value << endl;
		}else if(const NextNodeMessage* mm = c.getMessage().isa<NextNodeMessage>()){
			nextNode = mm->nextNode;
            id = mm->id;
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

    std::cout << "Running with " << number_of_threads << " threads and " <<
    NUM_ACTORS << " actors, and " << MAX_FORWARD << " rounds" << std::endl;

    for(size_t i=1; i < number_of_threads; i++)
        kThread *kt = new kThread(Cluster::getDefaultCluster());

//    kThread kt1(Cluster::getDefaultCluster());

	PID* myactors[NUM_ACTORS];
	auto p = Props::fromProducer(MyActorProducer);
	for(size_t i =0; i < NUM_ACTORS; i++){
		myactors[i] = Spawner::spawn(&p);
	};
	for(size_t i = 0 ; i < NUM_ACTORS; i++){
		myactors[i]->tell(NextNodeMessage(myactors[(i+1)%NUM_ACTORS], i));
	}

    tbegin  = std::chrono::high_resolution_clock::now();;
	for(size_t i = 0 ; i < MAX_FORWARD; i++){
	    MyMessage *mm = new MyMessage();
    	myactors[0]->tell(*mm);
    }

    std::cout << MAX_FORWARD << " Messages sent" << std::endl;
    sem.P();

	return 0;
}


