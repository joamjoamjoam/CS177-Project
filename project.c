// CS177 Project Phase 1
// Trent Callan SID:861117907

#include "cpp.h"
#include <string.h>
#include <stdlib.h>
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
int targetSpeedForCar[60];
int state;
// max is 60
int numOfCars = 20;
void car(int index);  // set up car processes
void streetLight();

int nextCell(int curCell);

void initArrays();
void initCars();
void finalReport();
void snapshot();
void calcTargetSpeed();
void checkForCollison();
int prevCell(int index);
int randomInt();
bool collisionDetected = false;

extern "C" void sim(){
    create("sim");
    initArrays();
    streetLight();
    calcTargetSpeed();
    //checkForCollison();
    
    for(int i = 0; i < numOfCars; i++){
        car(i);
        printf("car %d created\n", i);
    }
    hold(SIMUNIT);
    if (collisionDetected) {
        printf("Results Invalid Collision Detected.");
    }
    else{
        printf("No Collisons detetected.");
    }
}

int prevCell(int index){
    if ((index - 1) < 0) {
        return 119;
    }
    else{
        return index - 1;
    }
}

void checkForCollison(){
    create("check for collision");
    
    int tmpPrev, tmpNext, tmpCurr, ones,twos,threes,fours,fives,sixes;
    
    while (1) {
        ones = 0;
        twos = 0;
        threes = 0;
        fours = 0;
        fives = 0;
        sixes = 0;
        for (int i = 0; i < 120; i++) {
            
            switch (isCellOccupied[i]) {
                case 0:
                    
                    break;
                case 1:
                    ones++;
                    break;
                case 2:
                    twos++;
                    break;
                case 3:
                    threes++;
                    break;
                case 4:
                    fours++;
                    break;
                case 5:
                    fives++;
                    break;
                case 6:
                    sixes++;
                    break;
            }
        }
        if(!(ones % 2) && !(twos % 3) && !(threes % 3) && !(fours % 3) && !(fives % 3) && !(sixes % 3)){
        }
        else{
            collisionDetected = true;
            return;
        }
        hold(.2);
    }
}

void finalReport(){
    
}
int randomInt(){
    return 6;
}

void calcTargetSpeed(){
    create("calcTargetSpeed");
    //while(1){
        
        for (int i = 0; i < numOfCars; i++) {
            printf("%d", i);
            targetSpeedForCar[i] = 6;
        }
        //hold(uniform(60,120));
    //}
}

void initArrays(){
    for (int i = 0; i <= 120; i++) {
        isCellOccupied[i] = 0;
        
        if (i < 60) {
            carStepsForCar[i] = 0;
            speedForCar[i] = 0;
            targetSpeedForCar[i] = randomInt;
            
        }
    }
}


