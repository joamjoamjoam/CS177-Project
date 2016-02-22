// CS177 Project Phase 1
// Trent Callan SID:861117907

#include "cpp.h"
#include <string.h>
#include <math.h>

#define epsilon 1.e-20
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) > (y)) ? (y) : (x))
#define SIMUNIT 86400 // number of seconds in one day
#define cellLLengthInFeet 11
// each cell is half of a car length space occupied by a car = 2 cells

facility_set road("road",120);


// when a car occupies a cell it will put its speed into all cells it occupies
// transposing all speed values up one so that 1 = stopped and 6 = full speed.
int isCellOccupied[120];

// Since each car occupies at least 2 cells thenthe max amount of cars allowed is 120/2 = 60 cars
int carStepsForCar[60];
int speedForCar[60];
int numOfCars = 1;
void car(int index);  // set up car processes

int nextCell(int curCell);

void initArrays();
void finalReport();
void snapshot();

extern "C" void sim(){
    create("sim");
    initArrays();
    printf("enter for\n");
    for(int i = 0; i < numOfCars; i++){
        car(i);
        printf("car %d created\n", i);
    }
    printf("enter hold\n");
    hold(SIMUNIT);
}

void finalReport(){
    
}

void initArrays(){
    for (int i = 0; i <= 120; i++) {
        isCellOccupied[i] = 0;
        
        if (i < 60) {
            carStepsForCar[i] = 0;
            speedForCar[i] = 0;
        }
    }
}

void snapshot(){
    
    printf("---------------------------Snapshot-------------------------------\n");
    
    for(int i = 0; i < 20; i++){
        printf("%d ",i);
        /*if(isCellOccupied[i]){
            printf("Cell %d Occupied at time %.1f\n", i,clock);
        }*/
    }
    printf("\n");
    for(int i = 0; i < 20; i++){
        if (i < 10) {
                printf("%d ",isCellOccupied[i]);
        }
        else{
            printf("%d  ", isCellOccupied[i]);
        }
        
        /*if(isCellOccupied[i]){
         printf("Cell %d Occupied at time %.1f\n", i,clock);
         }*/
    }
    printf("\n");
}

int nextCell(int curCell){
    return (curCell + 1) % 120;
}

