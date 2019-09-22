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

#define LINEBUFF 2048
#define DICTLINEBUFF 1024

#define TRUE 1
#define FALSE 0

// #define MY_TESTING_ON
#undef MY_TESTING_ON

// avail in dictionary.h
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
    char line[LINEBUFF];
    char* ip_word;
    int num_misspelled = 0;
    
    #ifdef MY_TESTING_ON
    int i=0;
    hashmap_t cur = NULL;
    // print hashtable i.e. dictionary
    printf("Printing dictionary from check_words() function\n");
    while (hashtable[i] != NULL){
        cur = hashtable[i]->next;
        printf("\nBucket [%d] - dict word(s) [%s], ", i, hashtable[i]->word);
	while (cur != NULL){
	    printf("[%s], ", cur->word );
	    cur = cur->next;
	}
	i++;
    }
    #endif
	
    /* **************************
    misspelled[0] = (char *) malloc(MAX_MISSPELLED * sizeof(char *));
    if (misspelled != NULL){
        // initialize all string pointers to NULL
	for (int i=0; i < MAX_MISSPELLED; i++){
		misspelled[i] = NULL;
	}
    }
    else {
        exit(1);
    }
    ***************************/
	
    while (fgets(line, LINEBUFF, fp) != NULL){
        ip_word = strtok(line, " \t\r\n");
	while (ip_word != NULL){
            if (!check_word(ip_word, hashtable)) {
                // malloc space for this word and add to misspelled array    
		misspelled[num_misspelled] = (char *) malloc(strlen(ip_word)+1);
		// strcpy(misspelled[num_misspelled], ip_word);
		if ((strncpy(misspelled[num_misspelled], ip_word, strlen(ip_word)+1)) != NULL){
                    #ifdef MY_TESTING_ON
		    printf("Input Word [%s] has length [%d]\n", ip_word, (int) strlen(ip_word));
		    printf("misspelled[%d] is [%s] has length [%d]\n", num_misspelled, misspelled[num_misspelled], (int) strlen(misspelled[num_misspelled]));
                    #endif
		    num_misspelled++;
		}
		else
                    exit(1);
            }
	    ip_word = strtok(NULL, " \t\r\n");
	}
    }
	
