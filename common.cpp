
#include "include.h"

int** allocate2d(int n, int m) {
	int **buf = new int*[n];
	for(int i=0; i<n; i++) {
		buf[i] = new int[m];
	}
	
	return buf;
}

