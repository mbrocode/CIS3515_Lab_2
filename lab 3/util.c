/*
 * @author: Manuel Broco
 * Filename: util.c
 * Description: CIS3207 Project3 Networked Spell Checker
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "spellchecker.h"

const int MAX_WRD_LEN = 35;

///________________________________________________________________________________
//SPELLCHECKER FUNCTIONS __________________________________________________________

/* TO_LOWER COMMAND ---------------------------------------------------------------
 * spellchecker Function: makes all characters from input lowercase
 * input:   str*
 * output:  void
*/
void to_lower(char* str) {
    //iterate through str using standard toLower function at each index
    for (int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

/* BUILD DICTIONARY COMMAND ----------------------------------------------------------
 * spellchecker Function: reads file and builds dictionary of words from it
 * input:   char*
 * output:  struct dictionary
*/
dictionary buildDictionary(const char* path) {
    //open file pointer
    FILE* fp;
    dictionary d;
    d.size = 0;
    //check for null path return error message
    //open file in read mode. The file must exist.
    if ((fp = fopen(path, "r")) == NULL) {
        printf("buildDictionary ERROR: path == NULL\n");
        return d;
    }
    char ch;
    int lines = 0;
    //loop to get all words in file
    do {
        ch = fgetc(fp);
        //increment lines for each newline
        if (ch == '\n'){
            lines++;
        }
    } while (ch != EOF);
    rewind(fp);
    //make new dictionary the correct size
    d.size = lines;
    d.words = malloc(lines * sizeof(char*));
    for (int i = 0; i < d.size; i++) {
        d.words[i] = malloc(MAX_WRD_LEN * sizeof(char*));
        fgets(d.words[i], MAX_WRD_LEN, fp);
        int len = strlen(d.words[i]);
        if (d.words[i][len-1] == '\n') {
            d.words[i][len-1] = 0;
        }
        to_lower(d.words[i]);
    }
    return d;
}

/* SPELLCHECK COMMAND ---------------------------------------------------------------
 * spellchecker Function: checks word against words in dictionary, if correctly 
 *                        spelled returns positive number, else returns 0
 * input:   dictionary, char*
 * output:  int
*/
int spellcheck(dictionary dict, const char* word) {
    int i = 0, j = dict.size -1;
    while (i <= j) {
        int m = (i + j) / 2;
        int cmp = strcmp(dict.words[m], word);
        if (cmp == 0) {
            return m;
        } else if (cmp < 0) {
            i = m + 1;
        } else {
            j = m - 1;
        }
    }
    return 0;
}

///________________________________________________________________________________
//QUEUE FUNCTIONS _________________________________________________________________

/* CREATE QUEUE COMMAND ---------------------------------------------------------------
 * queue Function: builds queue
 * input:   
 * output:  queue*
*/
queue* createQ() {
    queue* q;
    q = malloc(sizeof(queue));
    q->size = 0;
    q->head = NULL;
    return q;
}

/* PRINT QUEUE COMMAND ------------------------------------------------------------
 * queue Function: prints queue, used during testing
 * input:   queue*
 * output:  int
*/
int printQ(queue* q) {
    //check for empty queue
    if (q == NULL || q->size == 0) {
        printf("printQ ERROR: empty fifo queue\n");
        return 0;
    }
    //build node to work on
    queue_node* iter;
    iter = q->head;
    //print first node in queue
    printf("%s,", iter->data);
    //iterate through rest of queue and print data
    while (iter->after != NULL) {
        iter = iter->after;
        printf("%s,", iter->data);
    }
    printf("\n");
    return 1;
}

/* ENQUEUE COMMAND ---------------------------------------------------------------
 * queue Function: push node onto queue
 * input:   queue*, char*
 * output:  int
*/
int enqueue(queue* q, char* str) {
    //printQ(q);
    //create null node to enqueue
    queue_node* node;
    node = malloc(sizeof(queue_node));
    node->after = NULL;
    node->before = NULL;
    //fill created node
    strcpy(node->data, str);
    //if first node in queue, make the head
    if (q->size == 0) {
        q->head = node;
        //increment queue size to account for end
        q->size++;
        return 1;
    }
    queue_node* iter;
    iter = q->head;
    while (iter->after != NULL) {
        iter = iter->after;
    }
    iter->after = node;
    node->before = iter;
    q->size++;
    return 1;
}

/* POP QUEUE FUNCTION ------------------------------------------------------------
 * queue Function: pop top node from queue and return data as a "string"
 * input:   queue*
 * output:  char*
*/
char* popQ(queue* q) {
    //printQ(q);
    //q.size == 1 means first node in queue
    if (q->size == 1) {
        char* str = malloc(256 * sizeof(char));
        strcpy(str, q->head->data);
        q->head = NULL;
        q->size--;
        return str;
    //if q is null or less than 1 there is an error
    }else if (q == NULL || q->size < 1) {
        printf("popQ ERROR: q == NULL\n");
        return NULL;
    }
    //otherwise pop top node from queue
    char* str = malloc(256 * sizeof(char));
    strcpy(str, q->head->data);
    q->head = q->head->after;
    q->head->before = NULL;
    q->size--;
    return str;
}