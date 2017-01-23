/*
 * Main.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: saman
 */

#include <iostream>
#include "LocalContext.h"
#include "Actor.h"

using namespace std;
using namespace uActors;

class MyActor : public Actor{
public:
	void receive(Context& c){
		cout << "Here we are receiving" << endl;
	};
};

int main(){
	MyActor ma;
	LocalContext lc(nullptr);
	ma.receive(lc);
	return 0;
}