/********
    #ifdef MY_TESTING_ON
    printf("Total misspelled words [%d]\nList of misspelled words\n",num_misspelled);
    for (int i=0; i < num_misspelled; i++) {
	printf("[%d]   [%s]\n", i, misspelled[i]);
    }
    #endif
*********/
	
    return num_misspelled;
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
    bool punct_begin = FALSE, punct_end = FALSE;
    char chkword[LENGTH+1], chkword_lower[LENGTH+1];
 
    int word_len = strlen(word);

    // cursor points to the current node of the selected hashtable bucket linked list
    hashmap_t cursor = NULL;
    
    #ifdef MY_TESTING_ON
    printf("\nword to be spell-checked is [%s]\n", word);
    #endif

    if (word_len > LENGTH){
        for (int i=0; i<word_len; i++){
            if (word[i] >= '0' && word[i] <= '9'){
                num_cnt++;
	    }
        }
	if (num_cnt == word_len){
            #ifdef MY_TESTING_ON
            printf("Word being checked [%s] has [%d] chars - spellcheck passed since it's numeric\n",word, word_len);
            #endif
	    return TRUE;
	}
	else{
            #ifdef MY_TESTING_ON
            printf("Word being checked [%s] has [%d] chars - exceeds max word length of [%d]\n",word, word_len, LENGTH);
            #endif
	    return FALSE;
	}
    }

    // count alpha & numeric chars 
    for (int i=0; i<word_len; i++){
        if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z')){
	    alpha_cnt++;
	}
	else if (word[i] >= '0' && word[i] <= '9'){
            num_cnt++;
	}
	else if (ispunct(word[i])){
            if ((i==0) && (i==word_len-1)) return TRUE;
	    if (i == 0) punct_begin = TRUE;
	    if (i == word_len-1) punct_end = TRUE;
	}
	else if ((word[i] < 32) || (word[i] == 127)){
	    return FALSE;
	}
    }

    // word is a number - passes spell check automatically
    if (num_cnt == word_len) {
        #ifdef MY_TESTING_ON
	printf("Word is a numeric string - spell check passed [%s]\n", word);
        #endif
        return TRUE;
    }

    // word is made of alphabets - copy to chkword for dir lookup
    if (alpha_cnt == word_len)
	// strcpy(chkword, word);
	strncpy(chkword, word, (int) sizeof(chkword));

    // check if first char is a punctuation mark - if so, copy from 2nd char to end
    // else copy the entire word to chkword
    if (punct_begin)
	// strcpy(chkword, word+1);
	strncpy(chkword, word+1, (int) sizeof(chkword));
    else
	// strcpy(chkword, word);
	strncpy(chkword, word, (int) sizeof(chkword));

    // if the last char of word is punc, truncate chkword string to eliminate the last punc mark
    if (punct_end){
	int chkword_len = strlen(chkword);
		
	// null terminate string
	chkword[chkword_len-1] = '\0';  
    }

    #ifdef MY_TESTING_ON
    printf("word to be spell-checked after stripping leading/trailing punct if present is [%s]\n", chkword);
    #endif
    
    int bucket = hash_function(chkword);
	
    #ifdef MY_TESTING_ON
    printf("bucket id returned by hash_function is [%d]\n", bucket);	
    #endif
    
    if ((bucket < 0) || (bucket >= HASH_SIZE)){
        #ifdef MY_TESTING_ON
        printf("hash_function returned an illegal index [%d] for input word [%s]\n",bucket, chkword);
        #endif
        return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        #ifdef MY_TESTING_ON
	printf("input word [%s], dict word at bucket # [%d] is [%s]\n", chkword, bucket, cursor->word);
        #endif
        // if (!strcmp(chkword,cursor->word))
        if (!strncmp(chkword, cursor->word, strlen(cursor->word))){
	    if (strlen(chkword) == strlen(cursor->word)){
                #ifdef MY_TESTING_ON
                printf("Input Word [%s] found in dict [%s] - correctly spelled\n", chkword, cursor->word);
                #endif
                return TRUE;
	    }
	}
	cursor = cursor->next;
    }

    // word (minus puncs at beginning and end) did not pas muster - try lowercase version
    for (int i=0; i <= strlen(chkword); i++) {
	chkword_lower[i] = tolower(chkword[i]);
    }

    #ifdef MY_TESTING_ON
    printf("word to be spell-checked after conv to lowercase is [%s]\n", chkword_lower);
    #endif
    bucket = hash_function(chkword_lower);
    if ((bucket < 0) || (bucket >= HASH_SIZE)){
        #ifdef MY_TESTING_ON
        printf("hash_function returned an illegal index [%d] for lowercase input word [%s]\n",bucket, chkword_lower);
        #endif
        return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        // if (!strcmp(chkword_lower,cursor->word)) 
        if (!strncmp(chkword_lower, cursor->word, strlen(cursor->word))){
	    if (strlen(chkword_lower) == strlen(cursor->word)){
                #ifdef MY_TESTING_ON
                printf("Input Word [%s] in lower case [%s] found in dict [%s] - correctly spelled\n", chkword, chkword_lower, cursor->word);
                #endif
                return TRUE;
	    }
	}
	cursor = cursor->next;
    }

    #ifdef MY_TESTING_ON
    printf("word to be spell-checked [%s] or [%s] not found in dict - it's misspelled\n", chkword, chkword_lower);
    #endif

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
    char *dict_word;
    // char str[LENGTH+1]; - used in prev implementation
    char dict_line[DICTLINEBUFF];
    
    fp = fopen(dictionary_file, "r");
    if (fp == NULL){
        #ifdef MY_TESTING_ON
        printf("Could not open dictionary wordlist file %s\n", dictionary_file);
        #endif
	return FALSE;
    }
    else{
        #ifdef MY_TESTING_ON
	printf("Opened dictionary file [%s] successfully\n", dictionary_file);
        #endif
    }

    /*********************************
    // allocate space for 2000 node* and initialize passed in hashtable
    hashtable[0] = (node *) malloc(sizeof(node *) * HASH_SIZE);
    if (hashtable == NULL){
        #ifdef MY_TESTING_ON
        printf("Malloc to allocate memory for hashtable failed - exiting program\n");
        #endif
        exit(1);
    }
    ********************************/
	
    // initialize hashtable - array of linked lists to NULL
    for (int i=0; i<HASH_SIZE; i++){
        hashtable[i] = NULL; 
        // (node *) hashtable + i = NULL; 
    }

    while (fgets(dict_line, DICTLINEBUFF, fp) != NULL){
        dict_word = strtok(dict_line, " \t\r\n");
	while (dict_word != NULL){
            // allocate mem for a new node
	    node* new_node_p = (node *) malloc(sizeof(node));

	    // initialize the new node with word read from dict
	    // strcpy(new_node_p->word, dict_word);
	    strncpy(new_node_p->word, dict_word, (int) sizeof(new_node_p->word));
	    new_node_p->next = NULL;

	    // find the bucket id for the word using hash_function
	    int bucket = hash_function(dict_word); 
		
            #ifdef MY_TESTING_ON
	    // printf("Dictionary word is [%s] and bucket id is [%d]\n", dict_word, bucket);
            #endif
	    if ((bucket < 0) || (bucket >= HASH_SIZE)){
                #ifdef MY_TESTING_ON
	        printf("hash_function returned an illegal index [%d] for dict word [%s]\n",bucket, dict_word);
                #endif
	        return FALSE;
	    }

	    if (hashtable[bucket] == NULL){
                hashtable[bucket] = new_node_p; 
	    }
	    else{
	        new_node_p->next = hashtable[bucket];
	        hashtable[bucket] = new_node_p;
	    }
	    dict_word = strtok(NULL, " \t\r\n");
	}
    }

    fclose(fp);
	
    #ifdef MY_TESTING_ON
    printf("Printing dictionary hashtable from load_dictionary() before exiting\n");
    hashmap_t cur = NULL;
    int i = 0;
    while (hashtable[i] != NULL){
	cur = hashtable[i]->next;
	printf("\nBucket [%d] - dict word(s) [%s], ", i, hashtable[i]->word);
	while (cur != NULL){
            printf("[%s], ", cur->word );
            cur = cur->next;
        }
	i++;
    }
    #endif

    return TRUE; 
}
