# Project Grade: 100.0/100.0
Completed fall of 2021 as a part of Lehigh University's CSE 109: System Software.

## Note:
All programs were written and optimized for X86-64 machines with gcc module 7.10.

# hashtab-jackwkeane
hashtab-jackwkeane created by GitHub Classroom

Prog2 - hashTab - Description								CSE109 Fall 2021

Learning Objectives for Student
Hash functions and hash tables
Structures and accessing structure members
Pointers and arrays of pointers

Description of Program and Program Requirements
Write a program whose executable name will be hashTab and which conforms to the following “specification” (which is actually the help message).

hashTab creates, retrieves, updates, and deletes key-value pairs in a hash table.
Keys can be any integer in the range [0, 18446744073709551615] and values can be
any integer in the range [-2147483648, 2147483647]. Commands to perform the above
actions may be specified in the input text file as follows.

# format is: action key [value] [# comment]
# action is one of the following characters: c r u d
c key value  # if key already in table, value will be updated and a warning printed
r key        # if key not in the table, a warning will be printed, otherwise value
u key value  # if key not in the table, value will be stored and a warning printed
d key        # if key not in the table, a warning will be printed

The table auto-resizes to maintain "optimum" performance. Run program as follows.

hashTab -i|--input <input file>
        or
hashTab [-h|--help]

You are given hashTab.c, global_defs.h, and hashTab_funcs.h. You will need to create and manage the following files: hashTab_funcs.c, syscalls.h, and syscalls.c.  Your Stevens-style wrapped functions are to be placed in syscalls. 

You need to create a makefile to manage the selective compilation of your relocatable and executable objects files. You may choose to pass -DDEBUG as a parameter to gcc to print additional information during debug. This is fine, so long as it is obvious this is NOT your primary method for debug… which should be gdb. When we compile for grading, we will not compile with -DDEBUG, and it is fine if your submitted code has statements like “#ifdef DEBUG …” or “if (DEBUG) { … }”.

Exit with the following return code and an appropriate message to stderr under the following conditions. Your Stevens-style wrapped system calls should exit with the errno value and the strerror(errno) message (still to stderr). Except for syscall errors and the conditions specified below, the return code is 0.
201 - Incorrect/unexpected argument enteredkj
202 - Input file already specified
203 - No argument after '-i|--input'

It is recommended you create a directory structure of cse109.fall2021/prog2 under your home directory, and do your development work for this assignment in the prog2 directory. Make sure you change the access privileges so no one else can see the directory’s contents.
	
During development, you may find it beneficial to work with a smaller INITIALBUCKETS value. To do so, simply change the “#define INITIALBUCKETS 100” to a smaller value and recompile. Same goes for CHAININGTHRESHOLD, PERCENTAGETHRESHOLD, and RESIZEPERCENTAGE.

Use the hashTab.c and hashTab_funcs.h files, without modification, as your top-level/main file and program-specific-function header file, respectively. You can copy the ~merle/109/prog2/hashTab_init2buckets and ~merle/109/prog2/hashTab_init100buckets executables to your prog2 directory to compare their output and return code against your program’s output and return code. These files were created by setting INITIALBUCKETS value in global_defs.h to 2 and 100, respectively, compiling, and then renaming the hashTab executable appropriately.

