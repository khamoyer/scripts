#include "judge.h"
using namespace std;

class SemiEfficientSequentialSolution : public ISolution {
	struct Job {
		int workerId;
		int importance;
		int runtime;
		Status status;
	};
	vector<int> workersToBeFree[2000001]; //workers that becomes free at this time
	int lastTime;
public:
	Job jobs[70001];
	int seniors[100];
	bool workerStates[100]; //true free, false busy
	vector<int> workerJobs[100];
	vector<int> workerChildren[100]; //de facto free workres
	SemiEfficientSequentialSolution() {
		for(int i=0; i<100; i++) {
		 	seniors[i] = 0;
			workerStates[i] = false;
		}
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
	void assignJobCore(int workerId, vector<int> *workerJobs, BestJob bestJob) {
		workerJobs->erase(workerJobs->begin() + bestJob.idx);
		jobs[bestJob.job].status.workerAssigned = workerId;
		jobs[bestJob.job].status.finishTime = lastTime + jobs[bestJob.job].runtime;
		int finish = jobs[bestJob.job].status.finishTime;
		workersToBeFree[finish].push_back(workerId);
	}
	int assignJob(int workerId) { //returns finishTime
		BestJob workerJob = getBestJobIdx(workerId);
		int parentId = seniors[workerId];
		BestJob parentJob = getBestJobIdx(parentId);
		if(workerJob.job >= 0 && parentJob.job >= 0) {
			if(compare(workerJob.job, parentJob.job)) {
				assignJobCore(workerId, &workerJobs[workerId], workerJob);
				return 1;
			}
			else {
				assignJobCore(workerId, &workerJobs[parentId], parentJob);
				return 1;
			}
		}
		else if(workerJob.job >= 0 ) {
			assignJobCore(workerId, &workerJobs[workerId], workerJob);
			return 1;
		}
		else if(parentJob.job >= 0 ) {
			assignJobCore(workerId, &workerJobs[parentId], parentJob);
			return 1;
		}
		return -1;
	}
	void markAsFree(int worker) {
		workerStates[worker] = true;
		if(seniors[worker] > 0)
			workerChildren[seniors[worker]].push_back(worker);
	}
	void markAsBusy(int worker) {
		workerStates[worker] = false;
		int senior = seniors[worker];
		if(senior > 0) {
			int childIdx = indexOf(workerChildren[senior], worker);
			workerChildren[senior].erase(workerChildren[senior].begin() + childIdx);
		}
	}
	template<class T>
	int indexOf(vector<T> &vector, T value) {
		int i = 0;
		for(T t : vector) {
			if(t == value)
				return i;
			i++;
		}
		return -1;
	}
	int getBestFreeWorkerForGroup(int worker) {
		if(workerStates[worker] == true)
			return worker;
		else if(workerChildren[worker].size() > 0) {
			sortVector(workerChildren[worker], 100);
			return workerChildren[worker][0];
		}
		return -1;
	}
	int processWorkersThatBecomesFree(int time) {
		while(lastTime <= time) {
			if(workersToBeFree[lastTime].size() > 0) {
				sortVector(workersToBeFree[lastTime], 100);
				for(int worker : workersToBeFree[lastTime]) {
					if(assignJob(worker) == -1)
						markAsFree(worker);
				}
			}
			lastTime++;
		}
	}
	virtual void addWorker(int time, int id, int seniorId)
	{
		seniors[id] = seniorId;
		workersToBeFree[time].push_back(id);
		processWorkersThatBecomesFree(time);
	}
	virtual void addWork(int time, int workId, int workerId, int importance, int runtime) 
	{
		processWorkersThatBecomesFree(time - 1);
		jobs[workId] = { workerId, importance, runtime, {}};
		workerJobs[workerId].push_back(workId);
		
		int waitingWorker = getBestFreeWorkerForGroup(workerId);
		if(waitingWorker >= 0) {
			markAsBusy(waitingWorker);
			workersToBeFree[time].push_back(waitingWorker);
		}
		processWorkersThatBecomesFree(time);
	}
	virtual Status getStatus(int time, int workId )
	{
		processWorkersThatBecomesFree(time);
		return jobs[workId].status;	
	}
};