void car(int index){
    bool carOnRoad = false;
    bool shouldAccelerate = false;
    int waitForAccelerate = 0;
    int decelerateTo = 0;
    int speedOfCarInFront = 6;
    bool reactionTimeElapsed = false;
    int tail,head,movingSpace;
    int closestObstructionSpeed = 6;
    create("car");
    while(1){
        if(!carOnRoad){
            //place car on road at cell 0,1 with speed 0 if there is space
            if (!isCellOccupied[119] && !isCellOccupied[118] && !isCellOccupied[117] && !isCellOccupied[116]  && !isCellOccupied[115]) {
                // previous 5 cells are free car is free to enter roadway
                carOnRoad = true;
                road[0].reserve();
                road[1].reserve();
                tail = 0;
                head = 1;
                isCellOccupied[0] = 1;
                isCellOccupied[1] = 1;
                speedForCar[index] = 1;
                
                printf("Car %d placed on road.\n", index);
            }
            else{
                printf("Car %d not placed on road\n", index);
            }
        }
        else{
            // car is on road
            
            // look ahead to see what if I should accelerate or decelerate or stay the same.
            
            if (head < 11/* && lookahead is clear*/) {
                shouldAccelerate = true;
                reactionTimeElapsed = false;
                // avoid collisions here
            }
            else{
                printf("decelerate detected");
                shouldAccelerate = false;
                //obstruction found
                //closestObstructionSpeed =
            }
            
            // for now we just accelerate
            
            if (shouldAccelerate) {
                switch (speedForCar[index]) {
                    case 1:
                        // start moving
                        
                        speedForCar[index] = 2;
                        isCellOccupied[tail] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        road[nextCell(head)].reserve();
                        movingSpace = nextCell(head);
                        isCellOccupied[movingSpace] = speedForCar[index];
                        waitForAccelerate++;
                        hold(1.5);
                        break;
                    case 2:
                        if (waitForAccelerate == 1) {
                            waitForAccelerate++;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            
                            // shift car into spaces
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold(1.5);
                        }
                        else if(waitForAccelerate == 2){
                            waitForAccelerate = 0;
                            speedForCar[index] = 3;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            // shift car
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold((11/12));
                            
                            
                            
                            waitForAccelerate++;
                        }
                        break;
                    case 3:
                        if (waitForAccelerate == 1) {
                            waitForAccelerate++;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            
                            // shift car into spaces
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold((11/12));
                        }
                        else if(waitForAccelerate == 2){
                            waitForAccelerate = 0;
                            speedForCar[index] = 4;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            // shift car
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold(.5);
                            
                            
                            
                            waitForAccelerate++;
                        }
                        
                        break;
                    case 4:
                        if (waitForAccelerate == 1) {
                            waitForAccelerate++;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            
                            // shift car into spaces
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold(.5);
                        }
                        else if(waitForAccelerate == 2){
                            waitForAccelerate = 0;
                            speedForCar[index] = 5;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            // shift car
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold(.333);
                            
                            
                            
                            waitForAccelerate++;
                        }
                        
                        break;
                    case 5:
                        if (waitForAccelerate == 1) {
                            waitForAccelerate++;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            
                            // shift car into spaces
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold(.333);
                        }
                        else if(waitForAccelerate == 2){
                            waitForAccelerate = 0;
                            speedForCar[index] = 6;
                            
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            
                            road[nextCell(movingSpace)].reserve();
                            
                            // shift car
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold(.25);
                            
                            
                            
                            waitForAccelerate++;
                        }
                        
                        break;
                    case 6:
                        waitForAccelerate = 0;
                        
                        road[tail].release();
                        isCellOccupied[tail] = 0;
                        
                        road[nextCell(movingSpace)].reserve();
                        
                        
                        // shift car into spaces
                        tail = head;
                        head = movingSpace;
                        movingSpace = nextCell(movingSpace);
                        
                        isCellOccupied[movingSpace] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        isCellOccupied[tail] = speedForCar[index];
                        
                        
                        hold(.25);
                        break;
                }
            }
            
            
            if (!shouldAccelerate) {
                // lookout is not clear decelerate
                if (!reactionTimeElapsed) {
                    hold(1);
                    reactionTimeElapsed =true;
                }
                if (speedForCar[index] == 2) {
                    decelerateTo = 1;
                }
                else{
                    decelerateTo = speedForCar[index] - 2;
                }
                speedOfCarInFront = closestObstructionSpeed;
                
                int chosenNewSpeed = max(decelerateTo,speedOfCarInFront);
                
                // if stopping completely set waitForAccelerate to 0 else set it to 1
                
                
                switch (speedForCar[index]) {
                    case 1:
                        // stopped
                        speedForCar[index] = 1;
                        isCellOccupied[tail] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        waitForAccelerate = 0;
                        break;
                    case 2:
                        waitForAccelerate = 1;
                        
                        road[tail].release();
                        isCellOccupied[tail] = 0;
                        
                        road[nextCell(movingSpace)].reserve();
                        
                        
                        // shift car into spaces
                        tail = head;
                        head = movingSpace;
                        movingSpace = -1;
                        
                        speedForCar[index] -= 1;
                        
                        isCellOccupied[head] = speedForCar[index];
                        isCellOccupied[tail] = speedForCar[index];
                        
                        
                        hold(1.5);
                        break;
                    case 3:
                        waitForAccelerate = 1;
                        
                        road[tail].release();
                        isCellOccupied[tail] = 0;
                        
                        road[nextCell(movingSpace)].reserve();
                        
                        
                        // shift car into spaces
                        tail = head;
                        head = movingSpace;
                        movingSpace = nextCell(movingSpace);
                        
                        speedForCar[index] -= 1;
                        
                        isCellOccupied[movingSpace] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        isCellOccupied[tail] = speedForCar[index];
                        
                        
                        hold((11/12));
                        
                        break;
                    case 4:
                        waitForAccelerate = 1;
                        
                        road[tail].release();
                        isCellOccupied[tail] = 0;
                        
                        road[nextCell(movingSpace)].reserve();
                        
                        
                        // shift car into spaces
                        tail = head;
                        head = movingSpace;
                        movingSpace = nextCell(movingSpace);
                        
                        speedForCar[index] -= 1;
                        
                        isCellOccupied[movingSpace] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        isCellOccupied[tail] = speedForCar[index];
                        
                        
                        hold(.5);
                        break;
                    case 5:
                        waitForAccelerate = 1;
                        
                        road[tail].release();
                        isCellOccupied[tail] = 0;
                        
                        road[nextCell(movingSpace)].reserve();
                        
                        
                        // shift car into spaces
                        tail = head;
                        head = movingSpace;
                        movingSpace = nextCell(movingSpace);
                        
                        speedForCar[index] -= 1;
                        
                        isCellOccupied[movingSpace] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        isCellOccupied[tail] = speedForCar[index];
                        
                        
                        hold(.333);
                        
                        break;
                    case 6:
                        waitForAccelerate = 1;
                        
                        road[tail].release();
                        isCellOccupied[tail] = 0;
                        
                        road[nextCell(movingSpace)].reserve();
                        
                        
                        // shift car into spaces
                        tail = head;
                        head = movingSpace;
                        movingSpace = nextCell(movingSpace);
                        
                        speedForCar[index] -= 1;
                        
                        isCellOccupied[movingSpace] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        isCellOccupied[tail] = speedForCar[index];
                        
                        
                        hold(.25);
                        break;
                }
            }
        }
        snapshot();
    }
}














