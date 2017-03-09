//
// Created by Omer on 09/03/2017.
//



#include <sys/time.h>
#include <w32api/_timeval.h>

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
/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
    // in case of iterations = 0
    iterations = iterations != 0? iterations : 1000;
    iterations += iterations % 5; // we are running 5 calls for each iteration so we want
                                    // that iterations % 5 = 0
    timeval sTime, eTime;
    unsigned long totalTime = 0;
    for(unsigned int i=0;i<iterations/5;++i){
        totalTime += eTime.tv_usec - sTime.tv_usec;
        gettimeofday(&sTime,NULL);
        dummyFoo();
        dummyFoo();
        dummyFoo();
        dummyFoo();
        dummyFoo();
        gettimeofday(&eTime,NULL);
    }
    return totalTime / (iterations / 5);
}
