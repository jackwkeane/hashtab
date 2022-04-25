/*
 * hashTab Supporting Functions
 * Program 2 assignment
 * CSE109 Fall 2021
 * student ID (e.g., idk321): jwk324
 * student Last name: Keane
 * student First name: Jack
 * gitHub ID: @jackwkeane
 */

 /* Note about this project:

    This project was a solid experience. 

    Most helpful resource:
        Understanding and implementing a Hash Table (in C): https://www.youtube.com/watch?v=2Ti5yvumFTU

    Quick notes:
        - This program creates a hashTable of an initial bucket size of 100 to store key-value pairs.
        - We use XOR encrption on the key to get a hashed key and then modulus that hashed key by the total bucket size to find the key-value pairs bucket index.
        - HashTable stores a max chain length of 5 and/or uses 75% of buckets before resizing.
        - We resize the hashTable because as worst chain lengths increase the time complexity of the hashTable becomes closer to O(n) instead of O(1).
        - We check the resize condition everytime we read a line from the text file we read as input. 
        - Effiecient memory allocation was aimed to achieve by freeing for each malloc. 

 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global_defs.h"
#include "syscalls.h"
#include "hashTab_funcs.h"


/* Defining the debug macro from the make file */
#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

/* Function header for a function that prints the visual of the hahsTable */
void printHashTableVisual(Node **);

/* Static Global Variables */
static size_t TOTALBUCKETS = INITIALBUCKETS; 
static int PERCENTAGEUSED = 0;
static int LONGESTCHAINCOUNT = 1;

/* Help Function defined in function header - 100% Completed*/
void help() {
    /* Print statment that instructs the user on what to enter */
    fprintf(stdout, "hashTab creates, retrieves, updates, and deletes key-value pairs in a hash table.\nKeys can be any integer in the range [0, 18446744073709551615] and values can be\nany integer in the range [-2147483648, 2147483647]. Commands to perform the above\nactions may be specified in the input text file as follows.\n\n# format is: action key [value] [# comment]\n# action is one of the following characters: c r u d\nc key value # if key already in table, value will be updated and a warning printed\nr key       # if key not in the table, a warning will be printed, otherwise value\nu key value # if key not in the table, value will be stored and a warning printed\nd key       # if key not in the table, a warning will be printed\n\nThe table auto-resizes to maintain \"optimum\" performance. Run program as follows.\n\nhashTab -i|--input <input file>\n\tor\nhashTab [-h|--help]\n\n");
    exit(0);
}

/* ParseArgs Function */
/* in: argc, argv */
void parseArgs(int argc, char *argv[]){   
    if (((strcmp(argv[1], "-h")) == 0) || ((strcmp(argv[1], "--help")) == 0)) { /* Checks to see if user is calling the help function */
        help(); /* Help function call */
        exit(0); /* Exit with code 0, meaing no error */
    } else if (((strcmp(argv[1], "-i")) == 0) || ((strcmp(argv[1], "--input")) == 0)) { /* Checks to see if user selected the option for input */
        if (argc == 2) { /* Checks to see if only two arguments were entered on the command line, meaning no argument was entered after '-i|--input' */
            fprintf(stderr, "command line error (203): No argument after '-i|--input'\n"); /* Formatted print statment from the project descripiton */
            exit(203); 
        } else if (argc > 3) { /* Checks to see if more than three arguments were entered on the command line, meaning the user may have specified more than one input file. */
            fprintf(stderr, "command line error (202): Input file already specified\n"); /* Formatted print statment from the project descripiton */
            exit(202); 
        }
    } else {
        fprintf(stderr, "command line error (201): Incorrect/unexpected argument entered\n"); /* Formatted print statment from the project descripiton */
        exit(201); 
    }
}

/* in: 8-byte key; return: 4 byte hash - 100% Completed */
uint32_t hash(uint64_t key) {
    uint32_t hash = 5381;
    unsigned char *hashKey = (unsigned char*)&key; /* Casting the uint64_t to a unsigned char to use as hash key*/
    int c; /* Is this supposed to be a char or an int */

    for (size_t i = 0; i < sizeof(uint64_t); i++) {
         c = *hashKey;
         hash = ((hash << 5) + hash) ^ c; 
         hashKey++;
    }

    return hash;
}     

