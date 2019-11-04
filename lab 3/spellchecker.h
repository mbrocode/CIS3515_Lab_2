/*
 * @author: Manuel Broco
 * Filename: spellchecker.h
 * Description: CIS3207 Project3 Networked Spell Checker
 */

#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H
#define WORKERS 3

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

typedef struct queue_node {
    char data[128];
    struct queue_node* before;
    struct queue_node* after;
} queue_node;

typedef struct queue {
    int size;
    struct queue_node* head;
} queue;

typedef struct dictionary {
    int size;
    char** words;
} dictionary;

dictionary buildDictionary( const char* );
queue* createQ();
char* popQ( queue* root );
int printQ( queue* root );
int enqueue( queue* root, char* str );
int spellcheck( dictionary, const char* );
void to_lower( char* );

#endif