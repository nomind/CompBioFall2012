
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
	Read t(new char[max], 0);
	readList.push_back(t);
	int sz = 0;
	while(!f.eof()) {
		char *read = new char[max];
		f.getline(read, max);
		f.getline(read, max);
		
		Read r(read, f.gcount());
		//cout<<read<<endl;
		readList.push_back(r);

		/*if(sz++ > 100) {
			break;
		}*/
	}
	return;
}


