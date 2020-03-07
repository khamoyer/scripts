#include <stdio.h>
#include "judge.h"

//the best sequence: efficient (real heap, arrays, templates), efficient array (as previous but without heap), semi efficient sequential (iterating thru all times from 1 to 2000000), semi efficient Vector (similar as efficient array but instead of array is vector, and instead of templates are virtuals)
#include "efficient.h"
//#include "efficientArray.h"
//#include "semiEfficientSequential.h"
//#include "semiEfficientVector.h"
#include "naive.h"

//EfficientSolution efficientSolution;
//NaiveSolution naiveSolution;

int main() {
	for(int i=0; i < 20; i++) {
		auto s1 = new NaiveSolution();
		auto s2 = new EfficientSolution();
		//auto s2 = new EfficientArraySolution();
		//auto s2 = new SemiEfficientSequentialSolution();
		//auto s2 = new SemiEfficientVectorSolution();
		int r1 = Judge::runCore(s1, 69999, 22213133 + i);
		int r2 = Judge::runCore(s2, 69999, 22213133 + i);
		delete s1;
		delete s2;
		if(r1 != r2)
			printf("\n\n\n\n\nWrong\n\n\n\n\n\n");
	}
	//Judge::run(&efficientSolution);
	//Judge::run(&naiveSolution);
	return 0;
}