##############################################
#             Table of Contents              #
# 1: Basic Flags in the compile and linking  #
# 2: where to put and to find files?         #
# 3: Compiling and Linking                   #
##############################################


# Part 1: Basic Flags in the compile and linking
CC = gcc
MPI_INCLUDE = -I/usr/lib/x86_64-linux-gnu/openmpi/include -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi
MPI_LINK = -L/usr/lib/x86_64-linux-gnu/openmpi/lib -lmpi
INCLUDE = -Iinclude $(MPI_INCLUDE)

LDFLAGS = -march=native -fno-strict-aliasing -g3 # -fsanitize=address 
CFLAGS =  -march=native -fno-strict-aliasing -g3 -Wall # -fopt-info-all -fanalyzer -fsanitize=address



# Part 2: where to put and to find files?
# store *.o files in obj/
OBJDIR = obj

# all source files can be found in these folders
SRC = src
## extract all *.c filenames from the directories
FILENAMES  =  $(wildcard $(SRC)/*.c)

# Substitution References: replaces all *.c with *.o
# note that it will keep the directory structure
OBJECTS := $(FILENAMES:$(SRC)/%.c=$(OBJDIR)/%.o)



# Part 3: Compiling and Linking


lib:
	cd lib/LIBRARY_NAME && make clean && make all

# BUILD OBJECT FILES IN OBJECTDIR directory
$(OBJDIR)/%.o: $(SRC)/%.c
	mkdir -p '$(@D)'
	$(CC) -c $< $(INCLUDE) $(CFLAGS)  -o $@



TARGETS = mitm

# REMOVE TARGETS FROM $(OBJECTS)
TARGET_OBJECTS = $(addprefix $(OBJDIR)/,  $(addsuffix .o, $(TARGETS)) )
COMMON_OBJECTS = $(filter-out $(TARGET_OBJECTS), $(OBJECTS) )

all: $(TARGETS) lib


mitm: $(COMMON_OBJECTS) $(OBJDIR)/mitm.o
	$(CC)  $^ $(LDFLAGS) $(LDLIBS) -o $@



.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGETS)


