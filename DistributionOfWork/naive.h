#include "judge.h"
using namespace std;

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
		for(int i=0; i<100; i++)
		 	workers[i] = 0;
		lastTime = 1;
		addWorker(0, 1, 0);
	}
	void sortVector(vector<int> &v, int range) {
        int count = v.size();
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
        delete[] counter;
    }
	int assignJob(int workerId) { //returns finishTime
		int parentId = workers[workerId];
		if(parentId == 0)
			parentId = workerId;
		int bestJob = -1;
		int i=0, idxOfBestJob=-1;
		for(int job : waitingJobs) {
			if(jobs[job].workerId == workerId || jobs[job].workerId == parentId )
				if(bestJob == -1
					|| (jobs[job].importance == jobs[bestJob].importance && job < bestJob)
					|| (jobs[job].importance > jobs[bestJob].importance)) 
				{
					bestJob = job, idxOfBestJob = i;
				}
			i++;
		}
		if(bestJob > -1) {
			waitingJobs.erase(waitingJobs.begin() + idxOfBestJob);
			jobs[bestJob].status.workerAssigned = workerId;
			jobs[bestJob].status.finishTime = lastTime + jobs[bestJob].runtime;
			return jobs[bestJob].status.finishTime;
		}
		return -1;
	}
	void assignJobs() { //last time is current time
		sortVector(freeWorkers, 100);
		vector<int> busyWorkers;
		for(int i=0, size = freeWorkers.size(); i < size; i++) {
			int finish = assignJob(freeWorkers[i]);
			if(finish > -1) {
				workersToBeFree[finish].push_back(freeWorkers[i]);
				busyWorkers.push_back(i);
			}
		}
		//removing from the end
		for(int i = busyWorkers.size() - 1; i >= 0; i--)
			freeWorkers.erase(freeWorkers.begin() + busyWorkers[i]);
	}
	int processTo(int time) {
		while(lastTime <= time) {
			for(int worker : workersToBeFree[lastTime])
				freeWorkers.push_back(worker);
			if(workersToBeFree[lastTime].size() > 0 //freeWorkers.size() > oldSize
				|| lastTime == time)
				assignJobs();
			lastTime++;
		}
	}

	virtual void addWorker(int time, int id, int seniorId)
	{
		workers[id] = seniorId;
		processTo(time - 1);
		freeWorkers.push_back(id);
		processTo(time);
	}
	virtual void addWork(int time, int workId, int workerId, int importance, int runtime) 
	{
		processTo(time - 1);
		jobs[workId] = { workerId, importance, runtime, {}};
		waitingJobs.push_back(workId);
		processTo(time);
	}
	virtual Status getStatus(int time, int workId )
	{
		processTo(time);
		return jobs[workId].status;	
	}
};
