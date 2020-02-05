struct Status {
	int finishTime;
	int workerAssigned;
};

class ISolution {
public:
	virtual void addWorker(int time, int id, int seniorId) = 0;
	virtual void addWork(int time, int workId, int workerId, int importance, int runtime) = 0;
	virtual Status getStatus(int time, int workId ) = 0;
};

class Judge
{
public:
	static void run(ISolution *solution);
};

