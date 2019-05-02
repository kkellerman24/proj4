//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int NUM_THREADS = 24;
int nextLine = 0;

pthread_mutex_t condition_next_line = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

int fileLines;
FILE *fp;
#define LINE_SIZE 2100

void freeArray(int **arr, int length)
{
	for (int i = 0; i < length; i++)
		free(arr[i]);
	free(arr);
}

void longest_common_substring(char* str1, char* str2, int len1, int len2, int line1, int line2)
{
	int **lcsLengths = malloc((len1 + 1) * sizeof(int *));;
	for (int k = 0; k < (len1 + 1); k++)
		lcsLengths[k] = (int*)malloc((len2 + 1) * sizeof(int));

	int len = 0;
	int row, col;
	for (int i = 0; i <= len1; i++)
	{
		for (int j = 0; j <= len2; j++)
		{
			if (i == 0 || j == 0)
				lcsLengths[i][j] = 0;

			else if (str1[i - 1] == str2[j - 1])
			{
				lcsLengths[i][j] = lcsLengths[i - 1][j - 1] + 1;
				if (len < lcsLengths[i][j])
				{
					len = lcsLengths[i][j];
					row = i;
					col = j;
				}
			}
			else
				lcsLengths[i][j] = 0;
		}
	}

	// no substring exists
	if (len == 0) {
		printf("%d-%d: No Common Substring\n", len1, len2);
		return;
	}
	// longest common substring allocation
	char* resultStr = (char*)malloc((len + 1) * sizeof(char));
	resultStr[len] = '\0';

	while (lcsLengths[row][col] != 0)
	{
		resultStr[--len] = str1[row - 1];
		row--;
		col--;
	}
	freeArray(lcsLengths, len1);
	size_t length;
	if ((length = strlen(resultStr)) > 0) 
	{
		if (resultStr[length - 1] == '\n')
			resultStr[length - 1] = '\0';
	}

	while (line1 != nextLine)
		pthread_yield();

	
	pthread_mutex_lock(&condition_next_line);
	nextLine++;
	printf("%d-%d: %s\n", line1, line2, resultStr);
	pthread_mutex_unlock(&condition_next_line);
	
	free(resultStr);
}

void *lcs_threading(void *id)
{
	int i, s1, s2;
	int startPos = ((int)id) * ceil((double)fileLines / NUM_THREADS);
	int endPos = startPos + ceil((double)fileLines / NUM_THREADS);

	char **entries;
	entries = malloc(sizeof(char*) *fileLines);
	for (i = 0; i < fileLines; i++)
	{
		if (ferror(fp) || feof(fp))
			break;
		entries[i] = malloc(sizeof(char) * LINE_SIZE);
		fgets(entries[i], LINE_SIZE, fp);
	}

	int l1 = startPos;
	int l2 = l1 + 1;
	for (i = startPos; (i < endPos) && (i + 1 < fileLines); i++)
	{
		s1 = strlen(entries[i]);
		s2 = strlen(entries[i + 1]);

		longest_common_substring(entries[i], entries[i + 1], s1, s2, l1, l2);
		l1 = l2;
		l2++;
	}
	freeArray((int**)entries, fileLines);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	struct timeval time1, time2;
	double elapsedTime;
	int version = 1;
	int probSize, threadCode, i;
	int lineCount = 0;
	char *fileName;

	pthread_attr_t attr;
	void *status;
	gettimeofday(&time1, NULL);
	// opening file

	if ((argc == 2)) {
		fileName = argv[1]; //get name from argument
	}
	else if (argc == 4) {
		fileName = argv[1];
		probSize = strtol(argv[2], NULL, 10);
		NUM_THREADS = strtol(argv[3], NULL, 10);
	}
	else {
		printf("Usage: ./program <file> | <problem size> <thread count>\n");
		return 1;
	}
	// opening file
	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		printf("Error opening file.");
		return 1;
	}

	// reading in file
	int lineTracker = 0;
	while (!feof(fp))
	{
		lineTracker = fgetc(fp);
		if (lineTracker == '\n')
			fileLines++;
	}

	if ((argc > 2) && (probSize < fileLines))
		fileLines = probSize;
	rewind(fp);

	// starting pthreading
	pthread_t threads[NUM_THREADS];
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < NUM_THREADS; i++) 
	{
		threadCode = pthread_create(&threads[i], &attr, lcs_threading, (void *)i);
		if (threadCode) 
		{
			printf("ERROR; return code from pthread_create() is %d\n", threadCode);
			exit(-1);
		}
	}

	pthread_attr_destroy(&attr);
	for (i = 0; i < NUM_THREADS; i++) 
	{
		threadCode = pthread_join(threads[i], &status);
		if (threadCode) 
		{
			printf("ERROR; return code from pthread_join() is %d\n", threadCode);
			exit(-1);
		}
	}
	
	fclose(fp);

	gettimeofday(&time2, NULL);

	elapsedTime = (time2.tv_sec - time1.tv_sec) * 1000.0;
	elapsedTime += (time2.tv_usec - time1.tv_usec) / 1000.0;
	printf("DATA, %d, %s, %f\n", version, getenv("SLURM_CPUS_ON_NODE"), elapsedTime);
	pthread_exit(NULL);
	free(threads);
	return 0;
}