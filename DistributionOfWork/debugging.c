// void printWorkersDepth(NaiveSolution *naiveSolution, int parent) {
// 	int children[100];
// 	int size=0;
// 	for(int i=1; i<100; i++)
// 		if(naiveSolution->workers[i] == parent)
// 			children[size] = i, size++;
// 	if(size > 0) {
// 		printf("Children of worker %d:\n", parent);
// 		for(int i=0; i<size; i++)
// 			printf("%d ", children[i]);
// 		printf("\n");
// 		for(int i=0; i<size; i++)
// 			printWorkersDepth(naiveSolution, children[i]);
// 	}
// }

// void printWorkersBredth(NaiveSolution *naiveSolution) {
// 	int queue[200];
// 	int from = 0; int to = 0;
// 	queue[to]=1;
// 	to++;
// 	while(from < to) {
// 		int current = queue[from++];
// 		int children[100];
// 		int size=0;
// 		{
// 			for(int i=1; i<100; i++)
// 				if(naiveSolution->workers[i] == current)
// 					children[size] = i, size++;	
// 		}
// 		if(size > 0) {
// 				printf("Children of worker %d:\n", current);
// 			for(int i=0; i<size; i++) {
// 				printf("%d ", children[i]);
// 				queue[to++] = children[i];
// 			}
// 			printf("\n");
// 		}
// 	}
// }

// void printWorkers(NaiveSolution *naiveSolution) {
// 	//printWorkersDepth(naiveSolution, 1);
// 	printWorkersBredth(naiveSolution);
// }

// void sortWorkers(int *source, int *target, int *indexOfSource, int count, int range) {
// 	int *counter = new int[range];
// 	for(int i=0; i < range; i++) 
// 		counter[i] = -1;
// 	for(int i=0; i < count; i++)
// 		counter[source[i]] = i;
	
// 	for(int s=0, t=0; s < range; s++) {
// 		if(counter[s] > -1) {
// 			target[t] = s;
// 			indexOfSource[t] = counter[s];
// 			t++;
// 		}
// 	}
// 	delete counter;
// }

	
	//vector<int> v = {12, 3, 17, 2, 5, 13, 11, 10, 9, 7, 15, 4, 6, 14, 18, 20, 8, 16};
	// sortVector(v, 100);
	// for(int i=0; i < 18; i++)
	//  	printf("%d\n", v[i]);

// void Judge3::run(ISolution *solution) {
// 	auto start = high_resolution_clock::now();
// 	solution->addWorker(2, 2, 1);
// 	solution->addWorker(3, 3, 1);
// 	solution->addWork(4, 1, 2, 24, 10);
// 	solution->addWork(5, 2, 2, 13, 10);
// 	solution->addWork(6, 3, 3, 25, 10);
// 	showStatus(solution, 7, 1);
// 	showStatus(solution, 8, 2);
// 	showStatus(solution, 9, 3);
// 	solution->addWork(10, 4, 1, 12, 10);
// 	solution->addWork(11, 5, 1, 26, 10);
// 	solution->addWork(12, 6, 1, 25, 10);
// 	showStatus(solution, 13, 4);
// 	showStatus(solution, 14, 5);
// 	showStatus(solution, 15, 6);
// 	solution->addWork(16, 7, 1, 2, 10);
// 	solution->addWork(17, 8, 1, 22, 10);
// 	solution->addWork(18, 9, 1, 21, 10);
// 	showStatus(solution, 33, 4);
// 	showStatus(solution, 34, 5);
// 	showStatus(solution, 35, 6);
// 	showStatus(solution, 36, 7);
// 	showStatus(solution, 37, 8);
// 	showStatus(solution, 38, 9);
	
// 	auto stop = high_resolution_clock::now();
// 	auto duration = duration_cast<milliseconds>(stop - start);
// 	printf("result: %d, duration: %d\n", result, duration);
// }