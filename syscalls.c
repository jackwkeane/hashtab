/*
 * hashTab Supporting Functions
 * Program 2 assignment
 * CSE109 Fall 2021
 * student ID (e.g., idk321): jwk324
 * student Last name: Keane
 * student First name: Jack
 * gitHub ID: @jackwkeane
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> /* for fork, pipe */

/* Defining the debug macro from the make file */
#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

/* Malloc Function in the form of Steven-styled wrap */
/* in: bytes to allocate; return: ptr of type void to allocated memory */
void *Malloc(size_t numBytes) {                                             
    void *ptr = NULL;                                                 
    if ((ptr = malloc(numBytes)) == NULL) {                                
        fprintf(stderr, "Could not allocate space - %s", strerror(errno));   
        exit(errno);                                                            
    }
    return ptr; /* Functions of type void * call for a pointer to be returned */
}

/* Fopen Function in the form of Steven-styled wrap */
/* in: path/file, mode; return: file ptr */
FILE *Fopen(const char *path, const char *mode) {
    FILE *fp = NULL;
    if((fp = fopen(path, mode)) == NULL){
        fprintf(stderr, "fopen error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
        exit(errno); 
    }
    return fp;
}

/* Fread Function in the form of Steven-styled wrap */
/* in: buffer ptr, buffer size, num buffers, input file ptr; return: bytes read */
size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t num_items = 0;
    if((num_items = fread(ptr, size, nmemb, stream)) != nmemb)  { /* If statement to check to see if the return value differs from the nmemb parameter, therefore causing an error or meaning the function has reached the end of file. */
        if(!feof(stream) && ferror(stream)) { /* If statement that if true means that fread returned an error */
            fprintf(stderr, "fread error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
            exit(errno);
        }
    }
    return num_items; /* Returns the totla number of elements successfully read */
}        

/* Fclose Function in the form of Steven-styled wrap */
/* in: file ptr */
void Fclose(FILE *stream) {
    if(fclose(stream)) { /* Checks to see if the stream returns EOF */ 
        fprintf(stderr, "fclose error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
        exit(errno);
    }
}   

/* Fwrite Function in the form of Steven-styled wrap */
/* in: buffer ptr, buffer size, num buffers, output file ptr; return: bytes written */
size_t Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t num_items;
    if((num_items = fwrite(ptr, size, nmemb, stream)) != nmemb)  { /* If statement to check to see if the return value differs from the nmemb parameter, therefore causing an error */
        fprintf(stderr, "fwrite error (%d): %s\n", errno, strerror(errno)); /* Formatted print taken from program directions */
        exit(errno);
    }
    return num_items; /* Returns the totla number of elements successfully returned as size_t object */
}  

/* in: int array, index 0 will be read end, index 1 will be write end out: returns 0 on success, returns -1 on error.*/
int Pipe(int filedes[]) {
    if (pipe(filedes) == -1) {       
        fprintf(stderr, "pipe error (%d): %s\n", errno, strerror(errno));
        exit(errno);
    } else {
        return pipe(filedes);
    }
}   

/* in: void out: returns a new process */
// pid_t Fork() {
//     pid_t pid;
//     if ((pid = fork()) == -1) {  /* create process */
//         fprintf(stderr, "fork error (%d): %s\n", errno, strerror(errno));
//         exit(errno);
//     } else {
//         return pid;
//     }
// } 

/* FUNCTIONS FROM THE LAB */
ssize_t Read(int fildes, void *buf, size_t nbyte) {
    ssize_t numBytes;

    if ((numBytes = read(fildes, buf, nbyte)) == 0) { /* read in a string from the pipe */
      fprintf(stderr, "PARENT: read error (%d): %s\n", errno, strerror(errno));
      exit(errno);
    } else {
        return numBytes; /* If no error, normal return. */
    }
}

ssize_t Write(int fildes, const void *buf, size_t nbyte) {
    ssize_t numBytes; 

    if ((numBytes = write(fildes, buf, nbyte)) == -1) { /* send "string" down the pipe */
        fprintf(stderr, "CHILD: write error (%d): %s\n", errno, strerror(errno));
        exit(errno);
    } else {
        return numBytes; /* If no error, normal return. */
    }
}

int Close(int fildes) {
    if (close(fildes) == -1) {  /* close unneeded "write" end of pipe */
      fprintf(stderr, "PARENT: close error (%d): %s\n", errno, strerror(errno));
      exit(errno);
    } else {
        return close(fildes); /* If no error, normal return. */
    }
}



