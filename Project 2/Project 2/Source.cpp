#include <iostream>
#include <string>
#include <queue>
#include <istream>
#include <fstream>
#include <map>
#include <set>
#include <ctime>
#include <algorithm>
using namespace std;

//DATA CONTAINERS////////////////////////////////////////////////////
#define NUMBER_INSTRUCTORS 5
#define NUMBER_OF_TIMESLOTS 7
#define SCHEDULE_POOL_SIZE 700
#define NUMBER_OF_CLASSES 12
#define NUMBER_OF_CLASS_TYPES 8
#define NUMBER_OF_ROOMS 7
#define MUTATION_PROB .05

string ClassesNames[] = { "CS101","CS201","CS191","CS291","CS303","CS341","CS449","CS461" };
string instrctorsArr[] = { "Hare", "Bingham", "Kuhail", "Mitchell", "Rao" };
string roomArr[] = { "Haag 301", "Haag 206", "Royall 204", "Katz 209", "Flarsheim 310", "Flarsheim 260", "Bloch 0009" };
string BuildingArr[] = { "Haag", "Royall", "Katz", "Flarsheim", "Bloch" };
// military time
int time_slots[] = { 10,11,12,13,14,15,16 }; // 7 total
//classes to schedule			
string ClassesScheduled[] = { "CS101","CS101","CS201","CS201","CS191","CS191","CS291","CS291","CS303","CS341","CS449","CS461" };
// records relationship between class index and demand
map <int, int> ClassDemand;
// records relationship between instrctors and classes taught
map <string, set<int>> instructorsClasses;
// records relationship between Room and Capacity
map <string, int > RoomCap;

// CLASSES//////////////////////////////////////////////////////////
class Class {
public:
	int classNameID;
	int numStudents;
	int instructorID;
	int time_slotID;
	int roomAssignmentID;
	int BuildingID;
};
// schedule container
class SchoolSchedule {
public:
	vector<Class> classes;
};
//Generation Container
class SecheduleGeneration {
public:
	vector<SchoolSchedule> population;
};
struct GenEvaluators {
	double avgFittness;
	double bestFittness;
	int idex_of_best;
};

//FUNCTIONS//////////////////////////////////////////////////////////
//Generates initial pool of schedules
Class GenerateRandClass(int);
SchoolSchedule  GenerateRandSchedule();
SecheduleGeneration   GenerateRandScheduleGeneration();
//Calculates Score of each Schedule
double ScoreCalc(SchoolSchedule, bool);               //per schedule
double Adjacent(int, int, int, int, double);
GenEvaluators fittnessEvaluator(SecheduleGeneration); //per generation
//mutate functions
vector<double> L2Normalization(vector<double>);
vector<double> GenerateNomalizationFromPool(SecheduleGeneration);
SecheduleGeneration makeNewGeneration(SecheduleGeneration);
//helper functions 
void printClass(Class);
int findIndexStrArr(string[], string);
int getBuildingID(int);


