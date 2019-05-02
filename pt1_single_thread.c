//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 24

#define NUM_LINES 3
#define LINE_SIZE 200

//pthread_mutex_t mutexLock;

char results[NUM_LINES][LINE_SIZE];

void longest_common_substring(int line, char* str1, char* str2, int len1, int len2)
{
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

	while (lcsLengths[row][col] != 0)
	{
		results[line][--len] = str1[row - 1];
		row--;
		col--;
	}
}

void print_results(int lineCount)
{
	for (int i = 0; i < lineCount-1; i++)
	{
		printf("%d-%d: %s", i, i + 1, results[i]);
		printf("%c", '\n');
	}
}

main() {
	FILE *fp;
	int lineCount = 0;
	errno_t err;
	// opening file
	if ((err = fopen_s(&fp, "text.txt", "r")) != 0)
	{
		printf("Error");
		return;
	}

	char entries[NUM_LINES][LINE_SIZE];

	// reading in file
	while (fgets(entries[lineCount], LINE_SIZE, fp) != NULL && lineCount < NUM_LINES)
	{
		entries[lineCount][strlen(entries[lineCount]) - 1] = '\0';
		lineCount++;	
	}
	// comparing strings
	int i = 1;
	while (i < lineCount)
	{		
		int len1 = strlen(entries[i - 1]);
		int len2 = strlen(entries[i]);
		longest_common_substring(i - 1, entries[i - 1], entries[i], strlen(entries[i - 1]), strlen(entries[i]));
		i++;
	}
	// printing results
	print_results(lineCount);
	fclose(fp);
}