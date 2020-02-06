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