void main() {
	srand(time(0));

	//initializeData
	{
		// defines classes that teaches teach
		instructorsClasses["Hare"] = { 0,1,3,4,6,7 };
		instructorsClasses["Bingham"] = { 0,1,2,3,6 };
		instructorsClasses["Kuhail"] = { 4,5 };
		instructorsClasses["Mitchell"] = { 2,3,4,5 };
		instructorsClasses["Rao"] = { 3,4,5,7 };

		//Defines room Capacity
		RoomCap["Haag 301"] = (70);
		RoomCap["Haag 206"] = (30);
		RoomCap["Royall 204"] = (70);
		RoomCap["Katz 209"] = (50);
		RoomCap["Flarsheim 310"] = (80);
		RoomCap["Flarsheim 260"] = (25);
		RoomCap["Bloch 0009"] = (30);

		//defines class demand 
		ClassDemand[0] = 40;
		ClassDemand[1] = 25;
		ClassDemand[2] = 30;
		ClassDemand[3] = 30;
		ClassDemand[4] = 60;
		ClassDemand[5] = 20;
		ClassDemand[6] = 40;
		ClassDemand[7] = 20;
		ClassDemand[8] = 50;
		ClassDemand[9] = 40;
		ClassDemand[10] = 55;
		ClassDemand[11] = 40;
	}

	//First Gen creation
	SecheduleGeneration  gen1 = GenerateRandScheduleGeneration();
	GenEvaluators eval = fittnessEvaluator(gen1);
	cout << "Gen 1 Avg: " << eval.avgFittness << endl;
	cout << "Gen 1 Bst: " << eval.bestFittness << endl;
	GenEvaluators evalChild;
	evalChild.avgFittness = 0;
	evalChild.bestFittness = .001;
	int indx = 1;
	int less_than2PercentIcreace = 0;
	do {
		eval = evalChild;
		SecheduleGeneration gen2 = makeNewGeneration(gen1);
		indx++;
		evalChild = fittnessEvaluator(gen2);
		cout << "Gen " << indx << " Avg: " << evalChild.avgFittness << endl;
		cout << "Gen " << indx << " Bst: " << evalChild.bestFittness << endl;
		eval = fittnessEvaluator(gen1);
		gen1 = gen2;
		if (evalChild.bestFittness / eval.bestFittness < 1.02) {
			less_than2PercentIcreace++;
		}
		else {
			less_than2PercentIcreace = 0;
		}
	} while (less_than2PercentIcreace <= 35);

	//print the best of last genereation
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		printClass(gen1.population[evalChild.idex_of_best].classes[i]);
	}

	//prints violations
	double score = ScoreCalc(gen1.population[evalChild.idex_of_best], true);
	cout << "Final Score: " << score << endl;
}

Class GenerateRandClass(int ClassesScheduled_ID) {

	Class randClass;
	randClass.classNameID = findIndexStrArr(ClassesNames, ClassesScheduled[ClassesScheduled_ID]);
	randClass.numStudents = ClassDemand[ClassesScheduled_ID];
	randClass.instructorID = 0 + rand() % (NUMBER_INSTRUCTORS);
	randClass.roomAssignmentID = 0 + rand() % (NUMBER_OF_ROOMS);
	randClass.time_slotID = 0 + rand() % (NUMBER_OF_TIMESLOTS);
	randClass.BuildingID = getBuildingID(randClass.roomAssignmentID);

	return randClass;
}

SchoolSchedule  GenerateRandSchedule() {
	SchoolSchedule Schedule;
	for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
		Schedule.classes.push_back(GenerateRandClass(i));
	}
	return Schedule;
}

SecheduleGeneration	GenerateRandScheduleGeneration() {
	SecheduleGeneration pool;
	for (int i = 0; i < SCHEDULE_POOL_SIZE; i++) {
		pool.population.push_back(GenerateRandSchedule());
	}
	return pool;
}

