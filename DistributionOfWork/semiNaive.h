#include "judge.h"
using namespace std;


class SemiNaiveSolution : public ISolution {
	struct Job {
		int workerId;
		int importance;
		int runtime;
		Status status;
	};
	vector<int> freeWorkers;
	vector<int> workersToBeFree[2000001]; //workers that becomes free at this time
	//vector<int> waitingJobs;
	int lastTime;
public:
	Job jobs[70001];
	int workers[100];
	vector<int> workerJobs[100];
	SemiNaiveSolution() {
		for(int i=0; i<100; i++)
		 	workers[i] = 0;
		lastTime = 0;
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
	struct BestJob {
		int idx;
		int job;
	};
	int compare(int job1, int job2) {
		return (jobs[job1].importance == jobs[job2].importance && job1 < job2)
				|| (jobs[job1].importance > jobs[job2].importance);
	}
	BestJob getBestJobIdx(int workerId) {
		int bestJob = -1;
		int i=0, idxOfBestJob = -1;
		if(workerId > 0) {
			for(int job : workerJobs[workerId]) {
				if(bestJob == -1 || compare(job, bestJob)) 
					bestJob = job, idxOfBestJob = i;
				i++;
			}
		}
		return {idxOfBestJob, bestJob};
	}
	int assignJobCore(int workerId, vector<int> *workerJobs, BestJob bestJob) {
		workerJobs->erase(workerJobs->begin() + bestJob.idx);
		jobs[bestJob.job].status.workerAssigned = workerId;
		jobs[bestJob.job].status.finishTime = lastTime + jobs[bestJob.job].runtime;
		return jobs[bestJob.job].status.finishTime;
	}
	int assignJob(int workerId) { //returns finishTime
		BestJob workerJob = getBestJobIdx(workerId);
		int parentId = workers[workerId];
		BestJob parentJob = getBestJobIdx(parentId);
		if(workerJob.job >= 0 && parentJob.job >= 0) {
			if(compare(workerJob.job, parentJob.job))
				return assignJobCore(workerId, &workerJobs[workerId], workerJob);	
			else 
				return assignJobCore(workerId, &workerJobs[parentId], parentJob);
		}
		else if(workerJob.job >= 0 )
			return assignJobCore(workerId, &workerJobs[workerId], workerJob);
		else if(parentJob.job >= 0 )
			return assignJobCore(workerId, &workerJobs[parentId], parentJob);
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
		while(lastTime <= time - 1) {
			if(workersToBeFree[lastTime].size() > 0) {
                sortVector(workersToBeFree[lastTime], 100);
                for(int worker : workersToBeFree[lastTime]) {
                    int finish = assignJob(worker);
                    if(finish > -1)
                        workersToBeFree[finish].push_back(worker);
                    else
                        freeWorkers.push_back(worker);
                }
            }
			lastTime++;
		}
        if(lastTime <= time) {
            for(int worker : workersToBeFree[lastTime])
				freeWorkers.push_back(worker);
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
		workerJobs[workerId].push_back(workId);
		//waitingJobs.push_back(workId);
		processTo(time);
	}
	virtual Status getStatus(int time, int workId )
	{
		processTo(time);
		return jobs[workId].status;	
	}
};