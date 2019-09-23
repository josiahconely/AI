#include <iostream>
#include <string>
#include <queue>
#include <istream>
#include <fstream>
#include <map>
#include <set>

using namespace std;
						// 0      1        2      3       4      5       6       7
string ClassesNames[] = { "CS101","CS201","CS191","CS291","CS303","CS341","CS449","CS461"};
						//	0			1		2		3			4
string instrctorsArr[] = { "Hare", "Bingham", "Kuhail", "Mitchell", "Rao"};
					//
string roomArr[] = { "Haag 301", "Haag 206", "Royall 204", "Katz 209", "Flarsheim 310", "Flarsheim 260", "Bloch 0009" };
// military time
int time_slots[] = { 10,11,12,13,14,15,16};
// records relationship between instrctors and classes taught
map <string, set<int>> instructorsClasses;
// records relationship between Room and Capacity
map <string, int > RoomCap;
//classes to schedule
string ClassesScheduled[]= {"CS101","CS101","CS201","CS201","CS191","CS191","CS291","CS291","CS303","CS341","CS449","CS461"};
// records relationship between class index and demand
map <int,int> ClassDemand;

class Class
{
	int classNameID;
	int numStudents;
	int instructorID;
	int time_slotID;
	int roomAssignmentID;

public:
	void set_values(int, int);
	int area(void);
};

class SchoolSchedule {
	vector<Class> classes;
};

class SechedulePool {
	vector<SchoolSchedule> population;
};




void main() {

	// defines classes that teaches teach
	instructorsClasses["Hare"] = {0,1,3,4,6,7};
	instructorsClasses["Bingham"] = {0,1,2,3,6};
	instructorsClasses["Kuhail"] = {4,5};
	instructorsClasses["Mitchell"] = {2,3,4,5};
	instructorsClasses["Rao"] = {3,4,5,7};
	
	RoomCap["Haag 301"] = (70);
	RoomCap["Haag 206"] = (30);
	RoomCap["Royall 204"] = (70);
	RoomCap["Katz 209"] = (50);
	RoomCap["Flarsheim 310"] = (80);
	RoomCap["Flarsheim 260"] = (25);
	RoomCap["Bloch 0009"] = (30);
	
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
	
};
