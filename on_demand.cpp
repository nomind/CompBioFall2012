#include "include.h"
#include "read.cpp"
#include "common.cpp"

using namespace std;

int blockSz = 5;
int errSz = 1;
int initScore = 3;
int jumpSz = 3;
int bval = 0;
int dval = -1;
int base = 5;

int getval(char c);
int rank(char *seq, int l);

int pow(int b, int p) {
	int ans = 1;
	for(int i=0; i<p; i++) {
		ans *= b;
	}
	return ans;
}

int min(int a, int b) {
	if(a<b) {
		return a;
	}
	return b;
}

int dist(char *r1, int n, char *r2, int m, int **mat) {
	int id1 = rank(r1, n);
	int id2 = rank(r2, m);
	
	for(int i=0; i<n+1; i++) {
		mat[i][0] = i;
	}
	
	for(int j=0; j<m+1; j++) {
		mat[0][j] = j;
	}
	
	for(int i=1; i<n+1; i++) {
		for(int j=1; j<m+1; j++) {
			if(r1[i-1] == r2[j-1]) {
				mat[i][j] = mat[i-1][j-1];
			} else {
				mat[i][j] = min(mat[i-1][j], mat[i][j-1]) + 1;
			}
		}
	}
	return mat[n][m];
}

int overlap(Read& r1, Read& r2, int **distBuf) {
	int l1 = r1.len;
	int l2 = r2.len;
	
	int bufSz = blockSz + errSz;
	
	for(int start=0; start<l1-blockSz; start+=jumpSz) {
		// try block wise matching starting from here in the r1
		int blockCnt = 0;
		//init the score
		int score = initScore;

		for(int i=start, j=0; j<l2-bufSz && i<l1-blockSz; i+=blockSz, blockCnt++) {
			//cout<<i<<" "<<j<<endl;
			// calculate distance matirx of the blocks
			/*dist(r1 + i, blockSz, r2 + j, bufSz, distBuf);
			
			// choose the best match in the blockSz +/- errSz
			int minErr = bufSz;
			int sz;
			for(int k=blockSz-errSz; k<blockSz+errSz+1; k++) {
				if(minErr > distBuf[blockSz][k]) {
					minErr = distBuf[blockSz][k];
					sz = k;
				}
			}*/
			
			int minErr = bufSz;
			int sz;
			int id1 = rank(r1+i, blockSz);
			for(int k=blockSz-errSz; k<blockSz+errSz+1; k++) {
				int id2 = rank(r2+j, k);
				if(distBuf[id1][id2] == dval) {
				    distBuf[id1][id2] = dist(r1 + i, blockSz, r2 + j, bufSz, distBuf);
				}
				
				if(minErr > distBuf[id1][id2]) {
					minErr = distBuf[id1][id2];
					sz = k;
				}
			}

			//cout<<"Error "<<minErr<<" "<<sz<<endl;
			// adjust from score the error
			score += errSz - minErr;
			j += sz;
			
			if(score < 0) {
				break;
			}
		}
		
		// found a good match
		if(score >=0) {
			return blockCnt * blockSz;
		}
	}
}

int getVal(char c) {
	switch(c) {
	case 'A':
		return 1;
	case 'C':
		return 2;
	case 'G':
		return 3;
	case 'T':
		return 4;
	default:
		assert(false);
	}
	return -1;
}

int rank(char *seq, int l) {
	/*for(int k=0; k<l; k++) {
		cout<<seq[k];
	}
	cout<<endl;
	*/
	int rank=0;
	for(int i=0; i<l; i++) {
		//cout<<rank<<" "<<getVal(seq[i])<<endl;
		rank = base*rank +  getVal(seq[i]);
	}
	//cout<<rank<<endl;
	return rank;
}

char getChar(int val) {
	switch(val) {
	case 1:
		return 'A';
	case 2:
		return 'C';
	case 3:
		return 'G';
	case 4:
		return 'T';
	default:
		assert(false);
	}
	return 'O';
}

int getSeq(char* str, int id) {
	int n = 0;
	//cout<<id<<endl;
	while(id>0) {
		int k = id%base;
		if(k == bval) {
			return -1;
		}
		char c = getChar(k);
		str[n++] = c;
		id /= base;
	}
	/*cout<<id<<" "<<n<<endl;
	for(int k=0; k<n; k++) {
		cout<<str[k];
	}
	cout<<endl;
	*/
	return n;
}