/* in: pointer to hash table, key, value; return 0 for success and 1 when update occurred  */
int create(Node **hashTable, uint64_t key, int value) {
    /* Getting index to assign the values to. */
    uint32_t index = hash(key) % TOTALBUCKETS;

    /* Creating a temporary node to store contents of the parameter*/
    if (hashTable[index] == NULL) {
        Node* entryNode = Malloc(sizeof(Node)); // Mallocs the new Node that will be put in table.
        entryNode->key = key;   // Matches the entryNode's key and values to that in the parameter.
        entryNode->value = value;
        hashTable[index] = entryNode; // Assigns the NULL pointer at the index to the pointer to the entryNode.
        return 0; // Returns 0 suggesting a successful create.
    } else {
        /* Stroing the memory location of the bucket and previous bucket being looked for so we can iterate down the chain */
        Node* prev; 
        Node* entryNode = hashTable[index]; 
        /* Iterating over the bucket (chain) checking to see if the keys match */
        while (entryNode != NULL) {
            /* If keys match update will occur */
            if (entryNode->key == key) {
                return (update(hashTable, key, value) + 1); 
            }
            prev = entryNode;
            entryNode = entryNode->next;
        }
        Node* newNode = Malloc(sizeof(Node));
        newNode->key = key;
        newNode->value = value;
        prev->next = newNode;
        return 0;
    }
    return 2; // Returns if an error occurred in my code. 
}    

/* in: pointer to new hash table, pointer to current hash table, size of current hash table */
void copyHashTable(Node **new_hashTable, Node **current_hashTable, size_t BUCKETS) {
    /* Copy hashtable contents over to new_hashTable */  
    for (size_t i = 0; i < BUCKETS; i++) {
        //Node *prev;
        Node *currentNode = current_hashTable[i];
        
        while (currentNode != NULL) {
            uint32_t currentKey = currentNode->key;
            uint32_t currentIndexInNewTable = hash(currentKey) % TOTALBUCKETS;

            Node *currentNodeInNewHT = new_hashTable[currentIndexInNewTable];

            if (currentNodeInNewHT == NULL) {
                /* Malloc, copy key and value, put in table */
                Node *entryNode = Malloc(sizeof(Node));
                entryNode->key = currentNode->key;
                entryNode->value = currentNode->value;
                new_hashTable[currentIndexInNewTable] = entryNode;
                /* This was one of my ways of debugging – still used GDB way more than this */
                //fprintf(stdout, "\tKey: %ld\tValue: %d\t... was added at index: %d\n", entryNode->key, entryNode->value, currentIndexInNewTable);
            }
            while (currentNodeInNewHT != NULL) {
                if (currentNodeInNewHT->key == currentNode->key) {
                    currentNodeInNewHT->value = currentNode->value;
                } else if (currentNodeInNewHT-> next == NULL) {
                    /* Malloc, copy key and value, put in table */
                    Node *entryNode = Malloc(sizeof(Node));
                    entryNode->key = currentNode->key;
                    entryNode->value = currentNode->value;
                    currentNodeInNewHT->next = entryNode;
                    /* This was one of my ways of debugging – still used GDB way more than this */
                    //fprintf(stdout, "(CHAIN:\tKey: %ld\tValue: %d\t... was added at index: %d\n", entryNode->key, entryNode->value, currentIndexInNewTable);
                }
                currentNodeInNewHT = currentNodeInNewHT->next;
            }

            /* Tried many different things to free the hashTable – IMPOSSIBLE! */
            //prev = currentNode;
            //freeHashTable(current_hashTable); /* Thought this line was funny, makes the amount of frees go from 1,000 to like 970,000 – Going to go out on a limb and say this ain't right. */
            currentNode = currentNode->next;
            //free(prev); /* One may think that freeing the previous node may work, which it actually does help the Malloc and frees come to closer number but the code doesn't work when you do this. */
        }
    }
}

/* in: pointer to current hash table (if there is one), number of initial or new buckets; return: pointer to new hash table */
Node **createHashTable(Node **hashTable, size_t BUCKETS) {
    /* Creating a Mallocing a new Hash Table */
    Node **new_hashTable = Malloc(TOTALBUCKETS * sizeof(Node));

    for (size_t i = 0; i < TOTALBUCKETS; i++) {
            *(new_hashTable + i) = NULL;
    }

    /* Iterating over the hashTable and Mallocing each index, allocating the number of bytes that struct Node requires */
    if (hashTable != NULL) {
        copyHashTable(new_hashTable, hashTable, BUCKETS);
    } 
    
    return new_hashTable;

}     
 
