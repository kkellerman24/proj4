CC = gcc
TARGETS = lcs_openmp
CFLAGS = -Wall -std=c99 
#-Werror 
OMPFLAGS = -fopenmp
MFLAGS = -lm
all: clean $(TARGETS)

lcs_openmp:
	$(CC) $(CFLAGS) $@.c -o $@ $(OMPFLAGS) $(MFLAGS)

clean:
	rm -f $(TARGETS)
