//
// Created by Omer on 09/03/2017.
//



#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <fcntl.h>
#include "osm.h"
#include <stdio.h>
#include <stdlib.h>

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

void emptyFoo(){}

void trap(){
    OSM_NULLSYSCALL;
}


unsigned int validateIterations(unsigned int iter){
    iter = iter != 0 ? iter : 1000;
    iter += iter % 10; // we are running 5 calls for each iteration so we want
    // that iterations % 5 = 0
    return iter;
}

double timeIt(void (*foo)(), unsigned int iter){
    timeval sTime, eTime, diff;
    double total = 0;
    for(unsigned int i=0;i<iter/10;++i){
        if(gettimeofday(&sTime,NULL) == -1){
            return -1;
        };
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
        if(gettimeofday(&eTime,NULL) == -1){
            return -1;
        }
        timersub(&eTime,&sTime,&diff);
        // im saving the results as nano..
        total += (double) ((diff.tv_sec * pow(10,9)) + (diff.tv_usec * pow(10,3)));
    }
    return total / iter;
}

/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
    iterations = validateIterations(iterations);
    timeval sTime, eTime, diff;
    double ttl = 0;

    for(unsigned int i=0;i<iterations/10;++i){
        if(gettimeofday(&sTime,NULL) == -1){
            return -1;
        };
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
        if(gettimeofday(&eTime,NULL) == -1){
            return -1;
        }
        timersub(&eTime,&sTime,&diff);
        ttl += (double) ((diff.tv_sec * pow(10,9)) + (diff.tv_usec * pow(10,3)));
    }


    return ttl / iterations; // convert from
    // micro to nano
}

size_t get_block_size()
{
    struct stat fi;
    stat("/tmp", &fi);
    return fi.st_blksize;
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
    timeval sTime, eTime, diff;
    int f;
    double ttl = 0;
    int readBytes = 0;
    size_t block_size = get_block_size();
    char * buff = (char*)aligned_alloc(block_size, sizeof(char)*block_size);//block_size,(fst param)
    f = open("WhatIDo", O_SYNC | O_DIRECT | O_CREAT, S_IRWXG | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(f<0){
            cout<<"error while open the file. errno: "<<f<<endl;
            return -1;
            // cant open file \ cant read from file
    }

    for(unsigned int i=0;i<iterations;++i){
        if(gettimeofday(&sTime,NULL) == -1){
            return -1;
        };
        readBytes = pread(f, buff, block_size, 0);
        if(gettimeofday(&eTime,NULL) == -1 || readBytes < 0){
            cout<<"error while reading from file"<<endl;
            return -1;
        }
        timersub(&eTime,&sTime,&diff);
        ttl += (double) ((diff.tv_sec * pow(10,9)) + (diff.tv_usec * pow(10,3)));
    }
    close(f);
    free(buff);
    return ttl / iterations;
}

timeMeasurmentStructure measureTimes (unsigned int operation_iterations,
                                      unsigned int function_iterations,
                                      unsigned int syscall_iterations,
                                      unsigned int disk_iterations){

    gethostname(myTime -> machineName,sizeof(char)*256);

    myTime -> instructionTimeNanoSecond = osm_operation_time(operation_iterations);

    myTime -> functionTimeNanoSecond = osm_function_time(function_iterations);

    myTime -> trapTimeNanoSecond = osm_syscall_time(syscall_iterations);
    myTime -> diskTimeNanoSecond = osm_disk_time(disk_iterations);
    if(myTime->instructionTimeNanoSecond == 0){ // we don`t want to divide by zero.
        myTime -> functionInstructionRatio = myTime->functionTimeNanoSecond / -1;
        myTime->trapInstructionRatio = myTime->trapTimeNanoSecond / -1;
        myTime->diskInstructionRatio = myTime->diskTimeNanoSecond / -1;
        return *myTime;
    }
    myTime -> functionInstructionRatio = myTime->functionTimeNanoSecond /
                                         myTime->instructionTimeNanoSecond;
    myTime->trapInstructionRatio = myTime->trapTimeNanoSecond / myTime->instructionTimeNanoSecond;
    myTime->diskInstructionRatio = myTime->diskTimeNanoSecond / myTime->instructionTimeNanoSecond;

    return *myTime;
}
