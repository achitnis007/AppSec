/****************************************************************************
 * spell.c
 *
 * Application Security, Assignment 1
 *
 * Abhijit Chitnis (aac664) - func prototypes from code written by Ben Halperin
 ***************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// in dictionary.h
// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
// #define LENGTH 45
// #define HASH_SIZE 2000
// #define MAX_MISSPELLED 1000
// 
// typedef struct node
// {
//     char word[LENGTH + 1];
//     struct node* next;
// }
// node;
// 
// typedef node* hashmap_t;

/**
 * Array misspelled is populated with words that are misspelled. Returns the length of misspelled.
 */
/**
 * Inputs:
 *  fp:         A file pointer to the document to check for spelling errors.
 *  hashtable:  The hash table used to determine spelling
 *  misspelled: An empty char* array to be populated with misspelled words.
 *              This array will never be greater than 1000 words long.
 *            
 * Returns:
 *  int:        The number of words in the misspelled arary.
 *
 * Modifies:
 *  misspelled: This array will be filled with misspelled words.
 *
 * Example:
 *  int num_misspelled = check_words(text_file, hashtable, misspelled);
 **/
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
}


/**
 * Returns true if word is in dictionary else false.
 */
/**
 * Inputs:
 *  word:       A word to check the spelling of.
 *  hashtable:  The hash table used to determine spelling
 *            
 * Returns:
 *  bool:       A boolean value indicating if the word was correctly spelled.
 *
 * Modifies:
 *  
 * Example:
 *  bool correct  = check_word(word, hashtable);
 **/
bool check_word(const char* word, hashmap_t hashtable[])
{
    hashmap_t cursor = NULL;
    
    int word_len = strlen(word);
    if (word_len > LENGTH){
        printf("Word being checked [%s] has [%d] chars - exceeds max word length of [%d]\n",word, word_len, LENGTH);
	return FALSE;
    }

    // check if input word is not made of ascii chars - if so, 
    // return FALSE;
    //
    // check if word is all digits - 
    // return TRUE
    //
    // check if first and/or last char is a punctuation mark - if so, chop the end with punc
    // and then check for the word below

    int bucket = hash_function(word);
    if ((bucket < 0) || (bucket >= HASH_SIZE)){
       printf("hash_function returned an illegal index [%d] for input word [%s]\n",bucket, word);
       return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        if (strcmp(word,cursor->word) == 0)
	    return TRUE;
	cursor = cursor->next;
    }

    // check dict hashmap for lowercase word from input file - that's acceptable
    int bucket = hash_function(strlwr(word));
    if ((bucket < 0) || (bucket >= HASH_SIZE)){
       printf("hash_function returned an illegal index [%d] for lowercase input word [%s]\n",bucket, strlwr(word));
       return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        if (strcmp(word,cursor->word)
	    return TRUE;
	cursor = cursor->next;
    }

    return FALSE;
}


/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
/**
 * Inputs:
 *  dictionary_file:    Path to the words file.
 *  hashtable:          The hash table to be populated.
 *            
 * Returns:
 *  bool:       Whether or not the hashmap successfully populated.
 *
 * Modifies:
 *  hashtable: This hashmap should be filled with words from the file provided.
 *
 * Example:
 *  bool success = load_dictionary("wordlist.txt", hashtable);
 **/
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    // open dictionary_file is null, return FALSE
    FILE *fp;
    char str[LENGTH+1];
    
    fp = fopen(dictionary_file, "r");
    if (fp == NULL){
	printf("Could not open dictionary wordlist file %s\n", dictionary_file);
	return FALSE;
    }

    if (sizeof(hashtable) <= 0){
	printf("Size of hashtable passed in has zero length\n");
	// allocate space for 2000 node* and initialize passed in hashtable
	hashtable = malloc(sizeof(node*) * HASH_SIZE);
	return FALSE;
    }

    // initialize hashtable - array of linked lists to NULL
    for (int i=0; i<HASH_SIZE; i++){
        hashtable[i] = NULL; 
    }

    while (fgets(str, LENGTH+1, fp) != NULL){
        // allocate mem for a new node
	node* new_node_p = malloc(sizeof(node));

	// initialize the new node with word read from dict
	new_node_p->word = str;
	new_node_p->next = NULL

	// find the bucket id for the word using hash_function
	int bucket = hash_function(str); 
	if ((bucket < 0) || (bucket >= HASH_SIZE)){
	   printf("hash_function returned an illegal index [%d] for dict word [%s]\n",bucket, str);
	   return FALSE;
	}

	if (hashtable[bucket] == NULL){
	    hashtable[bucket] = new_node_p; 
	}
	else{
	    new_node_p->next = hashtable[bucket];
	    hashtable[bucket] = new_node_p;
	}
    }

    fclose(fp);

    return TRUE; 
}


/**
 * Already implemented in dictionary.c
 **/
// int hash_function(const char* word);
