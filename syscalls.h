/*
 * hashTab Supporting Functions
 * Program 2 assignment
 * CSE109 Fall 2021
 * student ID (e.g., idk321): jwk324
 * student Last name: Keane
 * student First name: Jack
 * gitHub ID: @jackwkeane
 */

#ifndef SYSCALLS
#define SYSCALLS

void *Malloc(size_t); /* in: bytes to allocate; return: ptr of type void to allocated memory */
FILE *Fopen(const char*, const char*); /* in: path/file, mode; return: file ptr */
size_t Fread(void *, size_t, size_t, FILE *); /* in: buffer ptr, buffer size, num buffers, input file ptr; return: bytes read */
void Fclose(FILE*); /* in: file ptr */

/* UPDATED SYSCALLS */
int Pipe(int []); /* in: int array, index 0 will be read end, index 1 will be write end out: returns 0 on success, returns -1 on error.*/
//pid_t Fork(); /* in: void out: returns a new process */
size_t Fwrite(const void *, size_t, size_t, FILE *); /* in: buffer ptr, buffer size, num buffers, output file ptr; return: bytes written */

/* FUNCTIONS FROM THE LAB */
ssize_t Read(int, void *, size_t);
ssize_t Write(int, const void *, size_t);
int Close(int);







#endif