double ScoreCalc(SchoolSchedule ClassSchedule, bool printViolations) {

	double score = 0;

	//Tracks instructor class count
	int InstructorClassCount[NUMBER_INSTRUCTORS];
	for (int i = 0; i < NUMBER_INSTRUCTORS; i++) {
		InstructorClassCount[i] = 0;
	}

	if (printViolations) {
		cout << endl << "VOLATIONS:" << endl;
	}
	//Per class calculations
	for (int i = 0; i < ClassSchedule.classes.size(); i++) {
		//For each course that is taught by an instructor who can teach it : +3 
		if (instructorsClasses[instrctorsArr[ClassSchedule.classes[i].instructorID]].find(ClassSchedule.classes[i].classNameID)
			!= instructorsClasses[instrctorsArr[ClassSchedule.classes[i].instructorID]].end()) {
			score += 3;
		}
		else {
			if (printViolations) {
				cout << "CLASS " << i << ": >> " << ClassesNames[ClassSchedule.classes[i].classNameID]
					<< " course not taught by an instructor who can teach it INSTRUCTOR " << instrctorsArr[ClassSchedule.classes[i].instructorID] << endl;
			}
		}
		//For each course that is in a room large enough to accommodate it : +5 
		if (ClassSchedule.classes[i].numStudents <= RoomCap[roomArr[ClassSchedule.classes[i].roomAssignmentID]]) {
			score += 5;
		}
		else {
			if (printViolations) {
				cout << "CLASS " << i << ": >> " << ClassesNames[ClassSchedule.classes[i].classNameID] << " room "
					<< roomArr[ClassSchedule.classes[i].roomAssignmentID] << " not large enough to accommodate class. NUM/CAP: "
					<< ClassSchedule.classes[i].numStudents << "/" << RoomCap[roomArr[ClassSchedule.classes[i].roomAssignmentID]] << endl;
			}
		}
		//Room capacity is no more than twice the expected enrollment : +2 
		if (!(RoomCap[roomArr[ClassSchedule.classes[i].roomAssignmentID]] >= 2 * ClassSchedule.classes[i].numStudents)) {
			score += 2;
		}
		else {
			if (printViolations) {
				cout << "CLASS " << i << ": >> " << ClassesNames[ClassSchedule.classes[i].classNameID] << " Room "
					<< roomArr[ClassSchedule.classes[i].roomAssignmentID] << " capacity is more than twice the expected enrollment NUM/CAP: "
					<< ClassSchedule.classes[i].numStudents << "/" << RoomCap[roomArr[ClassSchedule.classes[i].roomAssignmentID]] << endl;
			}
		}
		//Tracks instructor class count
		InstructorClassCount[ClassSchedule.classes[i].instructorID] += 1;

		//For each course that does not have the same instructor teaching another course at the same time : +5 
		bool Instructor_time_conflict = false;
		for (int j = 0; j < ClassSchedule.classes.size(); j++) {
			if (j != i) {
				if (ClassSchedule.classes[i].time_slotID == ClassSchedule.classes[j].time_slotID) {
					if (ClassSchedule.classes[i].instructorID == ClassSchedule.classes[j].instructorID) {
						Instructor_time_conflict = true;
					}
				}
			}
		}
		if (!Instructor_time_conflict) {
			score += 5;
		}
		else {
			if (printViolations) {
				cout << "CLASS " << i << ": >> " << ClassesNames[ClassSchedule.classes[i].classNameID] << " Instructor teaching another course at the same time TIME/INSTRUCTOR: "
					<< time_slots[ClassSchedule.classes[i].time_slotID] << " / " << instrctorsArr[ClassSchedule.classes[i].instructorID] << endl;
			}
		}

		//For each course that is the only course scheduled in that room at that time : +5 
		bool room_time_conflict = false;
		for (int j = i + 1; j < ClassSchedule.classes.size(); j++) {
			if (j != i) {
				if (ClassSchedule.classes[i].time_slotID == ClassSchedule.classes[j].time_slotID) {
					if (ClassSchedule.classes[i].roomAssignmentID == ClassSchedule.classes[j].roomAssignmentID) {
						room_time_conflict = true;
					}
				}
			}
		}
		if (!room_time_conflict) {
			score += 5;
		}
		else {
			if (printViolations) {
				cout << "CLASS " << i << ": >> " << ClassesNames[ClassSchedule.classes[i].classNameID] << " Not only course scheduled in that room at that time TIME/ROOM: "
					<< time_slots[ClassSchedule.classes[i].time_slotID] << " / " << roomArr[ClassSchedule.classes[i].roomAssignmentID] << endl;
			}
		}
	}
	//Per Schedule Calulations
	//each schedule that has the same instructor teaching more than 4 courses : -5 per course over 4  
	for (int i = 0; i < NUMBER_INSTRUCTORS; i++) {
		if (InstructorClassCount[i] > 4) {
			score -= 5;
			if (printViolations) {
				cout << "INSTRUCTOR " << instrctorsArr[i] << " teaching more than 4 courses: "
					<< InstructorClassCount[i] << " CLASSES" << endl;
			}
		}
	}

	//For each schedule that has Rao or Mitchell teaching more courses than Hare or Bingham : -5 % to total fitness score. (Same number of courses is OK.) 
	double AA = 0;
	if ((InstructorClassCount[3] > InstructorClassCount[0]) || (InstructorClassCount[3] > InstructorClassCount[1]) ||
		(InstructorClassCount[4] > InstructorClassCount[0]) || (InstructorClassCount[4] > InstructorClassCount[1])) {
		AA = (.95 * score) - score;
		if (printViolations) {
			cout << "Rao or Mitchell teaching more courses than Hare or Bingham " << endl;
		}
	}


	//CS 101 and CS 191 || CS 201 and CS 291.  apply these rules to those pairs of courses :
	//								0		1		2		3		4		5		6		7		8		9		10		11
	//string ClassesScheduled[]= {"CS101","CS101","CS201","CS201","CS191","CS191","CS291","CS291","CS303","CS341","CS449","CS461"};

	//Courses are scheduled for same time : -10 % to score CS101 CS191
	double BB = 0;
	if ((ClassSchedule.classes[0].time_slotID == ClassSchedule.classes[4].time_slotID) ||
		(ClassSchedule.classes[0].time_slotID == ClassSchedule.classes[5].time_slotID) ||
		(ClassSchedule.classes[1].time_slotID == ClassSchedule.classes[4].time_slotID) ||
		(ClassSchedule.classes[1].time_slotID == ClassSchedule.classes[5].time_slotID)) {
		BB = (.90 * score) - score;
	}

	//Courses are scheduled for same time : -10 % to score CS201 CS291
	double CC = 0;
	if ((ClassSchedule.classes[2].time_slotID == ClassSchedule.classes[6].time_slotID) ||
		(ClassSchedule.classes[2].time_slotID == ClassSchedule.classes[7].time_slotID) ||
		(ClassSchedule.classes[3].time_slotID == ClassSchedule.classes[6].time_slotID) ||
		(ClassSchedule.classes[3].time_slotID == ClassSchedule.classes[7].time_slotID)) {
		CC = (.90 * score) - score;
	}

	/*CS 101 and CS 191 are usually taken the same semester; the same applies to CS 201 and CS 291. Therefore apply these rules to those pairs of courses :
		Courses are scheduled for adjacent times : +5 % to score Done
		if these courses are scheduled for adjacent times, and
			Are in the same building : +5 points
			Are both on the quad(Haag, Royall, Flarsheim) : no modification
			1 is in Katz and the other isn�t : -3 %
			1 is in Bloch and the other isn�t : -3 %
			(Yes, if one�s in Katz and the other�s in Bloch, that�s - 6 %) */

	double a = Adjacent(ClassSchedule.classes[0].time_slotID, ClassSchedule.classes[4].time_slotID,
		ClassSchedule.classes[0].BuildingID, ClassSchedule.classes[4].BuildingID, score);

	double b = Adjacent(ClassSchedule.classes[0].time_slotID, ClassSchedule.classes[5].time_slotID,
		ClassSchedule.classes[0].BuildingID, ClassSchedule.classes[5].BuildingID, score);

	double c = Adjacent(ClassSchedule.classes[1].time_slotID, ClassSchedule.classes[4].time_slotID,
		ClassSchedule.classes[1].BuildingID, ClassSchedule.classes[4].BuildingID, score);

	double d = Adjacent(ClassSchedule.classes[1].time_slotID, ClassSchedule.classes[5].time_slotID,
		ClassSchedule.classes[1].BuildingID, ClassSchedule.classes[5].BuildingID, score);

	double e = Adjacent(ClassSchedule.classes[2].time_slotID, ClassSchedule.classes[6].time_slotID,
		ClassSchedule.classes[2].BuildingID, ClassSchedule.classes[6].BuildingID, score);

	double f = Adjacent(ClassSchedule.classes[2].time_slotID, ClassSchedule.classes[7].time_slotID,
		ClassSchedule.classes[2].BuildingID, ClassSchedule.classes[7].BuildingID, score);

	double g = Adjacent(ClassSchedule.classes[3].time_slotID, ClassSchedule.classes[6].time_slotID,
		ClassSchedule.classes[3].BuildingID, ClassSchedule.classes[6].BuildingID, score);

	double h = Adjacent(ClassSchedule.classes[3].time_slotID, ClassSchedule.classes[7].time_slotID,
		ClassSchedule.classes[3].BuildingID, ClassSchedule.classes[7].BuildingID, score);

	// score plus delta change on percentage calulations to avoid compound percentage distortion
	score = score + a + b + c + d + e + f + g + h + AA + BB + CC;

	return score;
}