/* in: pointer to hash table, key, value; return 0 for success and 1 when create occurred  */
int update(Node **hashTable, uint64_t key, int value) {
    /* Getting index to assign the values to. */
    uint32_t index = hash(key) % TOTALBUCKETS;
    /* Stroing the memory location of the bucket being looked for so we can iterate down the chain */
    Node* entryNode = hashTable[index]; 
        
    /* Iterating over the bucket (chain) checking to see if the keys match */
    while (entryNode != NULL) {
        /* If the keys match, the value will be updated */
        if (entryNode->key == key) {
            entryNode->value = value;
            return 0;
        }
        entryNode = entryNode->next;
    }
    /* Returns if matching keys were never found */
    return (create(hashTable, key, value) + 1);
}  

/* in: pointer to hash table, key, pointer to value to be updated; return 0 for success and 2 when error */
int read(Node **hashTable, uint64_t key, int *value) {
    /* Getting index to assign the values to. */
    int index = hash(key) % TOTALBUCKETS;
    /* Stroing the memory location of the bucket being looked for so we can iterate down the chain */
    Node* entryNode = hashTable[index];
        
    /* Iterating over the bucket (chain) checking to see if the keys match */ 
    while (entryNode != NULL) {
        /* If the keys match, the value will be updated */
        if (entryNode->key == key) {
            *value = entryNode->value;
            return 0;
        }
        entryNode = entryNode->next;
    }
    /* Returns if key was not retrieved. */
    return 2; 
}  

/* in: pointer to hash table, key; return 0 for success and 2 when not successful - 100% Complete */
int delete(Node **hashTable, uint64_t key) {
    /* Getting index to assign the values to. */
    uint32_t index = hash(key) % TOTALBUCKETS;
    
    /* Node previous to keep track of creating a new node if no matching key is found by going through the linked list. */
    Node* prev = NULL; 
    
    /* entryNode is equal to the node at the specific bucket - same memory location */
    Node* entryNode = hashTable[index]; 

    /* Iterating over the node at that specific index, going down the chain seeing if any of the keys match up. */ 
    while (entryNode != NULL) {
        if (entryNode->key == key) {
            if (prev == NULL) { /* Checking to see if the node were deleting is the first one in the chain */
                hashTable[index] = entryNode->next;
                free(entryNode);
                return 0;  
            } else if (prev != NULL && entryNode->next == NULL) { /* Checking to see if the node were deleting is at the end */
                prev->next = NULL;
                free(prev->next);
                return 0; 
            } else if (prev != NULL && entryNode->next != NULL) {  /* Checking to see if the node were deleting is in the middle */
                prev->next = entryNode->next;
                free(entryNode);
                return 0;
            }
        }
        /* Moving down the chain */
        prev = entryNode;
        entryNode = entryNode->next;
    }
    return 2; /* Returns if the node was never found */
}               

