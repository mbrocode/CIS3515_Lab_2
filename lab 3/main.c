/*
 * @author: Manuel Broco
 * Filename: main.c
 * Description: CIS3207 Project3 Networked Spell Checker
 */

#include "spellchecker.h"

queue* logQ;
queue* socketQ;
void* logger();
void* worker();
char* logNum(int);
pthread_t tid[WORKERS+1];
pthread_cond_t logCondition, socketCondition;
pthread_mutex_t logLock, socketLock;
dictionary dict;
const char* DIRECTORY = "words.txt";
const int PORT = 12313;

//function logs socket number to spellcheck request
char* logNum(int val) {
    char* lgNum = malloc(256 * sizeof(lgNum));
    sprintf(lgNum, "%d", val);
    return lgNum;
}

void* logger() {
    while (1) {
        //Lock the mutex for logger
        pthread_mutex_lock(&logLock);
        //Unlock mutex and relock after conditionals
        pthread_cond_wait(&logCondition, &logLock);
        while (logQ->size > 0) {
            //create file pointer
            FILE* fp;
            //open the file server.log in append mode. Appends to a file. 
            //Writing operations, append data at the end of the file. 
            //The file is created if it does not exist.
            fp = fopen("serverLog", "a");
            char* message = popQ(logQ);
            printf("%s", message);
            //append serverLog with message
            fputs(message, fp);
            //close file pointer
            fclose(fp);
        }
        //unlock the logger lock mutex
        pthread_mutex_unlock(&logLock);
    }
}

void* worker() {
    printf("Worker Thread Created\n");
    int ender = 0;
    while (ender == 0) {
        //Lock the socet mutex
        pthread_mutex_lock(&socketLock);
        //Unlock mutex and relock after conditionals
        pthread_cond_wait(&socketCondition, &socketLock);
        //check for anything in the queue
        if (socketQ->size < 1) {
            //unlock socket lock mutex
            pthread_mutex_unlock(&socketLock);
            continue;
        }
        char* end;
        int new_socket = strtol(popQ(socketQ), &end, 10);
        //unlock mutex 
        pthread_mutex_unlock(&socketLock);
        int len;
        //create buffer to store user input message / word
        char buffer[256];
        char* msgRequest = "After entering '^]', enter '^Z' on telnet> to Stop\n";
        write(new_socket, msgRequest, strlen(msgRequest));
        while ((len = recv(new_socket, buffer, 256, 0)) > 0) {
            buffer[strcspn(buffer, "\r\n")] = 0;
            //must make all words lowercase to check against dictionary
            to_lower(buffer);
            char message[256];
            strcpy(message, logNum(new_socket));
            strcat(message, " - ");
            strcat(message, buffer);
            //create integer to store num greater than 0 for correctly spelled word
            int bingo = spellcheck(dict, buffer);
            if (bingo) {
                //if message is spelled correctly and recognizable
                strcat(message, " BINGO! SPELLED CORRECTLY\n");
            //int bingo is 0 for incorrectly spelled word
            } else {
                //if message is incorrectly spelled or unrecognizable
                strcat(message, " NEGATIVE! MISSPELLED\n");
            }
            //print the message to the worker 
            write(new_socket, message, strlen(message));
            //lock the log mutex
            pthread_mutex_lock(&logLock);
            enqueue(logQ, message);
            //unlock the log mutex
            pthread_mutex_unlock(&logLock);
            //blocked log thread is signalled to wakeup
            pthread_cond_signal(&logCondition);
        }
    }
    printf("Worker Thread Closed");
}

/* MAIN ---------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    /*
    if(argc == 1){
		printf("No port number entered.\n");
		return -1;
	}*/
    //starting prog with a.out starts argc with 1 thus
    if (argc >= 2) {
        dict = buildDictionary(argv[1]);
    } else {
        dict = buildDictionary(DIRECTORY);
    }
    //dict = buildDictionary(DIRECTORY);
    struct sockaddr_in client, server;
    int connectionSocket, clientSocket, optval, c;

    //create socket
    connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    //set socket and dont hang onto it
    setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    if (argc == 3) {
        char *end;
        server.sin_port = htons((int)strtol(argv[2], &end, 10));
    } else {
        server.sin_port = htons(PORT);
    }

    if (bind(connectionSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Bind failed!\n");
        return 1;
    }

    logQ = createQ();
    socketQ = createQ();
    //initialize the log mutex referenced by mutex with attributes specified by attr
    if (pthread_mutex_init(&logLock, NULL) != 0) {
        printf("Log Mutex Initialization failed!\n");
        exit (1);
    }
    //initialize the socket mutex referenced by mutex with attributes specified by attr
    if (pthread_mutex_init(&socketLock, NULL) != 0) {
        printf("Socket Mutex Initialization failed!\n");
        exit (1);
    }
    //create worker
    for (int i = 0; i < WORKERS; i++) {
        //create the new worker threads specified by const WORKERS
        int err = pthread_create(&(tid[i]), NULL, &worker, NULL);
        if (err != 0) {
            printf("Worker Thread: %s FAILED!", strerror(err));
        }
    }
    //create a new logger thread 
    int err = pthread_create(&(tid[WORKERS]), NULL, &logger, NULL);
    if (err != 0) {
        printf("Logger Thread: %s FAILED!", strerror(err));
        exit(1);
    }

    listen(connectionSocket, 3);
    //printQ(logQ);
    //printQ(socketQ);
    printf("Connection successful\n");

    c = sizeof(struct sockaddr_in);
    while ((clientSocket = accept(connectionSocket, (struct sockaddr*)&client, (socklen_t*)&c))) {
        //Lock the socket mutex
        pthread_mutex_lock(&socketLock);
        enqueue(socketQ, logNum(clientSocket));
        //unlock the Socket mutex
        pthread_mutex_unlock(&socketLock);
        //blocked Socket thread is signalled to wakeup
        pthread_cond_signal(&socketCondition);
    }
}
