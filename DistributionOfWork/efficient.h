#include "judge.h"
using namespace std;

template<typename T>
class Heap : public T {
	void swap(int i, int j) {
		int t = T::data[i];
		T::data[i] = T::data[j];
		T::data[j] = t;

		T::position[T::data[i]] = i;
		T::position[T::data[j]] = j;
	}
	void up(int i){
		while(i>1 && T::compare(i, i/2)) {
			swap(i, i/2);
			i=i/2;
		}
	}
	void down(int i) {
		int rightChild = i*2 + 1;
		int smallerChild = i*2;
		if(rightChild <= size) {
			smallerChild = T::compare(smallerChild, rightChild) 
				? smallerChild : rightChild;
		}
		if(smallerChild <= size) 
			if(T::compare(smallerChild, i)) {
				swap(i, smallerChild);
				down(smallerChild);
			}
	}
public:
	int size=0;
	bool isAny() {
		return size > 0;
	}
	int peek() {
		return isAny() ? T::data[1] : 0;
	}
	void push(int value) {
		size++;
		T::data[size] = value;
		T::position[value] = size;
		up(size);
	}
	int pop(){
		int r = T::data[1];
		swap(1, size);
		size--;
		down(1);
		return r;
	}
	void remove(int value) {
		int i = T::position[value];
		swap(i, size);
		size--;
		up(i);
		down(i);
	}
};

class HeapMin {
public:
	int data[101];
	int position[101];
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
	int position[70001];
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
	int position[101];
	int compare(int i, int j){
		int worker1Idx = data[i];
		int worker2Idx = data[j];
		if(Workers[worker1Idx].finishTime == Workers[worker2Idx].finishTime)
			return worker1Idx < worker2Idx;
		return Workers[worker1Idx].finishTime < Workers[worker2Idx].finishTime;
	}
};

class EfficientSolution : public ISolution {
	Heap<HeapBusy> busyWorkers;
public:
	EfficientSolution() {
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
		Workers[senior].freeChildren.remove(worker);
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
				Workers[waitingWorker].freeChildren.pop();
		}
		processTo(time);
	}
	virtual Status getStatus(int time, int workId )
	{
		processTo(time);
        return Jobs[workId].status;
	}
};