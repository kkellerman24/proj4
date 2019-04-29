//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 24

#define NUM_LINES 1000001
#define LINE_SIZE 2001

//pthread_mutex_t mutexLock;

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

void longest_common_substring(int line, char* str1, char* str2, int len1, int len2)
{
	int lcsLengths[LINE_SIZE+1][LINE_SIZE+1];
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
	printf("%d-%d: ", line, line+1);
	int k = 0;
	for (k = 0; k < totLen; k++)
		printf("%c", commonSub[k]);
	printf("%c", '\n');
	free(commonSub);
}

main() {
	char entries[NUM_LINES][LINE_SIZE];
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


	int i = 1;
	while (i < lineCount)
	{
		int len1 = strlen(entries[i - 1]);
		int len2 = strlen(entries[i]);
		longest_common_substring(i, entries[i - 1], entries[i], strlen(entries[i-1]), strlen(entries[i]));
		i++;
	}
}