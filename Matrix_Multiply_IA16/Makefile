CC      := ia16-elf-gcc
CCFLAGS := -mcmodel=medium -march=i286

SRCDIR   := .
INCL_DIR := ../00_include

TARGET = ../DOS_EXEs/MM128.EXE

SRCS := $(wildcard $(INCL_DIR)/*.c)
SRCS += $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:.c=.o)

all: clean $(OBJS)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJS) -li86

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

run: all
	@./$(TARGET)

clean:
	rm -fr *.o $(INCL_DIR)/*.o