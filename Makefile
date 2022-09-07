# Name of the project
PROJ_NAME=hashashash

# .c files
C_SOURCE=$(wildcard src/*.c)

# .o files
OBJ:=$(C_SOURCE:.c=.o)

# Compiler
CC=gcc

# Flags
CC_FLAGS= -Wall -O3

################################
# Compilation
################################

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ) lib compile clean

lib:
	@echo "\nGenerating library..."
	ar -cvq libhashashash.a $(OBJ)

compile:
	@echo "\nCompiling..."
	$(CC) main.c -o tests libhashashash.a -DHASH_DEBUG

clean:
	@echo "\nRemoving libs and objects..."
	rm libhashashash.a
	rm  src/*.o

$(OBJ): %.o: %.c
	@echo "\nCompiling $(<F)..."
	$(CC) $(CC_FLAGS) -c $< -o $@

debug: CC_FLAGS += -DHASH_DEBUG
debug: all