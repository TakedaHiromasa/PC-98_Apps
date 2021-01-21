CC      := gcc
CCFLAGS := 

SRCDIR   := .
INCL_DIR := 

TARGET = main

SRCS := $(wildcard $(INCL_DIR)/*.c)
SRCS += $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:.c=.o)

all: clean $(OBJS)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

run: all
	@./$(TARGET)

clean:
	rm -fr *.o $(INCL_DIR)/*.o