double Adjacent(int time_1, int time_2, int Building_1, int Building_2, double score) {
	double scoreINIT = score;
	//Courses are scheduled for adjacent times : +5 %   
	if ((time_1 == time_2 + 1) || (time_1 == time_2 - 1)) {
		score = 1.05 * score;
		//if these courses are scheduled for adjacent times, and Are in the same building : +5 points
		if (Building_1 == Building_2) {
			score += 5;
		}
		else {
			//1 is in Katz and the other isn�t : -3 %
			if (Building_1 == 2 && Building_2 != 2) {
				score = .97 * score;
			}
			//	1 is in Bloch and the other isn�t : -3 %
			if (Building_1 == 4 && Building_2 != 4) {
				score = .97 * score;
			}
		}
	}
	//returns change
	return score - scoreINIT;
}

GenEvaluators fittnessEvaluator(SecheduleGeneration gen) {
	GenEvaluators fittness;
	fittness.avgFittness = 0;
	fittness.bestFittness = 0;
	double sumOfScores = 0;
	double Score;

	for (int i = 0; i < SCHEDULE_POOL_SIZE; i++) {
		Score = ScoreCalc(gen.population[i], false);
		if (Score > fittness.bestFittness) {
			fittness.bestFittness = Score;
			fittness.idex_of_best = i;
		}
		sumOfScores += Score;
	}

	fittness.avgFittness = sumOfScores / SCHEDULE_POOL_SIZE;
	return fittness;
}

