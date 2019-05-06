#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <sys/time.h>
#include <math.h>

#define MAX_LINES 1000010
#define LINE_SIZE 2100

int NUM_THREADS = 24;
int nextLine = 0;

int fileLines;
int myStartLine;
int numLines;
char** entries;
//char** results;
char results[MAX_LINES][LINE_SIZE];
FILE *fp;


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
	// char* resultStr = (char*)malloc((len + 1) * sizeof(char));
	results[line1][LINE_SIZE] = '\0';

	while (lcsLengths[row][col] != 0)
	{
		results[line1][--len] = str1[row - 1];
		row--;
		col--;
	}
	freeArray(lcsLengths, len1);
	size_t length;
	if ((length = strlen(results[line1])) > 0) 
	{
		if (results[line1][length - 1] == '\n')
			results[line1][length - 1] = '\0';
	}

	//while (line1 != nextLine) ;
	// wait until it's our turn

	nextLine++;
	//if (line1 < 100) // only print first 100 lines
	//printf("%d-%d: %s\n", line1, line2, results[line1]);
	
	//free(resultStr);
}

void *lcs_threading(void *id)
{
	MPI_Status Status;
	int i, s1, s2;
	int startPos = ((int)id) * ceil((double)fileLines / NUM_THREADS);
	int endPos = startPos + ceil((double)fileLines / NUM_THREADS);
	// new
	//numLines = endPos-startPos;
	//myStartLine = startPos;
	//char **results = malloc(sizeof(char*) * (numLines));
	//for (i = 0; i < fileLines; i++)
	//{
	//	results[i] = malloc(sizeof(char) * LINE_SIZE);
	//}
	
	int num = NUM_THREADS;

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
	
	if ((int)id != 0)
	{
		int flag = 0;
		while(!flag)
		{
			MPI_Iprobe( (int)id-1, 0, MPI_COMM_WORLD, &flag, &Status );
		}
		
		MPI_Recv(&num, 1, MPI_INT, (int)id-1, 0, MPI_COMM_WORLD, &Status);
		num--;
	}
	
	for (i = startPos; (i < endPos) && (i+1<fileLines); i++)
	{
		printf("%d-%d: %s\n", i, i+1, results[i]);
	}
	
	if ((int)id < NUM_THREADS-1)
	{
		MPI_Send(&num, 1, MPI_INT, (int)id+1, 0, MPI_COMM_WORLD);
	}
}

int main(int argc, char *argv[]) {
	// start mpi
	int threadCode, num = 0;
	MPI_Status Status;
	threadCode = MPI_Init(&argc,&argv);
	if (threadCode != MPI_SUCCESS) 
	{
		printf ("Error starting MPI.\n");
        MPI_Abort(MPI_COMM_WORLD, threadCode);
    }
		
	int tasks, rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&tasks);
	
	struct timeval time1, time2;
	double elapsedTime;
	int version = 1;
	int probSize, i;
	int lineCount = 0;
	char *fileName;

	void *status;
	gettimeofday(&time1, NULL);
	// opening file

	if ((argc == 2)) {
		fileName = argv[1]; //get name from argument
	}
	else if (argc == 3) {
		fileName = argv[1];
		probSize = strtol(argv[2], NULL, 10);
	}
	else {
		printf("Usage: ./program <file> | <problem size>\n");
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
	
	entries = malloc(sizeof(char*) *fileLines);
	for (i = 0; i < fileLines; i++)
	{
		if (ferror(fp) || feof(fp))
			break;
		entries[i] = malloc(sizeof(char) * LINE_SIZE);
		fgets(entries[i], LINE_SIZE, fp);
	}
  
	NUM_THREADS = tasks;

	// running the function by thread rank
	lcs_threading(rank);

	MPI_Barrier(MPI_COMM_WORLD);
	
	if (rank == 0)
	{
		int flag = 0;
		while(!flag)
		{
			MPI_Iprobe( (int)NUM_THREADS-1, 0, MPI_COMM_WORLD, &flag, &Status );
		}
		
		MPI_Recv(&num, 1, MPI_INT, (int)NUM_THREADS-1, 0, MPI_COMM_WORLD, &Status);
		num--;
		// printing data
		gettimeofday(&time2, NULL);
		elapsedTime = (time2.tv_sec - time1.tv_sec) * 1000.0; //sec to ms
		elapsedTime += (time2.tv_usec - time1.tv_usec) / 1000.0; // us to ms
		printf("DATA, %d, %s, %f\n", version, getenv("SLURM_CPUS_ON_NODE"),  elapsedTime);
	}
	else if (rank == NUM_THREADS-1)
	{
		MPI_Send(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);	
	}
	
	freeArray((int**)entries, fileLines);
	fclose(fp);
	MPI_Finalize();
	// end mpi
	return 0;
}
