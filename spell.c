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
#define DICTLINEBUFF 512

#define TRUE 1
#define FALSE 0

/**
 * Checks if word passed in is made of printable ascii characters.
 */
/**
 * Inputs:
 *  word:       string to be validated if printable
 *            
 * Returns:
 *  bool:       true only if all chars are printable ascii
 *              false if even a single char is non-printable
 * Example:
 *  bool is_printable = is_printable_ascii(parsed_word);
 **/
bool is_printable_ascii(const char * word)
{
    for (int i=0; i<strlen(word); i++){
        if (!isascii(word[i])) {
            return false;
        }

        if (!isalnum(word[i]) && !ispunct(word[i])){
            return false;
        }
 
	// if ((word[i] < 33) || (word[i] == 127) || ((word[i] > 154) && (word[i] < 160)) || (word[i] > 165))
	if ((word[i] < 33) || (word[i] == 127) ){
            // printf("checking for is_printable_ascc() [%s] with length [%ld] is non-printable\n",word, strlen(word));
	    return false;
	}
    }
    // printf("checking for is_printable_ascc() [%s] is printable\n",word);
    return true;
}


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
    char trimmed_word[LENGTH+3];
    char line[LINEBUFF];
    char* ip_word;
    int num_misspelled = 0;
    int trimmed_word_len = 0;
    int ip_word_len = 0;
    bool misspelled_not_in_list = true;
    
    // initialize misspelled array of strings to NULL
    for (int i=0; i<MAX_MISSPELLED; i++){
        misspelled[i] = NULL; 
    }

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
            // if ip_word has non-alphanumeric (english or foreign) skip logging to misspelled
            // tokenize the next word and continue
            if (!is_printable_ascii(ip_word)){
	        ip_word = strtok(NULL, " \t\r\n");
                continue;
            }

	    if (!check_word(ip_word, hashtable)) {
		// printf("[%s] not found in dictionary\n",ip_word);
		// strip punctuation from front & back

                // afl fuzzer buffer overflow sigabrt
                // here the ip_word - word read from the input test file is being
                // copied to the trimmed_word string - while check_word() had detected
                // the word to > maximum allowed length (45 chars), that wasn't being checked
                // here in check_words() after returning from check_word()
                // copying a longer string into a shorter string caused buffer overflow

                // trim the input word to max LENGTH + 2 to accommodate for punctuation on either side
                // now even if the input word is crazy long, we are only considering the first LENGTH + 2
                // bytes and averting an overflow.

                // I had missed this memory bug until the fuzzer caught it
                                
                ip_word_len = strlen(ip_word);

                if (ip_word_len > LENGTH+2){
                    ip_word[LENGTH+2] = '\0';
                    ip_word_len = LENGTH + 2;
                }

		if (ispunct(ip_word[0]) && ispunct(ip_word[ip_word_len-1])){
		    trimmed_word_len = ip_word_len-2;
		    strncpy(trimmed_word, ip_word+1, trimmed_word_len);
		}
		else if (ispunct(ip_word[0])){
		    trimmed_word_len = ip_word_len-1;
		    strncpy(trimmed_word, ip_word+1, trimmed_word_len);
		}
		else if (ispunct(ip_word[ip_word_len-1])){
		    trimmed_word_len = ip_word_len-1;
		    strncpy(trimmed_word, ip_word, trimmed_word_len);
		}
		else {
		    trimmed_word_len = ip_word_len;
		    strncpy(trimmed_word, ip_word, trimmed_word_len);
		}

                // null terminate the string copied
		trimmed_word[trimmed_word_len] = '\0';

		// check if misspelled word is already in misspelled array
		misspelled_not_in_list = true;
		for (int i=0; i<num_misspelled; i++){
		    if (!strncmp(misspelled[i], trimmed_word, trimmed_word_len)){
                        misspelled_not_in_list = false;
		        break;
		    }
		}

		if (misspelled_not_in_list){	
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
    char chkword[LENGTH+3], chkword_lower[LENGTH+3];
 
    int word_len = strlen(word);

    // cursor points to the current node of the selected hashtable bucket linked list
    hashmap_t cursor = NULL;
    
    // printf("\nword to be spell-checked is [%s]\n", word);
    
    if (word_len > LENGTH + 2){
        for (int i=0; i<word_len; i++){
            if (word[i] >= '0' && word[i] <= '9'){
                num_cnt++;
            }
        }
	if (num_cnt == word_len){
	    // printf("Word being checked [%s] has [%d] chars - spellcheck passed since it's numeric\n",word, word_len);    
	    return true;
	}
	else{
	    // printf("Word being checked [%s] has [%d] chars - exceeds max word length of [%d]\n",word, word_len, LENGTH);
	    return false;
	}
    }

    // here word_len is 47 or less to accommodate for word of size LENGTHwith punct on either side
    // count alpha & numeric chars 

    for (int i=0; i<word_len; i++){
        if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'a' && word[i] <= 'z')){
	    alpha_cnt++;
	}
	else if (word[i] >= '0' && word[i] <= '9'){
		num_cnt++;
	}
	else if (ispunct(word[i])){
        	if ((i==0) && (i==word_len-1)) return true;
		if (i == 0) punct_begin = true;
		if (i == word_len-1) punct_end = true;
	}
	else if ((word[i] < 32) || (word[i] == 127) || (word[i] == 255)){
		return false;
	}
    }

    // word is a number - passes spell check automatically
    if (num_cnt == word_len) {
	// printf("Word is a numeric string - spell check passed [%s]\n", word);
        return true;
    }

    // word is made of alphabets - copy to chkword for dir lookup
    // if (alpha_cnt == word_len)
    strncpy(chkword, word, (int) sizeof(chkword));

    // check if first char is a punctuation mark - if so, copy from 2nd char to end
    // else copy the entire word to chkword
    if (punct_begin){
	strncpy(chkword, word+1, (int) sizeof(chkword));
    }
    else {
	strncpy(chkword, word, (int) sizeof(chkword));
    }

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
        return false;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
	// printf("input word [%s], dict word at bucket # [%d] is [%s]\n", chkword, bucket, cursor->word);
        if (strncmp(chkword, cursor->word, strlen(cursor->word)) == 0){
	    // printf("Input Word [%s], stripped word [%s] matched dict word [%s] - correctly spelled\n", word, chkword, cursor->word);    
	    return true;
	}
	cursor = cursor->next;
    }

    // word (minus puncs at beginning and end) did not pas muster - try lowercase version
    for (int i=0; i < strlen(chkword); i++) {
	chkword_lower[i] = tolower(chkword[i]);
    }
    chkword_lower[strlen(chkword)] = '\0';

    // printf("word to be spell-checked after conv to lowercase is [%s]\n", chkword_lower);
    
    bucket = hash_function(chkword_lower);
    if ((bucket < 0) || (bucket >= HASH_SIZE)){        
        // printf("hash_function returned an illegal index [%d] for lowercase input word [%s]\n",bucket, chkword_lower);
        return false;
    }

    cursor = hashtable[bucket];
    while (cursor != NULL){
        if (strncmp(chkword_lower, cursor->word, strlen(cursor->word)) == 0){
	    // printf("Input Word [%s] in lower case [%s] found in dict [%s] - correctly spelled\n", chkword, chkword_lower, cursor->word);             
	    return true;
	}
	cursor = cursor->next;
    }

    // printf("***** Input Word [%s], stripped word [%s], lower case word [%s] did not match dict word - incorrectly spelled *****\n", word, chkword, chkword_lower);    

    return false;
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


