#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "judge.h"
#include <vector>
using namespace std;
bool shouldLog = false;

void sortVector(vector<int> &v, int count, int range) {
	int *counter = new int[range];
	for(int i=0; i < range; i++) 
		counter[i] = -1;
	for(int i=0; i < count; i++)
		counter[v[i]] = i;
	v.clear();
	for(int s=0; s < range; s++) {
		if(counter[s] > -1)
			v.push_back(s);
	}
	delete counter;
}

void sortWorkers(int *source, int *target, int *indexOfSource, int count, int range) {
	int *counter = new int[range];
	for(int i=0; i < range; i++) 
		counter[i] = -1;
	for(int i=0; i < count; i++)
		counter[source[i]] = i;
	
	for(int s=0, t=0; s < range; s++) {
		if(counter[s] > -1) {
			target[t] = s;
			indexOfSource[t] = counter[s];
			t++;
		}
	}
	delete counter;
}

class NaiveSolution : public ISolution {	
	struct Job {
		int workerId;
		int importance;
		int runtime;
		Status status;
	};
	vector<int> freeWorkers;
	vector<int> workersToBeFree[2000001]; //workers that becomes free at this time
	vector<int> waitingJobs;
	int lastTime;
public:
	Job jobs[70001];
	int workers[100];
	NaiveSolution() {
		// for(int i=0; i<100; i++)
		// 	workers[i] = -1;
		lastTime = 1;
		addWorker(0, 1, 0);
	}

	int assignJob(int workerId) { //returns finishTime
		//printf("assigning job for worker %d \n", workerId);
		int parentId = workers[workerId];
		if(parentId == 0)
			parentId = workerId;
		int bestJob = -1;
		int i=0, idxOfBestJob=-1;
		for(int job : waitingJobs) {
			if(jobs[job].workerId == workerId || jobs[job].workerId == parentId )
				if(bestJob == -1
					|| (jobs[job].importance == jobs[bestJob].importance && job < bestJob)
					|| (jobs[job].importance < jobs[bestJob].importance)) 
				{
					bestJob = job, idxOfBestJob = i;
				}
			i++;
		}
		if(bestJob > -1) {
			if(shouldLog)
				printf("assigning worker to job\n");
			waitingJobs.erase(waitingJobs.begin() + idxOfBestJob);
			jobs[bestJob].status.workerAssigned = workerId;
			jobs[bestJob].status.finishTime = lastTime + jobs[bestJob].runtime;
			return jobs[bestJob].status.finishTime;
		}
		return -1;
	}
	void assignJobs() { //last time is current time
		int orderedWorkers[100];
		int indexOfWorkers[100];
		int size = freeWorkers.size();
		if(shouldLog) 
			printf("before sorting\n");
		sortWorkers(freeWorkers.data(), orderedWorkers, indexOfWorkers, size, 100);
		if(shouldLog) {
			printf("size %d %d\n", size);
			for(int i=0; i < 18; i++)
	 			printf("%d %d\n", orderedWorkers[i], indexOfWorkers[i]);
			//exit(0);
		}
		for(int i=0; i < size; i++) {
			int finish = assignJob(orderedWorkers[i]);
			if(finish > -1) {
				if(shouldLog)
					printf("1\n");
				workersToBeFree[finish].push_back(orderedWorkers[i]);
				if(shouldLog)
					printf("2 size %d index %d\n", size, indexOfWorkers[i]);
				freeWorkers.erase(freeWorkers.begin() + indexOfWorkers[i]);
				if(shouldLog)
					printf("3\n");
			}
		}
		if(shouldLog)
			printf("3\n");
	}
	int processTo(int time) {
		int p=9;
		//iterate from lastTime to timeTo
		while(lastTime <= time) {
			for(int worker : workersToBeFree[lastTime])
				freeWorkers.push_back(worker);
			assignJobs();
			lastTime++;
		}
	}

	virtual void addWorker(int time, int id, int seniorId)
	{
		//printf("adding worker\n");
		workers[id] = seniorId;
		workersToBeFree[time].push_back(id);
		processTo(time);
		printf("add Worker    -----------------  %d %d %d\n", time, id, seniorId);
	}
	virtual void addWork(int time, int workId, int workerId, int importance, int runtime) 
	{
		jobs[workId] = { workerId, importance, runtime, {}};
		waitingJobs.push_back(workId);
		printf("adding work\n");
		if(time == 99849) {
			printf("shouldLog %d ---- turning on\n", shouldLog);
			shouldLog = true;
			for(int worker : freeWorkers) {
				printf("%d ", worker);
			}
			printf("\n" );
			//exit(0);
		}
		processTo(time);
		printf("add Work %d %d %d %d %d\n", time, workId, workerId, importance, runtime);
	}
	virtual Status getStatus(int time, int workId )
	{
		printf("getting status\n");
		processTo(time);
		printf("status %d %d: worker %d finished %d\n", time, workId, jobs[workId].status.workerAssigned, jobs[workId].status.finishTime);
		return jobs[workId].status;	
	}
};

//EfficientSolution solution;
NaiveSolution naiveSsolution;

#include<math.h>
int main() {
	//Judge::run(&naiveSsolution);
	
	//int source[] = {12, 3, 17, 2, 5, 13, 11, 10, 9, 7, 15, 4, 6, 14, 18, 20, 8, 16};
	vector<int> v = {12, 3, 17, 2, 5, 13, 11, 10, 9, 7, 15, 4, 6, 14, 18, 20, 8, 16};
	// int source[] = {12, 23, 1, 45, 13, 55};
	// printf("6 & 100 \n");
	sortVector(v, 18, 100);
	for(int i=0; i < 18; i++)
	 	printf("%d\n", v[i]);
	return 0;
}

