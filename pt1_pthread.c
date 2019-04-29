#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 24

#define NUM_LINES 1000001
#define LINE_SIZE 2001

pthread_mutex_t mutexLock;
char entries[NUM_LINES][LINE_SIZE];
char results[NUM_LINES][LINE_SIZE];

void read_file()
{
	char entries[NUM_LINES][LINE_SIZE];
	FILE *fp;
	int i = 0;
	errno_t err;

	if ((err = fopen_s(&fp, "C:\\users\\mccochas.USERS\\Documents\\text.txt", "r")) != 0)
	{
		printf("Error");
		return;
	}

	while (fgets(entries[i], LINE_SIZE, fp) != NULL)
	{
		i++;
	}
	fclose(fp);
}

void *longest_common_substring(void *myID)
{
	int startPos = ((int)myID) * (NUM_LINES / NUM_THREADS);
	int endPos = startPos + (NUM_LINES / NUM_THREADS);
	printf("myID = %d startPos = %d endPos = %d \n", (int)myID, startPos, endPos);
	for (startPos; startPos++; startPos < endPos)
	{
		char *str1 = entries[startPos - 1];
		char *str2 = entries[startPos];
		int len1 = strlen(entries[startPos - 1]);
		int len2 = strlen(entries[startPos]);

		int lcsLengths[LINE_SIZE + 1][LINE_SIZE + 1];
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

		int totLen = len;
		char* commonSub = (char*)malloc((len + 1) * sizeof(char));

		while (lcsLengths[row][col] != 0)
		{
			commonSub[--len] = str1[row - 1];
			row--;
			col--;
		}
		int k = 0;
		pthread_mutex_lock(&mutexLock);
		for (k = 0; k < totLen; k++)
			results[startPos][k] = commonSub[k];
		pthread_mutex_unlock(&mutexLock);
		free(commonSub);
		pthread_exit(NULL);
	}
}

void print_results()
{
	for (int i = 0; i < NUM_LINES; i++)
		printf("%d-%d: %s\n", i, i + 1, results[i]);
}

main() {
	// pthread stuff
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	int i = 0;

	// initialize and set thread detached attribute
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&mutexLock, NULL);

	//read_file();
	FILE *fp;
	int lineCount = 0;
	errno_t err;

	if ((err = fopen_s(&fp, "C:\\users\\mccochas.USERS\\Documents\\text.txt", "r")) != 0)
	{
		printf("Error");
		return;
	}

	while (fgets(entries[lineCount], LINE_SIZE, fp) != NULL)
	{
		entries[lineCount][strlen(entries[lineCount]) - 1] = '\0';
		lineCount++;
	}
	fclose(fp);
	// end read file

	// start pthread operation
	for (i = 0; i < NUM_THREADS; i++) {
		rc = pthread_create(&threads[i], &attr, longest_common_substring, (void *)i);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for (i = 0; i<NUM_THREADS; i++) {
		rc = pthread_join(threads[i], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	// end pthread operation

	print_results();
	pthread_mutex_destroy(&mutexLock);
	printf("Main: program complted. Exiting.\n");
	pthread_exit(NULL);
}