void init(int **table, int dim) {
	int bufSz = blockSz + errSz;
	char str1[bufSz], str2[bufSz];
	char buf1[bufSz], buf2[bufSz];
	int **distBuf = allocate2d(bufSz + 1, bufSz + 1);
	
	for(int k=0; k<blockSz; k++) {
		str1[k] = getChar(1);
	}
	int s1 = rank(str1, blockSz);
	for(int k=0; k<blockSz+1; k++) {
		str2[k] = getChar(1);
	}
	int e1 = rank(str2, blockSz+1);
	//cout<<s1<<" "<<e1<<endl;
	for(int i=s1; i<e1; i++) {
		int n = getSeq(buf1, i);
		if(n<0) {
			continue;
		}
		/*for(int k=0; k<n; k++) {
			cout<<buf1[k];
		}
		cout<<endl;*/
		
		table[i][i] = 0;
		
		for(int k=0; k<blockSz - errSz; k++) {
			str1[k] = getChar(1);
		}
		int s2 = rank(str1, blockSz - errSz);
		for(int k=0; k<blockSz + errSz + 1; k++) {
			str2[k] = getChar(1);
		}
		int e2 = rank(str2, blockSz + errSz + 1);
			
		for(int j=s2; j<e2; j++) {
			if(table[i][j] != -1) {
				continue;
			}
			
			int m = getSeq(buf2, j);
			if(m<0) {
				continue;
			}
			
			/*for(int k=0; k<m; k++) {
				cout<<buf2[k];
			}
			cout<<endl;*/
			
 			table[i][j] = dist(buf1, n, buf2, m, distBuf);
			table[j][i] = table[i][j];
			//cout<<table[i][j];
		}
	}
}

