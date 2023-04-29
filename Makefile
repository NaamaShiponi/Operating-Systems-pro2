CC = gcc
CFLAGS = -g -Wall
ALL_FILE = cmp copy decode encode stshell
# Part A - cmp and copy tools
PART_A_A = cmp.c cmp.h
PART_A_B = copy.c copy.h
# Part B - coding libraries
LIB_A_SRC = codecA.c codecA.h
LIB_B_SRC = codecB.c codecB.h
LIBS = lib_A lib_B 
# Part C
PART_C = stshell.c

all: all_task_A all_task_B all_task_C

# Part A - cmp and copy tools
all_task_A: cmp copy

cmp: $(PART_A_A)
	$(CC) $(CFLAGS) $(PART_A_A) -o cmp

copy: $(PART_A_B)
	$(CC) $(CFLAGS) $(PART_A_B) -o copy

# Part B - coding libraries
all_task_B:	encode decode

encode:	encode.c $(LIBS)
	gcc encode.c -L. -l CodecA -l CodecB -o encode

decode:	decode.c $(LIBS)
	gcc decode.c -L. -l CodecA -l CodecB -o decode

lib_A:	$(LIB_A_SRC)
	gcc -shared $(LIB_A_SRC) -fPIC -o libCodecA.so

lib_B:	$(LIB_B_SRC)
	gcc -shared $(LIB_B_SRC) -fPIC -o libCodecB.so

# Part C
all_task_C: stshell

stshell: $(PART_C)
	$(CC) $(CFLAGS) $(PART_C) -o stshell






.PHONY: clean all

clean:
	-rm -f *.so $(ALL_FILE)