#include "judge.h"
#include <iostream>
#include <string.h>
#include <chrono> 
#include <math.h>
using namespace std::chrono; 
using namespace std;
#define Name "s7"
#define PathIN "testCases/" Name ".in"

int Expected[] = { 0, 493408, 85952, 231082, 328655, 856704, 34672, 692430 };
//s1 493408
//s2 85952
//s3 231082
//s4 328655
//s5 856704
//s6 34672
//s7 692430

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
	int result = 0;
	auto start = high_resolution_clock::now(); 
	freopen(PathIN, "r" , stdin);
	printf("%s\n", PathIN);
	int operations;
	scanf("%lld %d", &seed, &operations);
	if(operations > 70000) {
		printf("operations should be less or equal to 70000\n");
		return;
	}
	for(int i=1, t=0; i <= operations; i++) {
		int rand = pseudoRand();
		//skip t
		t += 1 + (rand % (1999000-t)/(operations+1-i));
		int op = rand % 1000;
		
		if(op > (630 + 40*(int)log2(10*Workers+34))) { //add worker
			int senior = (rand % Workers) + 1;
			Workers++;
			solution->addWorker(t, Workers, senior);
		} else { //add work or check status
			//bool isAdd = Works == 0 || (rand % operations) > Works;
			if(Works > 0 && rand % 4 == 0) {
				Status status = solution->getStatus(t, (rand % Works) + 1);
				result = (result + status.finishTime + status.workerAssigned) % 1000003;
			} else {
				Works++;
				solution->addWork(t, Works, (rand % Workers) + 1, (rand % 1000) + 1, (pseudoRand() % 1000) + 1);				
			}
		}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	int idx = Name[1] - '0';
	if(result != Expected[idx])
	printf("wrong, should be %d\n", Expected[idx]);
	printf("result: %d, duration: %d\n", result, duration);
}

int result = 0;
int p = 29;

Status showStatus(ISolution *solution, int time, int id) {
	Status s = solution->getStatus(time, id);
	printf("for time: %d, job: %d, worker: %d, finish: %d\n", time, id, s.workerAssigned, s.finishTime);
	result = (result + p*(s.workerAssigned + (s.finishTime % 31))) % 1000003;
	p = p*29 % 1000003;
	return s;
}

void Judge2::run(ISolution *solution) {
	auto start = high_resolution_clock::now();
	solution->addWorker(2, 2, 1);
	solution->addWork(3, 1, 1, 20, 20);
	showStatus(solution, 4, 1);
	solution->addWork(5, 2, 2, 20, 18);
	showStatus(solution, 6, 2);
	solution->addWork(7, 3, 1, 20, 16);
	showStatus(solution, 8, 3);
	solution->addWork(9, 4, 1, 21, 12);
	showStatus(solution, 10, 4);
	solution->addWorker(11, 3, 1);
	showStatus(solution, 12, 3);
	showStatus(solution, 13, 4);
	showStatus(solution, 23, 3);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("result: %d, duration: %d\n", result, duration);
}

void Judge3::run(ISolution *solution) {
	auto start = high_resolution_clock::now();
	solution->addWork(1, 1, 1, 20, 4);
	solution->addWork(2, 2, 1, 20, 4);
	solution->addWorker(4, 2, 1);
	Status s1 = showStatus(solution, 12, 1);//1, 5
	Status s2 = showStatus(solution, 13, 2);//2, 8
	
	bool correct = s1.finishTime == 5 && s1.workerAssigned == 1
		&& s2.finishTime == 8 && s2.workerAssigned == 2;
	if(correct == false)
		printf("J4 wrong\n");

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("result: %d, duration: %d\n", result, duration);
}

void Judge4::run(ISolution *solution) {
	auto start = high_resolution_clock::now();
	solution->addWork(1, 1, 1, 20, 2);
	solution->addWork(2, 2, 1, 20, 2);
	solution->addWorker(4, 2, 1);
	
	Status s1 = showStatus(solution, 12, 1);//1, 3
	Status s2 = showStatus(solution, 13, 2);//1, 5
	bool correct = s1.finishTime == 3 && s1.workerAssigned == 1
		&& s2.finishTime == 5 && s2.workerAssigned == 1;
	if(correct == false)
		printf("J4 wrong\n");
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("result: %d, duration: %d\n", result, duration);
}

void Judge5::run(ISolution *solution) {
	auto start = high_resolution_clock::now();
	solution->addWork(1, 1, 1, 20, 2);
	solution->addWork(2, 2, 2, 20, 2);
	solution->addWorker(4, 2, 1);
	
	Status s1 = showStatus(solution, 12, 1);//1, 3
	Status s2 = showStatus(solution, 13, 2);//2, 6
	bool correct = s1.finishTime == 3 && s1.workerAssigned == 1
		&& s2.finishTime == 6 && s2.workerAssigned == 2;
	if(correct == false)
		printf("J5 wrong\n");
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	printf("result: %d, duration: %d\n", result, duration);
}