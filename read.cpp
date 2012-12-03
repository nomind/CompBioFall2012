
#include "include.h"

class Read {
public:
	int len;
	char *read;
	
	Read(char *read, int len) {
		this->read = read;
		this->len = len;
		this->read[len] = '\0';
	}
	
	char* operator +(int i) {
		return read + i;
	}
};

void readReads(char *filename, vector<Read>& readList) {
	int max = 20000;
	ifstream f;
	f.open(filename, ios::in);
	Read t(new char[1024], 0);
	readList.push_back(t);
	int sz = 0;
	while(!f.eof()) {
		char *read = new char[max];
		f.getline(read, max);
		f.getline(read, max);
		
		Read r(read, f.gcount());
		//cout<<read<<endl;
		readList.push_back(r);

		if(sz++ > 100) {
			break;
		}
	}
	return;
}

int abs(int v) {
	if(v>=0) {
		return v;
	}
	return -1*v;
}

void readOverlaps(char *filename, int **mat, int sz, vector<Read>& readList) {
	ifstream f;
	f.open(filename, ios::in);
	
	int len = 20;
	char str[len];
	while(!f.eof()) {
		int id1, id2;
		int sl1, sl2;
		f.getline(str, len);
		f.getline(str, len);
		f.getline(str, len);
		//cout<<str<<endl;
		sscanf(str, "rds:%d,%d", &id1, &id2);
		//cout<<id1<<" "<<id2<<endl;
		//cout<<readList[id1].len<<endl;
		//cout<<readList[id2].len<<endl;
		f.getline(str, len);
		f.getline(str, len);
		sscanf(str, "ahg:%d", &sl1);
		f.getline(str, len);
		sscanf(str, "bhg:%d", &sl2);
		
		int o1 = readList[id1].len - abs(sl1), o2 = readList[id2].len - abs(sl2);
		//cout<<id1<<" "<<o1<<" "<<id2<<" "<<o2<<endl;
		//assert(o1 == o2);
		int o = max(o1, o2);
		int my = mat[id1][id2];
		//cout<<o<<" "<<my<<endl;
		//assert(0 <= my + blockSz && o >= my -blockSz);
		f.getline(str, len);
	}
	return;
}

