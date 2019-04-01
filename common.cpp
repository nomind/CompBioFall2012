
#include "include.h"

// This is a dummy commit 

int** allocate2d(int n, int m) {
	int **buf = new int*[n];
	for(int i=0; i<n; i++) {
		buf[i] = new int[m];
	}
	
	return buf;
}

int abs(int v) {
	if(v>=0) {
		return v;
	}
	return -1*v;
}

double stddev(char *filename, int **mat, int sz, vector<Read>& readList) {
	ifstream f;
	f.open(filename, ios::in);
	
	int len = 20, count = 0;
	int ok_count = 0;
	char str[len];
    double total = 0;
	while(!f.eof()) {
		int id1, id2;
		int sl1, sl2;
		f.getline(str, len);
		f.getline(str, len);
		f.getline(str, len);
		//cout<<str<<endl;
		sscanf(str, "rds:%d,%d", &id1, &id2);
        if(id1 >= sz || id2 >= sz) {
            continue;
        }
		//cout<<id1<<" "<<id2<<endl;
		//cout<<readList[id1].len<<endl;
		//cout<<readList[id2].len<<endl;
		f.getline(str, len);
		f.getline(str, len);
		sscanf(str, "ahg:%d", &sl1);
		f.getline(str, len);
		sscanf(str, "bhg:%d", &sl2);
		if(sl1 < 0 || sl2 < 0) {
		    continue;
		}
		//cout<<readList[id1].len<<" "<<abs(sl1)<<" "<<readList[id2].len<<" "<<abs(sl2)<<endl;
		int o1 = readList[id1].len - sl1, o2 = readList[id2].len - sl2;
		//cout<<sz<<" "<<id1<<" "<<o1<<" "<<id2<<" "<<o2<<endl;
		//assert(o1 == o2);
		int o = max(o1, o2);
		int my = mat[id1][id2];
        count++;
        double delta = (1.0*abs(o-my))/o;
        if(delta <= 0.01) {
            ok_count++;
        }
        total += delta;
		//cout<<o<<" "<<my<<endl;
		//assert(0 <= my + blockSz && o >= my -blockSz);
		f.getline(str, len);
	}
	cout<<ok_count<<" "<<count<<endl;
	return total/count;
}

