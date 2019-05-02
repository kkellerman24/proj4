CC = gcc
TARGETS = lcs_pthread
CFLAGS = -Wall -std=c99 
#-Werror 
TFLAGS = -lpthread
MFLAGS = -lm
all: clean $(TARGETS)

lcs_pthread:
	$(CC) $(CFLAGS) $@.c -o $@ $(TFLAGS) $(MFLAGS)

clean:
	rm -f $(TARGETS)