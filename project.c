// CS177 Project Phase 1
// Trent Callan SID:861117907

#include "cpp.h"
#include <string.h>
#include <math.h>

#define epsilon 1.e-20
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define SIMUNIT 86400 // number of seconds in one day
#define cellLLengthInFeet 11
// each cell is half of a car length space occupied by a car = 2 cells

facility_set road("road",120);
double timeToWait[120];

bool isCellOccupied[120];
// Since each car occupies at least 2 cells thenthe max amount of cars allowed is 120/2 = 60 cars
int carStepsForCar[60];
int speedForCar[60];
int numOfCars = 1;
void car(int index);  // set up car processes

int nextCell(int curCell);

void finalReport();
void snapshot();

extern "C" void sim(){
	create("sim");
	for(int i = 0; i < numOfCars; i++){
		car(i);
	}
	hold(SIMUNIT);
}

void finalReport(){
	

}

void snapshot(){
	printf("car cells = ");
	for(int i = 0; i < 120; i++){
		if(isCellOccupied[i]){
			printf("Cell %d Occupied at time %.1f", i,clock);
		}
	}
}

int nextCell(int curCell){
	return (curCell + 1) % 120;
}

void car1(int index){
	int currCarIndex = index;
	bool carOnRoad = false;
	create("car");
	while(1){
		if(!carOnRoad){
			//place car on road at cell 0,1 if there is space
			
		}

	}
}
