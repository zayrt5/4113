#ifndef MULTI_LOOKUP_H
#define MULTI_LOOKUP_H

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

#include "util.h"


#define ARRAY_SIZE 20
#define MAX_INPUT_FILES 10
#define MAX_RESOLVER_THREADS 10
#define MAX_REQUESTER_THREADS 5
#define MAX_NAME_LENGTH 1025
#define MAX_IP_LENGTH INET6_ADDRSTRLEN

char* fgets_unlocked(char *s, int n, FILE *stream);
int fputs_unlocked(const char *s, FILE *stream);

char** getFiles(int numFileReq, int* numValid, char* files[]);

void* theRequest(void *reqFiles);

void* theRequestHelp(int* filesDone, void *reqFiles);

void* theResolve(void *shArgs);



typedef struct inFile{

    FILE *fd;
    char* name;
    int status;
    pthread_mutex_t lock;

} inFile;


typedef struct outFile{

    FILE *fd;
    char* name;
    int status;
    pthread_mutex_t lock;


} outFile;


typedef struct fileList{

    inFile files[MAX_INPUT_FILES];
    int total;
    int index;
    int numDone;

    pthread_mutex_t lock;

} fileList;


typedef struct shBuffer{

    int total;
    int full;
    int empty;
    int index;
    int reqDone;

    char* shArray[ARRAY_SIZE];



    pthread_mutex_t lock;
    pthread_cond_t cFull;
    pthread_cond_t cEmpty;
    

} shBuffer;


struct reqArgs{
    
    shBuffer *buffetArg;
    outFile servicedArg;
    fileList *files;
    

} productionArgs;

struct resArgs{
    
    shBuffer *buffetArg;
    outFile servicedArg;
    fileList *files;
    

} requestArgs;





#endif
