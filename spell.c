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
    int alpha_cnt = 0, num_cnt = 0;
    char chkword[LENGTH+1], chkword_lower[LENGTH+1];

 
    // cursor points to the current node of the selected hashtable bucket linked list
    hashmap_t cursor = NULL;
    
    int word_len = strlen(word);
    if (word_len > LENGTH){
        printf("Word being checked [%s] has [%d] chars - exceeds max word length of [%d]\n",word, word_len, LENGTH);
	return FALSE;
    }

    // count alpha & numeric chars 
    for (int i=0; i<word_len; i++){
        if (word[i] >= 'A' && word[i] <= 'Z' || word[i] >= 'a' && word[i] <= 'z'){
	    alpha_cnt++;
	}
	else if (word[i] >= '0' && word[i] <= '9'){
	    num_cnt++;
	}
	else if (word[i] < 32 || word[i] == 127){
	    return FALSE;
	}
    }

    // word is a number - passes spell check automatically
    if (num_cnt == word_len)
        return TRUE;

    // word is made of alphabets - copy to chkword for dir lookup
    if (alpha_cnt == word_len)
	strcpy(chkword, word);

    // check if first char is a punctuation mark - if so, copy from 2nd char to end
    // else copy the entire word to chkword
    if ( (word[0] >= 33 && word[0] <= 47) || (word[0] >= 91 && word[0] <= 96) || (word[0] >= 123 && word[0] <= 126) )
	strcpy(chkword, word+1);
    else
	strcpy(chkword, word);

    // if the last char of word is punc, truncate chkword string to eliminate the last punc mark
    if ( (word[word_len-1] >= 33 && word[word_len-1] <= 47) || (word[word_len-1] >= 91 && word[word_len-1] <= 96) || (word[word_len-1] >= 123 && word[word_len-1] <= 126) ){
	int chkword_len = strlen(chkword);
	chkword[chkword_len-1] = '\0';  // truncate last char of chkword since it's a punc
    }

    int bucket = hash_function(chkword);
    if ((bucket < 0) || (bucket >= HASH_SIZE)){
       printf("hash_function returned an illegal index [%d] for input word [%s]\n",bucket, chkword);
       return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        if (strcmp(chkword,cursor->word) == 0)
	    return TRUE;
	cursor = cursor->next;
    }

    // word (minus puncs at beginning and end) did not pas muster - try lowercase version
    chkword_lower = strlwr(chkword);

    int bucket = hash_function(chkword_lower);
    if ((bucket < 0) || (bucket >= HASH_SIZE)){
       printf("hash_function returned an illegal index [%d] for lowercase input word [%s]\n",bucket, chkword_lower);
       return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        if (strcmp(chkword_lower,cursor->word)
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
