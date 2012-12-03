#include "include.h"

#include "read.cpp"

using namespace std;

int main() {
    /* read all reads */
	vector<Read> readList;
	readReads("../data/ecoli_1K/ecoli.reads.1k.readsim.30x.fasta", readList);

    return 0;
}