/**
 * Frees the hashtable used to store dictionary words for look-up
 */
/**
 * Inputs:
 *  hashtable: The hash table to be freed.
 *            
 * Returns:
 *  int:       Number of dictionary words (nodes) freed
 *
 * Modifies:
 *  hashtable: Memory allocated to hashtable should be freed
 *
 * Example:
 *  int dict_words = free_dictionary(hashtable);
 **/
int free_dictionary(hashmap_t hashtable[])
{
    int dict_words_freed = 0;

    // hashmap_t is alias fpr node *
    hashmap_t cur_node = NULL, next_node = NULL;

    for (int i=0; i<HASH_SIZE; i++){
        if(hashtable[i] == NULL)
            continue;
        while(hashtable[i]->next != NULL){
	    cur_node = hashtable[i];
            next_node = cur_node->next;
            while (next_node->next != NULL){
                cur_node = next_node; 
                next_node = next_node->next;            
	    }
            cur_node->next = NULL;
            free(next_node);
            next_node = NULL;
            dict_words_freed++;
        }
        free(hashtable[i]);
        hashtable[i] = NULL;
        dict_words_freed++;
    }

    return dict_words_freed;
}


/**
 * Frees the misspelled array of strings used to save misspelled words
 */
/**
 * Inputs:
 *  misspelled array:  misspelled array to be freed.
 *            
 * Returns:
 *  int:               Number of misspelled words freed
 *
 * Modifies:
 *  misspelled:        Memory allocated to misspelled string array should be freed
 *
 * Example:
 *  int misspelled_words = free_misspelled_list(misspelled);
 **/
int free_misspelled_list(char * misspelled[])
{
    int i = 0, misspelled_words_freed = 0;

    // added a check for i < MAX_MISSPELLED to prevent an invalid free outside the 
    // array limits based on Valgrind finding
    while ((i < MAX_MISSPELLED) && (misspelled[i] != NULL)){
        free(misspelled[i]); 
        misspelled[i] = NULL;
        misspelled_words_freed++;
        i++;
    }

    return misspelled_words_freed;
}
