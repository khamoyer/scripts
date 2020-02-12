#include <stdio.h>
#include "judge.h"
#include "naive.h"
#include "semiNaive.h"


//EfficientSolution solution;
SemiNaiveSolution semiNaiveSolution;
NaiveSolution naiveSolution;

int main() {
	Judge::run(&semiNaiveSolution);
	// Judge3::run(new NaiveSolution());
	// Judge4::run(new NaiveSolution());
	// Judge5::run(new NaiveSolution());
	return 0;
}

