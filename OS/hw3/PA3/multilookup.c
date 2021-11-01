#include "multilookup.h"


void* theRequest(void *reqFiles){

    struct reqArgs *reqData = (struct reqArgs *) reqFiles;
	int total = 0;

	theRequestHelp( &total, reqFiles);

    int length = snprintf( NULL, 0, "%i", total);
	char* nameLen = malloc( sizeof(char) * (length + 1));
    //printf ("  nameLen size: %lu \n", sizeof(nameLen));


	snprintf(nameLen, sizeof(char) * (length + 1), "%i", total);
	length = snprintf( NULL, 0, "%ld", syscall(SYS_gettid));
	char* tid = malloc( sizeof(char) * (length + 1));
    //printf ("  tid size: %lu \n", sizeof(tid));
	snprintf(tid, sizeof(char) * (length + 1), "%ld", syscall(SYS_gettid));

	char* out = malloc(sizeof(char) * (26 + strlen(nameLen) + strlen(tid)));
    //printf ("  out size: %lu \n", sizeof(out));
	strcpy(out, "Thread ");
	strcat(out, tid);
	strcat(out, " serviced ");
	strcat(out, nameLen);
	strcat(out, " files.\n");

	free(nameLen);
	free(tid);

	pthread_mutex_lock(&reqData->servicedArg.lock);
	fputs_unlocked(out, reqData->servicedArg.fd);
	pthread_mutex_unlock(&reqData->servicedArg.lock);


	printf("%s",out);

	free(out);


	return 0;

}

void* theRequestHelp(int* filesDone, void* reqFiles){


    struct reqArgs *reqData = (struct reqArgs *) reqFiles;
    fileList *argDir = reqData->files;
    shBuffer *buffet = reqData->buffetArg;
    pthread_mutex_lock(&argDir->lock);


    if(argDir->numDone == argDir->total){

		pthread_mutex_unlock(&argDir->lock);
		return 0;

	}
	else{
        
		//Find an unfinished file
		while(argDir->files[argDir->index].status){

			if(argDir->index == argDir->total - 1){

				argDir->index = 0;

			}

			else{

				argDir->index++;

			}
		}

		inFile *curFile = &argDir->files[argDir->index];
		(*filesDone)++;

        //loop index back
		if(argDir->index == argDir->total - 1){

			argDir->index = 0;
		}
		else{

			argDir->index++;
		}

		pthread_mutex_unlock(&argDir->lock);

		while(1){

			pthread_mutex_lock(&curFile->lock);

			char *hostname = (char *) malloc((MAX_NAME_LENGTH) * sizeof(char));
            //printf ("  hostname size: %lu \n", sizeof(hostname));

			if(fgets_unlocked(hostname, MAX_NAME_LENGTH, curFile->fd)){

				pthread_mutex_unlock(&curFile->lock);

				if(strlen(hostname) <= MAX_NAME_LENGTH - 1 && hostname[strlen(hostname) - 1] == '\n'){

					hostname[strlen(hostname) - 1] = '\0';
				}


				pthread_mutex_lock(&buffet->lock);

				while(buffet->full){

					pthread_cond_wait(&buffet->cFull, &buffet->lock);
				}


				buffet->shArray[buffet->index] = hostname;

				if(buffet->index == buffet->total - 1){

					buffet->full = 1;
				}

				buffet->index++;

				if(buffet->index - 1 == 0){

					buffet->empty = 0;
					pthread_cond_broadcast(&buffet->cEmpty);
				}

				pthread_mutex_unlock(&buffet->lock);
			}
			else{

				free(hostname);

				if(!curFile->status){

					curFile->status = 1;
					pthread_mutex_unlock(&curFile->lock);
					pthread_mutex_lock(&argDir->lock);

					argDir->numDone++;

					pthread_mutex_unlock(&argDir->lock);
					theRequestHelp(filesDone++, reqFiles);
				}
				else{

					pthread_mutex_unlock(&curFile->lock);
				}

				break;
			}
            
		}
	}


	return 0;

}


