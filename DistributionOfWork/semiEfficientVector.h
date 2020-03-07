#include "judge.h"
using namespace std;

template<class T>
class Vector {
public:
	vector<T> data;
	void push(T value) {
		data.push_back(value);
	}
	void remove(int idx) {
		data.erase(data.begin()+idx);
	}
};

class Heap {
public:
	Vector<int> data;
	virtual int compare(int i, int j) = 0;
	void push(int value) {
		data.push(value);
	}
	bool isAny() {
		return data.data.size() > 0;
	}
	int peek() {
		if(isAny()) {
			int exIdx = 0;
			int i=0;
			for(int n : data.data) {
				if(compare(i, exIdx))
					exIdx = i;
				i++;
			}
			return data.data[exIdx];
		}
		else
			return 0;
	}
	int pop() {
		int exIdx = 0;
		int i=0;
		for(int n : data.data) {
			if(compare(i, exIdx))
				exIdx = i;
			i++;
		}
		int result = data.data[exIdx];
		data.remove(exIdx);
		return result;
	}
	void remove(int idx) {
		if(idx < data.data.size())
			data.data.erase(data.data.begin() + idx);
	}
};

class HeapMin : public Heap {
public:
	virtual int compare(int i, int j){
		return data.data[i] < data.data[j];
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

class HeapJob : public Heap {
public:
	virtual int compare(int i, int j){
		return jobCompare(data.data[i], data.data[j]);
	}
};

struct Worker {
	HeapMin freeChildren; //own children with him itself
	HeapJob jobs;
	int senior; //seniorId
	int finishTime; //needed for processing
};
Worker Workers[101];

class HeapBusy : public Heap {
public:
	virtual int compare(int i, int j){
		int worker1Idx = data.data[i];
		int worker2Idx = data.data[j];
		if(Workers[worker1Idx].finishTime == Workers[worker2Idx].finishTime)
			return worker1Idx < worker2Idx;
		return Workers[worker1Idx].finishTime < Workers[worker2Idx].finishTime;
	}
};

class SemiEfficientVectorSolution : public ISolution {
	HeapBusy busyWorkers;
public:
	SemiEfficientVectorSolution() {
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
		for(int w : Workers[senior].freeChildren.data.data) {
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