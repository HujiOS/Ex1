//
// Created by Omer on 09/03/2017.
//



#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "osm.h"

#define FILENAME 'WhadIDo'

/* Initialization function that the user must call
 * before running any other library function.
 * The function may, for example, allocate memory or
 * create/open files.
 * Pay attention: this function may be empty for some desings. It's fine.
 * Returns 0 uppon success and -1 on failure
 */
int osm_init(){
    return 1;
}


/* finalizer function that the user must call
 * after running any other library function.
 * The function may, for example, free memory or
 * close/delete files.
 * Returns 0 uppon success and -1 on failure
 */
int osm_finalizer(){
    return 1;
}

void dummyFoo(){
    1+1;
}
void emptyFoo(){}

void trap(){
    OSM_NULLSYSCALL;
}

void fileLoad(){
    FILE *f;
    fopen("WhatIDo","rw");
    fread(NULL,10,10,f);
    fflush(f);
    fputs("abcdef",f);
    fclose(f);
}

unsigned int validateIterations(unsigned int iter){
    iter = iter != 0? iter : 1000;
    iter += iter % 5; // we are running 5 calls for each iteration so we want
    // that iterations % 5 = 0
    return iter;
}

double timeIt(void (*foo)(), unsigned int iter){
    timeval sTime, eTime;
    unsigned long totalTime = 0;
    for(unsigned int i=0;i<iter/5;++i){
        totalTime += eTime.tv_usec - sTime.tv_usec;
        gettimeofday(&sTime,NULL);
        foo();
        foo();
        foo();
        foo();
        foo();
        gettimeofday(&eTime,NULL);
    }
    return (totalTime / (iter / 5))*1000; // convert from micro to nano
}
/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
    iterations = validateIterations(iterations);
    return timeIt(dummyFoo, iterations);

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
    timeMeasurmentStructure *myTime = new timeMeasurmentStructure;
    gethostname(myTime->machineName,100);
    myTime->instructionTimeNanoSecond = osm_operation_time(operation_iterations);
    myTime->functionTimeNanoSecond = osm_function_time(function_iterations);
    myTime->trapTimeNanoSecond = osm_syscall_time(syscall_iterations);
    myTime->diskTimeNanoSecond = osm_disk_time(disk_iterations);
    myTime->functionInstructionRatio = myTime->functionTimeNanoSecond /
            myTime->instructionTimeNanoSecond;
    myTime->trapInstructionRatio = myTime->trapTimeNanoSecond / myTime->instructionTimeNanoSecond;
    myTime->diskInstructionRatio = myTime->diskTimeNanoSecond / myTime->instructionTimeNanoSecond;
    return *myTime;
}
