CC = gcc
TARGETS = lcs_pthread lcs_openmp lcs_mpi
CFLAGS = -Wall -std=c99 
#-Werror 
TFLAGS = -lpthread
MFLAGS = -lm
OMPFLAGS = -fopenmp
MPIC = mpicc
all: clean $(TARGETS)

lcs_pthread:
	$(CC) $(CFLAGS) $@.c -o $@ $(TFLAGS) $(MFLAGS)
lcs_openmp:
	$(CC) $(OMPFLAGS) $(CFLAGS) $@.c -o $@ $(MFLAGS)
lcs_mpi:
	$(MPIC) $@.c -o $@ $(MFLAGS)

clean:
	rm -f $(TARGETS)