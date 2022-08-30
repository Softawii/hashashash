# Name of the project
PROJ_NAME=hashashash

# .c files
C_SOURCE=$(wildcard src/*)

# .h files
H_SOURCE=$(wildcard include/*)

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
	ar -cvq libhashashash.a src/*.o
	$(CC) main.c -o tests libhashashash.a
	rm  src/*.o

%.o: %.c %.h
	$(CC) $< $(CC_FLAGS)

clean:
	rm  src/*.o