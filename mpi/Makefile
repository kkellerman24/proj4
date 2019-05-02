CC = gcc
TARGETS = lcs_mpi
#-Werror 
MFLAGS = -lm
MPIC = mpicc
all: clean $(TARGETS)

lcs_mpi:
	$(MPIC) $@.c -o $@ $(MFLAGS)

clean:
	rm -f $(TARGETS)