vector<double> GenerateNomalizationFromPool(SecheduleGeneration Gen) {
	//collects scores for each schedule
	vector<double> RawScores;
	for (int i = 0; i < Gen.population.size(); i++) {
		RawScores.push_back(ScoreCalc(Gen.population[i], false));
	}

	// passes the scores to the normalizer 
	return L2Normalization(RawScores);
}

vector<double> L2Normalization(vector<double> scores) {
	double sumOfSquare = 0;
	vector<double> normalized;
	//Find the square of each 
	for (int i = 0; i < scores.size(); i++) {
		if (scores[i] > 0) {
			normalized.push_back(scores[i] * scores[i]);
			sumOfSquare += scores[i] * scores[i];
		}
		else {
			normalized.push_back(0);
		}
	}

	//Now divide by the sum of squares
	for (int i = 0; i < normalized.size(); i++) {
		normalized[i] = (normalized[i] / sumOfSquare);
	}

	//Now find the cumulative distribution
	double cumulativeDistribution = normalized[0];
	for (int i = 1; i < normalized.size(); i++) {
		cumulativeDistribution += normalized[i];
		normalized[i] = cumulativeDistribution;

	}
	return normalized;
}

SecheduleGeneration makeNewGeneration(SecheduleGeneration Gen1) {
	SecheduleGeneration Gen2;

	//get probability distrobution for current generation 
	vector<double> probDistrobution = GenerateNomalizationFromPool(Gen1);
	for (int i = 0; i < SCHEDULE_POOL_SIZE; i++) {

		//select 2 schedules for crossover
		double j = rand() / double(RAND_MAX);
		int idx = 0;
		if (idx < SCHEDULE_POOL_SIZE) {
			while (probDistrobution[idx] < j) {
				if (idx < SCHEDULE_POOL_SIZE - 1) { idx++; }
				else { break; }
			}
		}
		else {
			idx = SCHEDULE_POOL_SIZE - 1;
		}
		SchoolSchedule parent1 = Gen1.population[idx];
		j = rand() / double(RAND_MAX);;
		idx = 0;
		if (idx < SCHEDULE_POOL_SIZE) {
			while (probDistrobution[idx] < j) {
				if (idx < SCHEDULE_POOL_SIZE - 1) { idx++; }
				else { break; }
			}
		}
		else {
			idx = SCHEDULE_POOL_SIZE - 1;
		}
		SchoolSchedule parent2 = Gen1.population[idx];

		//Generate random crossover point for mating of two parents
		int crossOver = rand() % (NUMBER_OF_CLASSES);

		//crossover
		SchoolSchedule newSchedule;
		for (int j = 0; j < crossOver; j++) {
			newSchedule.classes.push_back(parent1.classes[j]);
		}
		for (int j = crossOver; j < NUMBER_OF_CLASSES; j++) {
			newSchedule.classes.push_back(parent2.classes[j]);
		}

		// push back to new generation
		Gen2.population.push_back(newSchedule);
	}

	// Mutate the generation 
	for (int i = 0; i < Gen2.population.size(); i++) {
		for (int j = 0; j < Gen2.population[i].classes.size(); j++) {

			double doMutation = rand() / double(RAND_MAX);
			if (doMutation < MUTATION_PROB) {
				Gen2.population[i].classes[j].roomAssignmentID;
				Gen2.population[i].classes[j].BuildingID = getBuildingID(Gen2.population[i].classes[j].roomAssignmentID);
			}


			doMutation = rand() / double(RAND_MAX);
			if (doMutation < MUTATION_PROB) {
				Gen2.population[i].classes[j].instructorID;
			}

			doMutation = rand() / double(RAND_MAX);
			if (doMutation < MUTATION_PROB) {
				Gen2.population[i].classes[j].time_slotID;
			}
		}
	}
	return Gen2;
}

