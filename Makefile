# Name of the project
PROJ_NAME=hashashash

# .c files
C_SOURCE=$(wildcard * .c)

# .h files
H_SOURCE=$(wildcard * .h)

# .o files
OBJ=$(C_SOURCE:.c=.o)

# Compiler
CC=gcc

# Flags
CC_FLAGS= -O3 -c

################################
# Compilation
################################

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@echo "Generating binary..."
	ar -cvq hashashash.a *.o
	gcc main.c -o tests hashashash.a
	rm  *.o

%.o: %.c %.h
	$(CC) $< $(CC_FLAGS)

clean:
	rm  *.o