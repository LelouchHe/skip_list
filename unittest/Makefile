CC = gcc
AR = ar

SRC 	= skip_list_test.c
OBJS 	= $(SRC:%.c=%.o)
EXE		= test

INCLUDE	= -I..

LDFLAGS	= -L.. -lskip_list

CFLAGS	= -g -Wall -DSKIP_LIST_DEBUG

all: $(EXE)


$(EXE): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.c.o:
	$(CC) -c $^ $(CFLAGS) $(INCLUDE)

clean:
	rm -f $(EXE) $(OBJS)
