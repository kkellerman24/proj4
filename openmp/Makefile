CC = gcc
TARGETS = lcs_openmp
CFLAGS = -Wall -std=c99 
#-Werror 
TFLAGS = -lopenmp
MFLAGS = -lm
all: clean $(TARGETS)

lcs_openmp:
	$(CC) $(CFLAGS) $@.c -o $@ $(TFLAGS) $(MFLAGS)

clean:
	rm -f $(TARGETS)