int main() {
	/* read all reads */
	vector<Read> readList;
	readReads("data/ecoli_1K/ecoli.reads.1k.readsim.30x.fasta", readList);
	
	
	/*char x1[10000];
	char *y1 = "TTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGTGGATTAAAAAAAGAGTGTCTGATAGCAGCTTCTGAACTGATTACCTGCCGTGAGTAAATTAAAATTTTATTGACTTAGGCACTAAATACTTTAACCAATATAGGCATAGCGCACAGACAGATAAAAATTACAGAGTACACAACATCCATGAAACGCATTAGCACCACCATTACCACCACCATCACCATTACCACAGGTAACGGTGCGGGCTGACGCGTACAGGAAACACAGAAAAAAGCCCGCACCTGACAGTGCGGGCTTTTTTTTTCGACCAAAGGTAACGAGGTAACAACCATGCGAGTGTTGAAGTTCGGCGGTACATCAGTGGCAAATGCAGAACGTTCTCTGCGTGTTGCCGATATTCTGGAAAGCAATGCCAGCAGGGGCAGGTGGCCACCGTCCTCTCTGCCCCCGCCAAAATCACCAACCACCTGGTGGCGATGATTGAAAAAACCATTAGCGGCCAGGATGCTTTACCCAATATCAGCGATGCCGAACGTATTTTTGCCGAACTTTTGACGGGACTCGCCGCCGCCCAGCCGGGGTTCCCGCCGGCGCAATTGAAAACTTTCGTCGATCAGGAATTTGCCCAAATAAAACATGTCCTGCATGGC";
	int z1 = strlen(y1);
	strcpy(x1, y1);
	Read r1(x1, z1);
	char x2[10000];
	char *y2 = "TTCATTCTGACTGCAACGGGCAACTATGTCTCTGTGTGGATTAAAAAAAGAGTGTCTGATAGAGCTTCTGAACTGGTTACATGCCGTGAGTAAATTAAAATTTTATTGACTTAGGTCACTAAATACTATTAACCAATATAGGCATAGCGCACAGACAGATAAAAATTACAGAGTACACAACATCCATGAAACGCATTAGCACCACCATTACCACCACCATCACCATTACCACAGGTAACGGTGCGGGCTGACGCGTACAGGAAACACAGAAAAAAGCCCGCACCTGACAGTGCGGCTTTTTTTTTCGACCAAAGGTTAACGAGGTAACAACCATGCGAGTGTTGAAGTTCGGCGGTACTCAGTGGCAAATGCAGAACGTTTTCTGCGTCGTTGCCGATATTCTGGAAAGCAATGCCAGGCAGGGGCAGGTGGCCACCGTCCTCTCTGCCCCCGCCAAAATCACCAACCACCTGGTGGCGATGATTGAAAAAACCATTAGCGGCCAGGATGCTTTACCCAATATCAGCGATGCCGAACGTATTTTTGCCGAACTTTTGACGGGACTCGCCGCCGCCCAGCCGGGGTTCCCGCTGGCGCAATTGAAAACTTTCGTCGATCAGGAATTTGCCCAAATAAAACATGTCCTGCATGGCATTAGTTTGTTGGGGCAGTGCCCGGATAGCATCAACGCTGCGCTGATTTGCCGTGGCGAGAAAATGTCGATGCCATTATGGCCGGCGTATTAGAAGCGCGCGGTCACAACGTTACTGTTGTCGATCCGGTCGAAAAACTGCTGGCAGTGCGGCATTACCTCGAATCTACCGTCGATATTGCTGAGTCCACCCGCCGTATTGCGGCAAGCCGCATTCCGGCTGACCACATGGTGCTGATGGCAGGTTTCACCGCCGGTAATGAAAAAGGCGAAACTGGTGGTGCTTGGACGCAACGGTTCCGACTACTCTGCTGCGGTGCTGGCTGCCTGTTTACGCGCCGATTGTTGCGAGATTTGGACGGACGTTGACGGGGTCTATACCTGCGACCCGCGTCAGGTGCCCGATGGAGGTTGTTGAAGTCGATGTCCTACCAGGAAGCGATGGAGCTTTCCTACTTCGGCGCTAAAGTTCTTCACCCCCGCACCATTACCCCCATCGCCCAGTTCCAGATCCCTTGCCTGATTAAAAATACCGGAAATCCTCAAGCACCAGGTACGCTCATTGGTGCCAGCCGTGATGAAGACGAATTACCGGTCAAGGGCATGTTCCAATCTGAATAACATGGCAATGTTCAGCGTTTCTGGTCCGGGGATGAAAGGGATGGTCGGCATGGCGGCGCGCGTCTTTGGCAGCGATGTCACGCGCCCGTATTTCCGTGGTGCTGATTACGCAATCATCTTCCGAATACAGCATCAGTTTCTGCGTTCCACAAAGCGACTGTGTTGCGAGCTGAACGGGCAATGCAGGAAGAGTTCTACCTGGAACTGAAAGAAGGCTTACTGGAGCCGCTGGCAGTGACGGAACGGCTGGCCATTATCTCGGTGGTAGGTGATGGTATGCGCACCTTGCGTGGGATCTCTGGCCGAAATTCTTTGCCCACTGGCCCGCGCCAATATCAACATTGTCGCCATTGCTCAGGGATCTTCTGAACGCTCAATCTCTGTCGTGGTAAATAACGATGATGCGACCACTGGCGTGCGCGTTACTCATCAGATGCTGTTCAATACCGATCAGGTTATCGAAGTGTTTGTGATTGGCGTCGGTGGCGTTGGCGGTGCGCTGCTGGAGCAACTGGAAGCGTCAGCAAAGCTGGCTGAAGAATAAACATATCGACTTACGTGTCTGCGGTGTTGCCAACTCGAAGGCTCTGCTCACCAATGTACATGGCCTTAATCTGGAAAACTGGCAGGAAGAACTGGCGCAAGCCAAAGAGCCGTTTAATCTCGGGCGCTTAATTCGCCTCGTGAAAGAATATCATCTGCTGAACCCGGTCATTGTAGACTGCACTTCCAGCCAGGCAGTGGCGGATCAATATGCCGA";
	int z2 = strlen(y2);
	strcpy(x2, y2);
	Read r2(x2, z2);
	readList.push_back(r1);
	readList.push_back(r2);*/

	// for every pair of Reads calculate the overlap 
	/*int sz = readList.size();
	int **mat = allocate2d(sz, sz);
	int bufSz = blockSz + errSz;
	int **distBuf = allocate2d(bufSz + 1, bufSz + 1);
	for(int i=0; i<sz; i++) {
		for(int j=i+1; j<sz; j++) {
			//cout<<i<<" "<<j<<endl;
			mat[i][j] = overlap(readList[i], readList[j], distBuf);
			//cout<<mat[i][j]<<endl;
		}
	}*/

	//Init the table
	int dim = pow(base, blockSz+errSz);
	int **table = allocate2d(dim+2, dim+2);
	for(int i=0; i<dim; i++) {
		for(int j=0; j<dim; j++) {
			table[i][j] = dval;
		}
	}
	
	//init(table, dim);
	
	int sz = 500;
	int **mat = allocate2d(sz, sz);
	for(int i=0; i<sz; i++) {
		for(int j=i+1; j<sz; j++) {
			//cout<<i<<" "<<j<<endl;
			mat[i][j] = overlap(readList[i], readList[j], table);
			//cout<<mat[i][j]<<endl;
		}
	}

	//cout<<stddev("../data/ecoli_1K/overlap.afg", mat, sz, readList)<<endl;

	return 0;
}