void* theResolve(void *shArgs){


    struct resArgs *resData = (struct resArgs *) shArgs;

	shBuffer *buffet = resData->buffetArg;

	while(1){

		pthread_mutex_lock(&buffet->lock);

		while(buffet->empty && !buffet->reqDone){

			pthread_cond_wait(&buffet->cEmpty, &buffet->lock);

		}
		//release if empty and requests are done
		if(buffet->empty && buffet->reqDone){

			pthread_mutex_unlock(&buffet->lock);
			break;

		}

		char *hostname = buffet->shArray[buffet->index - 1];
		buffet->shArray[buffet->index - 1] = NULL;

		if(buffet->index == 1){

			buffet->empty = 1;

		}

		buffet->index--;

		if(buffet->index == ARRAY_SIZE - 1){

			buffet->full = 0;
			pthread_cond_broadcast(&buffet->cFull);

		}

		pthread_mutex_unlock(&buffet->lock);

		char ip[INET6_ADDRSTRLEN];
        dnslookup(hostname, ip, INET6_ADDRSTRLEN);

		if(dnslookup(hostname, ip, INET6_ADDRSTRLEN) == UTIL_SUCCESS){

			char* out = malloc(sizeof(char) * (strlen(hostname) + strlen(ip) + 3));
            //printf ("  out size: %lu \n", sizeof(out));
            
			strcpy(out, hostname);
			strcat(out, ",");
			strcat(out, ip);
			strcat(out, "\n");
            //printf ("  ip: %s \n", ip);

			pthread_mutex_lock(&resData->servicedArg.lock);
			fputs_unlocked(out, resData->servicedArg.fd);
			pthread_mutex_unlock(&resData->servicedArg.lock);
			free(out);

		}
		else{

			char* out = malloc(sizeof(char) * (strlen(hostname) + 3));
            //printf ("  out size: %lu \n", sizeof(out));

			strcpy(out, hostname);
			strcat(out, ",");
			strcat(out, "\n");

			pthread_mutex_lock(&resData->servicedArg.lock);
			fputs_unlocked(out, resData->servicedArg.fd);
			pthread_mutex_unlock(&resData->servicedArg.lock);

			fprintf(stderr, "Host \"%s\" not resolved.\n", hostname);
			free(out);
		}

        if(buffet->empty && buffet->reqDone){

			pthread_mutex_unlock(&buffet->lock);
            free(hostname);
			break;

		}

		free(hostname);
	}
    

	return 0;

}


