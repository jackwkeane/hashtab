CC=gcc
CFLAGS=-g -c -Wall -Wextra -Wwrite-strings
LFLAGS=-g

hashTab: syscalls.o hashTab_funcs.o hashTab.o
	${CC} ${LFLAGS} -o hashTab hashTab_funcs.o hashTab.o syscalls.o
syscalls.o: syscalls.c syscalls.h
	${CC} ${CFLAGS} syscalls.c
hashTab_funcs.o: hashTab_funcs.c hashTab_funcs.h global_defs.h
	${CC} ${CFLAGS} hashTab_funcs.c
hashTab.o: hashTab_funcs.h global_defs.h syscalls.h hashTab.c
	${CC} ${CFLAGS} hashTab.c

clean:
	rm -f *.o
	rm -f hashTab
