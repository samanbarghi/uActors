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

Semaphore sem;
struct NextNodeMessage : public uMessage<NextNodeMessage>{
	PID* next;
    int id;
	NextNodeMessage(PID* n, int i) : next(n), id(i){};
};
struct MyMessage: public uMessage<MyMessage>{};

class MyActor : public Actor{
public:
	PID* next = nullptr;
    int id = -1;
    size_t counter =0;
	void receive(Context& c){
        if(const MyMessage* mm = c.getMessage().isa<MyMessage>()){
            if(id == 0){
                if(counter >= MAX_FORWARD){
                    sem.V();
                    return;
                }
            }
            ++counter;
            MyMessage *msg = new MyMessage();
            next->tell(*msg);
//			cout << counter << ": Here we are receiving: " << mm->value << endl;
		}else if(const NextNodeMessage* mm = c.getMessage().isa<NextNodeMessage>()){
			next = mm->next;
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

	for(size_t i = 0 ; i < MAX_FORWARD; i++){
	    MyMessage *mm = new MyMessage();
    	myactors[0]->tell(*mm);
    }
    std::cout << MAX_FORWARD << " Messages sent" << std::endl;
    clock_t begin = clock();
	sem.P();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "Elapsed time: " << elapsed_secs << " seconds " << endl;

	return 0;
}