/* in: pointer to hash table, pointer to commands file; return: pointer to new hash table */
Node **runHashCommands(Node **hashTable, FILE *cmdFile) {
    char *buffer;
    size_t buffsize = 8192;
    char command = ' ';
    uint64_t key;
    uint32_t value;

    buffer = (char *)Malloc(buffsize * sizeof(char));

    while (getline(&buffer, &buffsize, cmdFile) > 0) {
        /* Updates the global varaibles */
        getHashTableStats(hashTable);
        
        /* Checking to see if we need to resize */
        while (LONGESTCHAINCOUNT > CHAININGTHRESHOLD || PERCENTAGEUSED > PERCENTAGETHRESHOLD) {
            // printHashTableVisual(hashTable); // Visualizing the hashTable
            fprintf(stdout, "Stats before resizing: %ld buckets, %d%% of buckets used, worst chaing length = %d\n", TOTALBUCKETS, PERCENTAGEUSED, LONGESTCHAINCOUNT);
            size_t PREVIOUSBUCKETSIZE = TOTALBUCKETS;
            TOTALBUCKETS = (size_t) ((float)TOTALBUCKETS * (1.0 + (float)(RESIZEPERCENTAGE / 100.0)) + 0.99);
            hashTable = createHashTable(hashTable, PREVIOUSBUCKETSIZE);
            getHashTableStats(hashTable);
            fprintf(stdout, "Stats after resizing: %ld buckets, %d%% of buckets used, worst chaing length = %d\n", TOTALBUCKETS, PERCENTAGEUSED, LONGESTCHAINCOUNT);
            
        }
        
        
        /* Returns the number of arguments picked up by sscanf */
        int arguments = sscanf(buffer, "%c %ld %d", &command, &key, &value);

        /* Checks the condition in the case that the user entered a command, key, and value */
        if (arguments == 3) {
            if(command == 'c') {
                int action = create(hashTable, key, value);
                 /* Checking to see if action's functionall returned a value meaning its action was unsuccessful */
                if (action == 1) {
                    fprintf(stderr, "Warning: Attempt to create the following key-value pair resulted in update (%lu, %d)\n", key, value);
                }
            } else if (command == 'u') {
                int action = update(hashTable, key, value);
                /* Checking to see if action's functionall returned a value meaning its action was unsuccessful */
                if (action == 1) {
                    fprintf(stderr, "Warning: Attempt to update the following key-value pair resulted in create (%lu, %d)\n", key, value);
                }
            }
        } else if (arguments == 2) {
            if(command == 'r') {
                int retrievedValue;
                int action  = read(hashTable, key, &retrievedValue);
                if (action == 0) {
                    fprintf(stdout, "key %ld has value %d\n", key, retrievedValue);
                } else if (action == 2) {
                    fprintf(stderr, "Warning: Unable to read the value for key %lu\n", key);
                }
            } else if (command == 'd') {
                int action = delete(hashTable, key);
                 /* Checking to see if action's functionall returned a value meaning its action was unsuccessful */
                if (action == 2) {
                    fprintf(stderr, "Warning: Unable to delete, no entry found for key %lu\n", key);
                }
            }
        } 
    }

    // printHashTableVisual(hashTable); // Where I would call visualize hashTable. 
    getHashTableStats(hashTable);
    printHashTableStats();
    free(buffer); 
    return hashTable;
}     

/* in: pointer to hash table */
void getHashTableStats(Node **hashTable) {
    /* Setting the longest chain back to zero */
    LONGESTCHAINCOUNT = 0;
    
    /* Calculating the amount of buckets used. */
    int USEDBUCKETCOUNT = 0;

    for (size_t i = 0; i < TOTALBUCKETS; i++) {
        if (hashTable[i] != NULL) {
            USEDBUCKETCOUNT++;
        }
    }

    /* Updating global variable */
    PERCENTAGEUSED = (int) (((double) USEDBUCKETCOUNT / (double) TOTALBUCKETS) * 100);

    /* Calculating the worst chain */
    Node *temp = NULL;
    int chainCount = 1, longestChain = 0;
    
    for (size_t i = 0; i < TOTALBUCKETS; i++) {
        if (hashTable[i] != NULL) {
            temp = hashTable[i];
            while (temp->next != NULL) {
                temp = temp->next;
                chainCount++;
            }
            if (chainCount > longestChain) {
                longestChain = chainCount;
            }
            chainCount = 1;
        }
    }

    /* Updating Global Variable */
    LONGESTCHAINCOUNT = longestChain;
}    

/* no parameters to pass, simply prints out some global variables */
void  printHashTableStats(){
    /* Print statement showing table stats */ 
    fprintf(stdout, "Stats at end: %ld buckets, %d%% of buckets used, worst chain length = %d\n", TOTALBUCKETS, PERCENTAGEUSED, LONGESTCHAINCOUNT);
}       

/* in: pointer to hash table */
void freeHashTable(Node **hashTable) {  
    Node *prev;
    Node *temp = NULL;
    for (size_t i = 0; i < TOTALBUCKETS; i++) {
        temp = hashTable[i];
        while (temp != NULL) {
            prev = temp;
            temp = temp->next;
            free(prev);
        }
        free(temp);
    }
    free(hashTable);
}                 

/* Function to print a visual of a hashTable because I realized hand drawing chain7 10 times in my notebook is silly */
void printHashTableVisual(Node **hashTable) {
    fprintf(stdout, "\nPrinting HashTable - Bucket Size: %ld\n", TOTALBUCKETS);
    for (size_t i = 0; i < TOTALBUCKETS; i++) {
        Node *temp = hashTable[i];
        fprintf(stdout, "\tBUCKET INDEX: %ld\t\t", i);
        while (temp != NULL){
            fprintf(stdout, "[K: %ld V: %d]-", temp->key, temp->value);
            temp = temp->next;
        }
        fprintf(stdout, "[NULL]\n");
    }
    fprintf(stdout, "\n\n");
}

/* SOS – can't free memory leaks for the life of me. */