void snapshot(){
    
    printf("---------------------------Snapshot------------------------------- clock = %.2f state = %d var = %d  %d\n", clock, state,speedForCar[0],targetSpeedForCar[0]);
    
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

void streetLight(){
    create("streetLight");
    bool allowedToSwitch = true;
    //0 = red
    //1 = yellow
    //2 = green
    printf("street light created\n");
    
    while (1) {
        switch (state) {
            case 0:
                while (!allowedToSwitch) {
                    //wait until 118 and 119 is clear
                    if (!isCellOccupied[118] && !isCellOccupied[119]) {
                        allowedToSwitch = true;
                    }
                }
                isCellOccupied[118] = 1;
                isCellOccupied[119] = 1;
                road[118].reserve();
                road[119].reserve();
                hold(uniform(30,90));
                state = 2;
                
                break;
            case 1:
                hold(10);
                state = 0;
                break;
            case 2:
                isCellOccupied[118] = 0;
                isCellOccupied[119] = 0;
                road[118].release();
                road[119].release();
                hold(expntl(120));
                state = 1;
                break;
                
            default:
                break;
        }
    }
}

void car(int index){
    bool carOnRoad = false;
    bool shouldAccelerate = false;
    int waitForAccelerate = 0;
    int decelerateTo = 0;
    int speedOfCarInFront = 6;
    bool reactionTimeElapsed = false;
    int tail,head;
    int movingSpace = -1;
    int closestObstructionSpeed = 6;
    bool obstructionDetected = false;
    int monitoringCarLengths = 0;
    int initCarsHelper = 117;
    create("car");
    while(1){
        if(!carOnRoad){
            carOnRoad = true;
            
            head = initCarsHelper - (2 * index);
            tail = head - 1;
            
            isCellOccupied[head] = 1;
            isCellOccupied[tail] = 1;
            speedForCar[index] = 1;
            road[tail].reserve();
            road[head].reserve();
            
            initCarsHelper -= 2;
            
            
            
            printf("Car %d placed on road. with tail = %d and head = %d\n", index, tail, head);
            
        }
        else{
            // car is on road
            
            // look ahead to see what if I should accelerate or decelerate or stay the same.
            
            // the 4 cell look ahead will determine if we accelerate or decelerate
            
            // lookahead
            obstructionDetected = false;
            switch (speedForCar[index]) {
                case 1:
                    monitoringCarLengths = 1;
                    break;
                case 2:
                    monitoringCarLengths = 1;
                    break;
                case 3:
                    monitoringCarLengths = 2;
                    break;
                case 4:
                    monitoringCarLengths = 2;
                    break;
                case 5:
                    monitoringCarLengths = 3;
                    break;
                case 6:
                    monitoringCarLengths = 4;
                    break;
                    
                default:
                    break;
            }
            for (int i = 0; i < monitoringCarLengths; i++) {
                if (isCellOccupied[nextCell(movingSpace + i)] && speedForCar[index] != 1) {
                    obstructionDetected = true;
                    //obstruction found
                    //printf("decelerate detected 1");
                    shouldAccelerate = false;
                    
                    closestObstructionSpeed = isCellOccupied[nextCell(movingSpace + i)];
                    
                    break;
                    
                }
                else if(speedForCar[index] == 1 && isCellOccupied[nextCell(head)]){
                    obstructionDetected = true;
                    //obstruction found
                    //printf("decelerate detected 2");
                    shouldAccelerate = false;
                    
                    closestObstructionSpeed = isCellOccupied[nextCell(head)];
                    break;
                }
            }
            
            if (!obstructionDetected) {
                shouldAccelerate = true;
                reactionTimeElapsed = false;
            }
            if (shouldAccelerate && (speedForCar[index] < targetSpeedForCar[index])) {
                switch (speedForCar[index]) {
                    case 1:
                        // start moving
                        if (waitForAccelerate == 0) {
                            waitForAccelerate++;
                            speedForCar[index] = 2;
                            isCellOccupied[tail] = speedForCar[index];
                            
                            isCellOccupied[head] = speedForCar[index];
                            road[nextCell(head)].reserve();
                            movingSpace = nextCell(head);
                            isCellOccupied[movingSpace] = speedForCar[index];
                            hold(1.5);
                        }
                        else if (waitForAccelerate == 1) {
                            waitForAccelerate = 1;
                            road[tail].release();
                            isCellOccupied[tail] = 0;
                            road[nextCell(movingSpace)].reserve();
                            
                            
                            // shift car into spaces
                            tail = head;
                            head = movingSpace;
                            movingSpace = nextCell(movingSpace);
                            
                            speedForCar[index] = 2;
                            
                            isCellOccupied[movingSpace] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            isCellOccupied[tail] = speedForCar[index];
                            
                            hold(1.5);
                        }
                        
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
                            
                            hold((11/12));
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
                            
                            hold(.5);
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
                            
                            hold(.333);
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
                            
                            hold(.25);
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
            else if(speedForCar[index] == targetSpeedForCar[index]){
                // target speed reached
                switch (speedForCar[index]) {
                    case 1:
                        // stopped
                        speedForCar[index] = 1;
                        isCellOccupied[tail] = speedForCar[index];
                        isCellOccupied[head] = speedForCar[index];
                        waitForAccelerate = 0;
                        hold(1.5);
                        break;
                    case 2:
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
                        break;
                    case 3:
                        
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
                        break;
                    case 4:
                        
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
                        break;
                    case 5:
                        
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
                        break;
                    case 6:
                        
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
                        
                    default:
                        hold(1);
                        break;
                }
              
            }
            else{
                printf("other");
            }
            
            if(!shouldAccelerate) {
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
                
                chosenNewSpeed = min(targetSpeedForCar[index],chosenNewSpeed);
                
                // if stopping completely set waitForAccelerate to 0 else set it to 1
                
                if (speedForCar[index] > chosenNewSpeed) {
                    switch (speedForCar[index]) {
                        case 1:
                            // stopped
                            speedForCar[index] = 1;
                            isCellOccupied[tail] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            waitForAccelerate = 0;
                            hold(1.5);
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
                else{
                    //printf("target speed reached");
                    
                    switch (speedForCar[index]) {
                        case 1:
                            // stopped
                            speedForCar[index] = 1;
                            isCellOccupied[tail] = speedForCar[index];
                            isCellOccupied[head] = speedForCar[index];
                            waitForAccelerate = 0;
                            hold(1.5);
                            break;
                        case 2:
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
                            break;
                        case 3:
                            
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
                            break;
                        case 4:
                            
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
                            break;
                        case 5:
                            
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
                            break;
                        case 6:
                            
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
                            
                        default:
                            hold(1);
                            break;
                    }
                }
                
                
            }
        }
        if (!index) {
            snapshot();
        }
    }
}














