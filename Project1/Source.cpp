#include <iostream>
#include <bitset>
#include <string>
#include <queue>
#include <istream>
#include <fstream>

using namespace std;

bitset<26> solution_array[33554432];
queue<bitset<26>> nodeCue;
int numsolutionFound;
queue<int> intcue;
bitset<26> flip_position(int i, bitset<26> parent);
void solution_builder();
void showMap(bitset<26>);

void main() {
	numsolutionFound = 0;
	solution_array[0].set(25);
	solution_array[33554431].set(25);
	cout << solution_array[0]<<endl;
	cout << solution_array[33554431] << endl;
	nodeCue.push(bitset<26>(0));
	nodeCue.push(bitset<26>(33554431));
	solution_builder();
	cout << "number of soulutions found " << numsolutionFound << endl;
	
	/*for (int i = 0; i < 33554432; i++) {
		cout << "parent" << endl;
		showMap(solution_array[i]);
		cout << "Child" << endl;
		showMap(bitset<26>(i));
	}*/
}

void solution_builder() {
	char x;
	while (!nodeCue.empty()) { 
		//cout << "number inline    >>> " << nodeCue.size() << endl;
		bitset<26> parent = nodeCue.front();
		nodeCue.pop();
		
		//cout << "parent ++++++++++++++++++++++++++++++++++++++" << endl;
		//showMap(parent);
		//cout << "number inline    >>> " << nodeCue.size() << endl;
		int oldline = nodeCue.size();
		//cin >> x;
		int numDups = 0;
		int numNew = 0;
		for (int i = 0; i < 25; i++) {
			
			bitset<26> childSet;
			childSet = flip_position(i, parent);
			//cout << "Child" << endl;
			//showMap(childSet);
			if (!solution_array[childSet.to_ulong()].any()) {
				solution_array[childSet.to_ulong()] = parent;
				numsolutionFound++;
				nodeCue.push(childSet);
				//cout << "pushed to cue "<< endl;
				numNew++;
			}
			else{ 
				//cout << "dup found=============" << endl;
				numDups++;
			}
		}
		//cout << "number Duplicates>>> " <<numDups<< endl;
		//cout << "number New       >>> " <<numNew << endl;
		//cout << "number inline old>>> " <<oldline<< endl;
	}
}


bitset<26> flip_position(int i, bitset<26> parent) {
	bitset<26> childSet;
	childSet = parent;
	//cout << "position change " << i;
	int j = i;
	j += 5;
	while (j < 25) {
		childSet.flip(j);
		j += 5;
	}
	j = i;
	j -= 5;
	while (j > -1) {
		childSet.flip(j);
		j -= 5;
	}
	if (i>= 0 && i<5) {
		for (int t = 0; t < 5; t++) {
			childSet.flip(t);
		}
	}
	else if (i >= 5 && i < 10) {
		for (int t = 5; t < 10; t++) {
			childSet.flip(t);
		}
	}
	else if (i >= 10 && i < 15) {
		for (int t = 10; t < 15; t++) {
			childSet.flip(t);
		}
	}
	else if (i >= 15 && i < 20) {
		for (int t = 15; t < 20; t++) {
			childSet.flip(t);
		}
	}
	else if (i >= 20 && i < 25) {
		for (int t = 20; t < 25; t++) {
			childSet.flip(t);
		}
	}
	//cout << "parent" << endl;
	//showMap(parent);
	//cout << "Child" << endl;
	//showMap(childSet);
	return childSet;
}


void showMap(bitset<26> bits) {
	int p = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << bits[p];
			p++;
		}
		cout << endl;
	}
}
