#include "judge.h"
#include <iostream>
#include <string.h>
#include <chrono> 
#include <math.h>
using namespace std::chrono; 
using namespace std;
#define Name "s4"
#define PathIN "testCases/" Name ".in"

namespace {
long long seed;
int pseudoRand() {
	seed = seed * 13477581 + 1;
	return (seed & 0x7FFFFFFF) % 1000000007;
}

int Workers = 1;
int Works = 0;
}

void Judge::run(ISolution *solution) {
	auto start = high_resolution_clock::now(); 
	freopen(PathIN, "r" , stdin);
	printf("%s\n", PathIN);
	// int workers, works, alphSize;
	// scanf("%lld %d %d", &seed, &workers, &works);
	int operations;
	scanf("%lld %d", &seed, &operations);
	if(operations > 70000) {
		printf("operations should be less or equal to 70000\n");
		return;
	}
	//for(int i=0, size = workers + works; i < size; i++) {
		//skip??
	//printf("log %d\n", (int)log2(99));
	for(int i=1, t=0; i <= operations; i++) {
		int rand = pseudoRand();
		//skip t
		t += 1 + (rand % (1999000-t)/(operations+1-i));
		int op = rand % 1000;
		//int limit = 20 + 8*(int)log2(10*Workers);
		//if(op > limit) { //add worker
		//if(op > (20 + 8*(int)log2(10*Workers))) { //add worker
		if(Workers==100){
			int s= 3;
		}
		if(op > (630 + 40*(int)log2(10*Workers+16))) { //add worker
			int senior = (rand % Workers) + 1;
			Workers++;
			solution->addWorker(t, Workers, senior);
		} else { //add work or check status
			bool isAdd = Works == 0 || (rand % operations) > Works;
			if(isAdd) {
				Works++;
				solution->addWork(t, Works, (rand % Workers) + 1, (rand % 1000) + 1, (pseudoRand() % 1000) + 1);
			} else {
				solution->getStatus(t, (rand % Works) + 1);
			}
		}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("duration %d\n", duration);
}
