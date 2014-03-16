CC = gcc

SRC 	= $(wildcard *.c)
OBJS 	= $(SRC:%.c=%.o)
LIB		= libskip_list.a

INCLUDE	=

LDFLAGS	= 

CFLAGS= -g -Wall -DSKIP_LIST_DEBUG

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) -r $@ $^

.c.o:
	$(CC) -c $^ $(CFLAGS) $(INCLUDE)

clean:
	rm -f $(LIB) $(OBJS)
