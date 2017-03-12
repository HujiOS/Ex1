//
// Created by Omer on 09/03/2017.
//



#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "osm.h"

using namespace std;
timeMeasurmentStructure *myTime;

int dummyInt;
/* Initialization function that the user must call
 * before running any other library function.
 * The function may, for example, allocate memory or
 * create/open files.
 * Pay attention: this function may be empty for some desings. It's fine.
 * Returns 0 uppon success and -1 on failure
 */
int osm_init(){
    myTime = new timeMeasurmentStructure;
    myTime->machineName = new char[256];
    if(myTime == nullptr || myTime->machineName == nullptr){
        return -1;
    }
    return 0;
}


/* finalizer function that the user must call
 * after running any other library function.
 * The function may, for example, free memory or
 * close/delete files.
 * Returns 0 uppon success and -1 on failure
 */
int osm_finalizer(){
    delete myTime -> machineName;
    delete myTime;
    return 0;
}

void dummyFoo(){
    dummyInt = 1 + 1;
}
void emptyFoo(){}

void trap(){
    OSM_NULLSYSCALL;
}

void fileLoad(){
    FILE *f;
    f = fopen("stamText.txt","r");
    fclose(f);
    f = fopen("stamText.txt","r");
    fclose(f);
    f = fopen("stamText.txt","r");
    fclose(f);
    f = fopen("stamText.txt","r");
    fclose(f);
    f = fopen("stamText.txt","r");
    fclose(f);
}

unsigned int validateIterations(unsigned int iter){
    iter = iter != 0 ? iter : 1000;
    iter += iter % 10; // we are running 5 calls for each iteration so we want
    // that iterations % 5 = 0
    return iter;
}

double timeIt(void (*foo)(), unsigned int iter){
    timeval sTime, eTime, diff;
    if(gettimeofday(&sTime,NULL) == -1){
        return -1;
    };
    for(unsigned int i=0;i<iter/10;++i){
        foo();
        foo();
        foo();
        foo();
        foo();
        foo();
        foo();
        foo();
        foo();
        foo();

    }
    if(gettimeofday(&eTime,NULL) == -1){
        return -1;
    }
    timersub(&eTime,&sTime,&diff);
    return (double) ((diff.tv_sec * pow(10,9)) + (diff.tv_usec * pow(10,3))) / iter; // convert from
    // micro to nano
}

/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
    iterations = validateIterations(iterations);
    timeval sTime, eTime, diff;
    if(gettimeofday(&sTime,NULL) == -1){
        return -1;
    };
    for(unsigned int i=0;i<iterations/10;++i){
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
        dummyInt = 1 + 1;
    }
    if(gettimeofday(&eTime,NULL) == -1){
        return -1;
    }
    timersub(&eTime,&sTime,&diff);
    return (double) ((diff.tv_sec * pow(10,9)) + (diff.tv_usec * pow(10,3))) / iterations; // convert from
    // micro to nano
}


/* Time measurement function for an empty function call.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_function_time(unsigned int iterations){
    iterations = validateIterations(iterations);
    return timeIt(emptyFoo, iterations);
}

/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time(unsigned int iterations){
    iterations = validateIterations(iterations);
    return timeIt(trap, iterations);
}

/* Time measurement function for accessing the disk.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_disk_time(unsigned int iterations){
    iterations = validateIterations(iterations);
    timeval sTime, eTime;
    unsigned long totalTime = 0;
    for(unsigned int i=0;i<iterations/5;++i){
        totalTime += eTime.tv_usec - sTime.tv_usec;
        gettimeofday(&sTime,NULL);
        fileLoad();
        gettimeofday(&eTime,NULL);
    }
    return (totalTime / (iterations / 5))*1000; // convert from micro to nano
}

timeMeasurmentStructure measureTimes (unsigned int operation_iterations,
                                      unsigned int function_iterations,
                                      unsigned int syscall_iterations,
                                      unsigned int disk_iterations){

    gethostname(myTime -> machineName,sizeof(char)*256);

    myTime -> instructionTimeNanoSecond = osm_operation_time(operation_iterations);

    myTime -> functionTimeNanoSecond = osm_function_time(function_iterations);

    myTime -> trapTimeNanoSecond = osm_syscall_time(syscall_iterations);
    cout << "what what in the butt3" << endl;
    myTime -> diskTimeNanoSecond = osm_disk_time(disk_iterations);

    myTime -> functionInstructionRatio = myTime->functionTimeNanoSecond /
            myTime->instructionTimeNanoSecond;

    myTime->trapInstructionRatio = myTime->trapTimeNanoSecond / myTime->instructionTimeNanoSecond;
    myTime->diskInstructionRatio = myTime->diskTimeNanoSecond / myTime->instructionTimeNanoSecond;
    return *myTime;
}
