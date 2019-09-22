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

#define LINEBUFF 512
#define DICTLINEBUFF 512

#define TRUE 1
#define FALSE 0



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
    char trimmed_word[LENGTH+1];
    char line[LINEBUFF];
    char* ip_word;
    int num_misspelled = 0;    
    
    int i=0;
    hashmap_t cur = NULL;
	// printf("Printing dictionary from check_words() function\n");
    while (hashtable[i] != NULL){
        cur = hashtable[i]->next;
        // printf("\nBucket [%d] - dict word(s) [%s], ", i, hashtable[i]->word);
		while (cur != NULL){
			// printf("[%s], ", cur->word );
			cur = cur->next;
		}
		i++;
    }
	
    while (fgets(line, LINEBUFF, fp) != NULL){
        ip_word = strtok(line, " \t\r\n");
		while (ip_word != NULL){
			if (!check_word(ip_word, hashtable)) {
				// strip punctuation from front & back
				if (ispunct(ip_word[0]) && ispunct(ip_word[strlen(ip_word)-1]))
				    strncpy(trimmed_word, ip_word+1, strlen(ip_word)-2);
				else if (ispunct(ip_word[0]))
				    strncpy(trimmed_word, ip_word+1, strlen(ip_word)-1);
				else if (ispunct(ip_word[strlen(ip_word)-1]))
				    strncpy(trimmed_word, ip_word, strlen(ip_word)-1);
				else
				    strncpy(trimmed_word, ip_word, strlen(ip_word));

                                // null terminate the string copied
				trimmed_word[strlen(trimmed_word)] = '\0';

				// malloc space for this trimmed word and add to misspelled array    
				misspelled[num_misspelled] = (char *) malloc(strlen(trimmed_word)+1);
				if ((strncpy(misspelled[num_misspelled], trimmed_word, strlen(trimmed_word)+1)) != NULL){
				    // printf("Input Word [%s] has length [%d]\n", ip_word, (int) strlen(ip_word));
				    // printf("misspelled[%d] is [%s] has length [%d]\n", num_misspelled, misspelled[num_misspelled], (int) strlen(misspelled[num_misspelled]));
				    num_misspelled++;
				}
				else
				    exit(1);
			}
			ip_word = strtok(NULL, " \t\r\n");
		}
    }
	
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
    
    // printf("\nword to be spell-checked is [%s]\n", word);
    
    if (word_len > LENGTH){
        for (int i=0; i<word_len; i++){
            if (word[i] >= '0' && word[i] <= '9'){
                num_cnt++;
			}
        }
	if (num_cnt == word_len){
	    // printf("Word being checked [%s] has [%d] chars - spellcheck passed since it's numeric\n",word, word_len);    
	    return TRUE;
	}
	else{
	    // printf("Word being checked [%s] has [%d] chars - exceeds max word length of [%d]\n",word, word_len, LENGTH);
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
	// printf("Word is a numeric string - spell check passed [%s]\n", word);
        return TRUE;
    }

    // word is made of alphabets - copy to chkword for dir lookup
    if (alpha_cnt == word_len)
	strncpy(chkword, word, (int) sizeof(chkword));

    // check if first char is a punctuation mark - if so, copy from 2nd char to end
    // else copy the entire word to chkword
    if (punct_begin)
	strncpy(chkword, word+1, (int) sizeof(chkword));
    else
	strncpy(chkword, word, (int) sizeof(chkword));

    // if the last char of word is punc, truncate chkword string to eliminate the last punc mark
    if (punct_end){
	int chkword_len = strlen(chkword);
	chkword[chkword_len-1] = '\0';
    }

    // printf("word to be spell-checked after stripping leading/trailing punct if present is [%s]\n", chkword);
    
    int bucket = hash_function(chkword);
	    
    // printf("bucket id returned by hash_function is [%d]\n", bucket);	
    
    if ((bucket < 0) || (bucket >= HASH_SIZE)){    
        // printf("hash_function returned an illegal index [%d] for input word [%s]\n",bucket, chkword);
        return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
	// printf("input word [%s], dict word at bucket # [%d] is [%s]\n", chkword, bucket, cursor->word);
        if (strncmp(chkword, cursor->word, strlen(cursor->word)) == 0){
	    // printf("Input Word [%s], stripped word [%s] matched dict word [%s] - correctly spelled\n", word, chkword, cursor->word);    
	    return TRUE;
	}
	cursor = cursor->next;
    }

    // word (minus puncs at beginning and end) did not pas muster - try lowercase version
    for (int i=0; i <= strlen(chkword); i++) {
		chkword_lower[i] = tolower(chkword[i]);
    }

    // printf("word to be spell-checked after conv to lowercase is [%s]\n", chkword_lower);
    
    bucket = hash_function(chkword_lower);
    if ((bucket < 0) || (bucket >= HASH_SIZE)){        
        // printf("hash_function returned an illegal index [%d] for lowercase input word [%s]\n",bucket, chkword_lower);
        return FALSE;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        if (strncmp(chkword_lower, cursor->word, strlen(cursor->word)) == 0){
	    // printf("Input Word [%s] in lower case [%s] found in dict [%s] - correctly spelled\n", chkword, chkword_lower, cursor->word);             
	    return TRUE;
	}
	cursor = cursor->next;
    }

    // printf("***** Input Word [%s], stripped word [%s], lower case word [%s] did not match dict word - incorrectly spelled *****\n", word, chkword, chkword_lower);    

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
    char dict_line[DICTLINEBUFF];
    
    fp = fopen(dictionary_file, "r");
    if (fp == NULL){
        // printf("Could not open dictionary wordlist file %s\n", dictionary_file);
		return FALSE;
    }
	// printf("Opened dictionary file [%s] successfully\n", dictionary_file);

    // initialize hashtable - array of linked lists to NULL
    for (int i=0; i<HASH_SIZE; i++){
        hashtable[i] = NULL; 
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
				
			// printf("Dictionary word is [%s] and bucket id is [%d]\n", dict_word, bucket);
			if ((bucket < 0) || (bucket >= HASH_SIZE)){
				// printf("hash_function returned an illegal index [%d] for dict word [%s]\n",bucket, dict_word);
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
	
    return TRUE; 
}
