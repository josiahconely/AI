#include <iostream>
#include <bitset>
#include <string>
#include <queue>
#include <istream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <time.h> 

using namespace std;

//index number equals current board state
//value in array[i] equals next board state toward solution
bitset<26> solution_array[33554433];
//cue of new board states found. Children of these board states will be explored
queue<bitset<26>> searchCue;
int numsolutionFound;
//flips current bitset at positon i[0-24] and returns new bitset
bitset<26> flip_position(int i, bitset<26> parent);
// explores all board states starting from solution going backwards
void solution_builder();
//Visual representation of boardstate
void showBoard(bitset<26>);
vector<bitset<26>> solveable;
bool playGame();
//prints a list of pos to select to solve the game
void printList(bitset<26>);
//prints a play by play board solving the game
void printCompList(bitset<26>);
//identifies flipped pos between two board states
int findFlipPos(bitset<26>, bitset<26>);

void main() {
	srand(time(0));

	//Goal solutions are initialized with 26th bit flipped on to avoid being pushed to the search cue
	solution_array[0].set(25);
	solution_array[33554431].set(25);

	//The search cue is initialized with the two goal states
	bitset<26> empty;
	searchCue.push(bitset<26>(0));
	searchCue.push(bitset<26>(33554431));

	//Finds all solutions and build solution array
	solution_builder();
	showBoard(bitset<26>(1082431));
	solution_array[1082431] = bitset<26>(0);
	//plays game
	cout << "Game Ready! Want to play?" << endl;

	while (playGame()) {
		cout << "thanks for playing!" << endl;
	}
}

void solution_builder() {
	while (!searchCue.empty()) {

		bitset<26> parent = searchCue.front();
		
		//cout << searchCue.size()<<endl;
		for (int i = 0; i < 25; i++) {
			bitset<26> childSet;
			childSet = flip_position(i, parent);

			if (solution_array[childSet.to_ulong()].none()) {
				if (parent != bitset<26>(0)) {
					solution_array[childSet.to_ulong()] = parent;
				}
				else {
					solution_array[childSet.to_ulong()] = bitset<26>(33554432);
				}
				searchCue.push(childSet);
				solveable.push_back(childSet);
			}
		}
		searchCue.pop();
	}
}

bool playGame() {
	bitset<26> startBoard;
	startBoard = solveable[rand() % (solveable.size())];
	int option;
	cout << "Start board!!" << endl;
	showBoard(startBoard);
	cout << "enter 1 for seeing a list of moves to make,\n Enter 2 to see comprehensive list of states of the board after each flip " << endl;
	cin >> option; 
	
	if (option == 1) {printList(startBoard);}
	else if (option == 2) {printCompList(startBoard);}
	else { cout << "invalid entry"; }
	return true;
}

void printList(bitset<26> start) {
	bitset<26> end;
	end.set(25);
	while (solution_array[(int)start.to_ulong()] != end) {
		cout << findFlipPos(solution_array[(int)start.to_ulong()], start)<<endl;
		start = solution_array[(int)start.to_ulong()];
	}
	
}


void printCompList(bitset<26> start) {
	bitset<26> end;
	end.set(25);
	int stepnum = 1;
	while (solution_array[(int)start.to_ulong()] != end) {
		cout << "Step " << stepnum << " change " << findFlipPos(solution_array[(int)start.to_ulong()], start) << endl;
		stepnum++;
		cout << "the result is..." << endl;
		showBoard(solution_array[(int)start.to_ulong()]);
		start = solution_array[(int)start.to_ulong()];
	}
	cout << endl;
}


int findFlipPos(bitset<26> parent, bitset<26> child) {
	int column;
	int row;
	if (parent[0] != child[0] && parent[1] != child[1]) {
		row = 0;
		for (int i = 5; i < 10; i++) {
			if (parent[i] != child[i]) {
				column = i - 5;
			}
		}
	}

	else {
		for (int i = 0; i < 5; i++) {
			if (parent[i] != child[i]) {
				column = i;

				for (int j = 0; j < 5; j++) {
					if (parent[((i+1) % 5) +(j*5)] != child[((i + 1) % 5) + (j * 5)]) {
						row = j;
					}
				}
			}
		}
	}
	column++;
	return  row*5 + column;
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
