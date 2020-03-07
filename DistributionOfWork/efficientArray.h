#include "judge.h"
using namespace std;

template<typename T>
class Heap : public T {
public:
	int size=0;
	void push(int value) {
		size++;
		T::data[size] = value;
	}
	bool isAny() {
		return size > 0;
	}
	int peek() {
		if(isAny()) {
			int exIdx = 1;
			for(int i=1; i <= size; i++)
				if(T::compare(i, exIdx))
					exIdx = i;
			return T::data[exIdx];
		}
		else
			return 0;
	}
	int pop() {
		int exIdx = 1;
		for(int i=1; i <= size; i++)
			if(T::compare(i, exIdx))
				exIdx = i;
		int result = T::data[exIdx];
		remove(exIdx);
		return result;
	}
	void remove(int idx) {
		if(idx <= size) {
			T::data[idx] = T::data[size];
			size--;
		}
	}
};

class HeapMin {
public:
	int data[101];
	int compare(int i, int j){
		return data[i] < data[j];
	}
};

struct Job {
	int workerId;
	int importance;
	int runtime;
	Status status;
};
Job Jobs[70001];

int jobCompare(int job1Idx, int job2Idx) {
	if(Jobs[job1Idx].importance == Jobs[job2Idx].importance)
		return job1Idx < job2Idx;
	return Jobs[job1Idx].importance > Jobs[job2Idx].importance;
}

class HeapJob {
public:
	int data[70001];
	int compare(int i, int j){
		return jobCompare(data[i], data[j]);
	}
};

struct Worker {
	Heap<HeapMin> freeChildren; //own children with him itself
	Heap<HeapJob> jobs;
	int senior; //seniorId
	int finishTime; //needed for processing
};
Worker Workers[101];

class HeapBusy {
public:
	int data[101];
	int compare(int i, int j){
		int worker1Idx = data[i];
		int worker2Idx = data[j];
		if(Workers[worker1Idx].finishTime == Workers[worker2Idx].finishTime)
			return worker1Idx < worker2Idx;
		return Workers[worker1Idx].finishTime < Workers[worker2Idx].finishTime;
	}
};

class EfficientArraySolution : public ISolution {
	Heap<HeapBusy> busyWorkers;
public:
	EfficientArraySolution() {
		Jobs[0].importance = -1;
		for(int i = 0; i < 101; i++)
			Workers[i] = {};
		addWorker(0, 1, 0);
	}
	void assign(int time, int wIdx, int jIdx) {
		Jobs[jIdx].status.workerAssigned = wIdx;
		Jobs[jIdx].status.finishTime = Jobs[jIdx].runtime + time;
		Workers[wIdx].finishTime = Jobs[jIdx].status.finishTime;
		busyWorkers.push(wIdx);
	}
	void processTo(int time) {
		while(busyWorkers.isAny()) {
			int worker = busyWorkers.peek();
			if(Workers[worker].finishTime > time)
				break;
			busyWorkers.pop();
			int senior = Workers[worker].senior;
			if(Workers[worker].jobs.isAny() || Workers[senior].jobs.isAny()) {
				if(jobCompare(Workers[worker].jobs.peek(), Workers[senior].jobs.peek()))
					assign(Workers[worker].finishTime, worker, Workers[worker].jobs.pop());
				else
					assign(Workers[worker].finishTime, worker, Workers[senior].jobs.pop());
			}
			else
			{
				Workers[worker].freeChildren.push(worker);
				Workers[senior].freeChildren.push(worker);
			}
		}
	}
    virtual void addWorker(int time, int id, int seniorId)
	{
		//making it busy by default
		Workers[id].senior = seniorId;
		Workers[id].finishTime = time;
		busyWorkers.push(id);
		processTo(time);
	}
	virtual void removeFromParentFreeChildren(int worker)
	{
		int senior = Workers[worker].senior;
		int idx = 0;
		for(int w : Workers[senior].freeChildren.data) {
			if(w == worker) {
				Workers[senior].freeChildren.remove(idx);
				break;
			}
			idx++;
		}
	}
	virtual void addWork(int time, int job, int worker, int importance, int runtime)
	{
		processTo(time - 1);
		Jobs[job] = {worker, importance, runtime, {}};
		Workers[worker].jobs.push(job);
		if(Workers[worker].freeChildren.isAny()) {
			int waitingWorker = Workers[worker].freeChildren.pop();
			Workers[waitingWorker].finishTime = time;
			busyWorkers.push(waitingWorker);
			
			if(waitingWorker == worker)
				removeFromParentFreeChildren(waitingWorker);
			else
				Workers[waitingWorker].freeChildren.pop(); //we are sure it is on top
		}
		processTo(time);
	}
	virtual Status getStatus(int time, int workId )
	{
		processTo(time);
        return Jobs[workId].status;
	}
};