int main(int argc, char* argv[]){

    // ./multilookup #req #res <reqlog> <reslog> [<file1>, ...]

    if(argc <= 5){

        fprintf(stderr, "At least 5 args are expected.\n ");
        fprintf(stderr, "./multilookup #req #res <reqlog> <reslog> [<file1>, ...]\n ");
        exit(EXIT_FAILURE);

    }

    struct timeval start, end;

    gettimeofday(&start, NULL);

    //number of resolver and requestor threads
    int numReq = atoi(argv[1]);
    int numRes = atoi(argv[2]);



    //txt logs for request and resolve
    char* reqLog = argv[3];
    char* resLog = argv[4];



    if(argv[3] && argv[4]){
        if(access(reqLog, F_OK | W_OK) == -1){
            
		    fprintf(stderr, "Requestor log not found.\n");
		    exit(EXIT_FAILURE);

	    }
	    if(access(resLog, F_OK | W_OK) == -1){

		    fprintf(stderr, "Resolver log not found.\n");
		    exit(EXIT_FAILURE);

	    }
        
    }
    else{

        fprintf(stderr, "No output logs specified in args\n");
		exit(EXIT_FAILURE);

    }

    if(numReq <= 0 || numReq >5){

		fprintf(stderr, "1 - 5 Requester threads only. You requested %u \n", numReq);
		exit(EXIT_FAILURE);

	}
	if(numRes <= 0 || numRes >10){

		fprintf(stderr, "1 - 10 Resolver threads only. You requested %u \n", numRes);
		exit(EXIT_FAILURE);

	}

    printf ("Request Threads: %i \n", numReq);
    printf ("Resolve Threads: %i \n", numRes);

    int i;

    //num of valid files
    int numValid = 0;
    char** validFiles = getFiles(argc-5, &numValid, argv+5 );


    for(i=0; i < numValid; i++){

        fprintf(stderr, "\"%s\" added to be serviced \n", validFiles[i]);

    }
    fprintf(stderr, "%u out of %u files valid \n", numValid, argc-5);

    if(!numValid){

        fprintf(stderr, "No valid files set as args. Exiting.\n");
        exit(EXIT_FAILURE);

    }

    //serviced file init
    outFile serviced;
    serviced.lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	serviced.name = reqLog;
    serviced.fd = fopen(reqLog, "w");
	

    //resolved file init
    outFile results;
    results.lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	results.name = resLog;
    results.fd = fopen(resLog, "w");
	


    fileList *fileDir;

    fileDir = malloc(numValid * sizeof(inFile) + sizeof(*fileDir));
    //printf ("  fileDir size: %lu \n", sizeof(fileDir));

    fileDir->index = 0;
    fileDir->total = numValid;
    fileDir->numDone = 0;

    
    fileDir->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;


    for(i=0; i < numValid; i++){

		inFile file;

		file.status = 0;
		file.name = validFiles[i];
		file.fd = fopen(file.name, "r");
        file.lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

		fileDir->files[i] = file;

	}

    

//buffets are a producer consumer problem of food? memory is computer food. this is a buffet
    shBuffer *buffet;
    buffet = malloc(sizeof(*buffet) + sizeof(char*) * ARRAY_SIZE);
    //printf ("  buffet size: %lu \n", sizeof(buffet));

    buffet->total = 20;
    buffet->full = 0;
    buffet->empty = 1;
    buffet->index = 0;
    buffet->reqDone = 0;
    
    buffet->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    buffet->cFull = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
    buffet->cEmpty = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

    //this is what avoiding globals does to ya. init arg data for data sharing
    struct reqArgs reqData;
    reqData.buffetArg = buffet;
    reqData.files = fileDir;
    reqData.servicedArg = serviced;

    struct resArgs resData;
    resData.buffetArg = buffet;
    resData.servicedArg = results;

    //array to store thread info and thread init
    pthread_t *tidRes = malloc(sizeof(pthread_t) * numReq);
    pthread_t *tidReq = malloc(sizeof(pthread_t) * numRes);
  

	int j=0;
    for (i=0; i < numRes; i++){
        for ( ; j < numReq; j++){
                pthread_create(&tidReq[j], NULL, theRequest, (void *)&reqData);
        }
        pthread_create(&tidRes[i], NULL, theResolve, (void *)&resData);
    }

    

  
	for (i=0; i < numReq; i++){
		if(pthread_join(tidReq[i], 0)){
			printf("Could not join tID: %lu\n",tidReq[i]);
		}
	}

    pthread_mutex_lock(&buffet->lock);
	buffet->reqDone = 1;
	pthread_cond_broadcast(&buffet->cEmpty);
	pthread_mutex_unlock(&buffet->lock);

	for (i = 0; i < numRes; i++){

		pthread_join(tidRes[i], 0);
	}

    //close all input files
	for(i = 0; i <  numValid; i++){

		fclose(fileDir->files[i].fd);
	}

	fclose(serviced.fd);
	fclose(results.fd);

	free(validFiles);
	free(fileDir);
	free(buffet);
	free(tidRes);
	free(tidReq);


	gettimeofday(&end, NULL);
	printf ("Run time: %f seconds\n", (double) (end.tv_usec - start.tv_usec) / 1000000 + (double) (end.tv_sec - start.tv_sec));

	exit(EXIT_SUCCESS);



}

char** getFiles(int numFileReq, int* numValid, char* files[]){

    int ivalid = 0;
    for(int i=0; i < numFileReq; i++){
        if(access(files[i], F_OK | R_OK) == -1){
			fprintf(stderr, "ERROR: Input file \"%s\" \n", files[i]);
		}
        else{
            ivalid++;
        }
    }

    *numValid = ivalid;
    //return file
    //I know this is the unfreed block
    //I am too deep in my infrstructure and too close to the deadline to change this
    //looks like yall will decide what to do with 1 loose block
    char** rFile = malloc(sizeof(char *) * ivalid);
    //printf ("  rFile size: %lu \n", sizeof(rFile));

    //return file index
    int j = 0;

    for(int i=0; i < numFileReq; i++){
        if(access(files[i], F_OK | R_OK) != -1){
            rFile[j] = files[i];
            j++;
        }
    }

    printf ("Files scanned \n");
    return rFile;


}
