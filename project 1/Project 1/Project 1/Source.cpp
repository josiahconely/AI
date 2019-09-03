#include <iostream>
#include <bitset>
#include <string>
#include <queue>
#include <istream>
#include <fstream>

using namespace std;

//index number equals current board state
//value in array[i] equals next board state toward solution
bitset<26> solution_array[33554432];
//cue of new board states found. Children of these board states will be explored
queue<bitset<26>> searchCue;
int numsolutionFound;
//flips current bitset at positon i[0-24] and returns new bitset
bitset<26> flip_position(int i, bitset<26> parent);
// explores all board states starting from solution going backwards
void solution_builder();
//Visual representation of boardstate
void showBoard(bitset<26>);

void main() {

	//Ensures all bitsets are zero: 
	for (int i = 0; i < 33554432; i++) {
		solution_array[i].reset();
	}

	numsolutionFound = 0;

	//Goal solutions are initialized with 26th bit flipped on to avoid being pushed to the search cue
	solution_array[0].set(25);
	solution_array[33554431].set(25);

	//The search cue is initialized with the two goal states
	searchCue.push(bitset<26>(0));
	searchCue.push(bitset<26>(33554431));
	//Finds all solutions and build solution array
	solution_builder();
	cout << "number of soulutions found " << numsolutionFound << endl;

	/*for (int i = 0; i < 33554432; i++) {
		cout << "parent" << endl;
		showBoard(solution_array[i]);
		cout << "Child" << endl;
		showBoard(bitset<26>(i));
	}*/
}

void solution_builder() {
	while (!searchCue.empty()) {
		//cout << "number inline    >>> " << nodeCue.size() << endl;
		bitset<26> parent = searchCue.front();
		searchCue.pop();

		//cout << "parent ++++++++++++++++++++++++++++++++++++++" << endl;
		//showBoard(parent);
		//cout << "number inline    >>> " << nodeCue.size() << endl;
		//int oldline = searchCue.size();
		//cin >> x;
		//int numDups = 0;
		//int numNew = 0;

		for (int i = 0; i < 25; i++) {
			bitset<26> childSet;
			childSet = flip_position(i, parent);
			//cout << "Child" << endl;
			//showBoard(childSet);

			if (solution_array[(int)childSet.to_ulong()].none()) {
				solution_array[(int)childSet.to_ulong()] = parent;
				numsolutionFound++;
				searchCue.push(childSet);
				//cout << "pushed to cue "<< endl;
				//numNew++;
			}
			else {
				//cout << "dup found=============" << endl;
				//numDups++;
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
	
	//changes positions at columns
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

	//changes positions at rows
	if (i >= 0 && i < 5) {
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


void showBoard(bitset<26> bits) {
	int p = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << bits[p];
			p++;
		}
		cout << endl;
	}
}