int getBuildingID(int roomID) {
	int buildingID;
	if (roomID == 0 || roomID == 1) {
		buildingID = 0;
	}
	else if (roomID == 2) {
		buildingID = 1;
	}
	else if (roomID == 3) {
		buildingID = 2;
	}
	else if (roomID == 4 || roomID == 5) {
		buildingID = 3;
	}
	else if (roomID == 6) {
		buildingID = 4;
	}
	return buildingID;
}

void printClass(Class class_) {
	cout << endl;
	cout << "classNam ID  " << class_.classNameID << " : Class     : " << ClassesNames[class_.classNameID] << endl;
	cout << "instructorID " << class_.instructorID << " : Instructor: " << instrctorsArr[class_.instructorID] << endl;
	cout << "Building ID  " << class_.BuildingID << " : Building  : " << BuildingArr[class_.BuildingID] << endl;
	cout << "Room     ID  " << class_.roomAssignmentID << " : Room      : " << roomArr[class_.roomAssignmentID] << endl;
	cout << "time_slotID  " << class_.time_slotID << " : time      : " << time_slots[class_.time_slotID] << endl;
	cout << "numStudents  " << class_.numStudents << endl;
}

int findIndexStrArr(string arr[], string target) {
	for (int i = 0; i < NUMBER_OF_CLASS_TYPES; i++) {
		if (arr[i] == target) {
			return i;
		}
	}
	return -1;
}