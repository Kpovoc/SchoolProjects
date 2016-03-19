#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/wait.h>

# define MAX_LINE 80 /* Max Length Command */

bool ampFlag = false;

// Gets user Input, and checks it for history calls. If no
// history calls are made, it sets line[] to user input
// via *point.
int getInput(char *point, char hist[][MAX_LINE], int histCount)
{
	char numstr[MAX_LINE]; // Array to hold numercial chars
	int iter = 1;

	//clears numstr for accidental numbers	
	memset(numstr,0,sizeof(numstr));

	// Prints the term
	printf("osh> ");
	fflush(stdout);

	// Gets user input and stores in to line[] declared
	// in main through point. Replaces EOL with NULL term.
	fgets(point, MAX_LINE, stdin);
	if(point[strlen(point) - 1] == '\n')
			point[strlen(point) - 1] = '\0';

	// Checks for previous command statement.
	if(point[0]=='!')
	{
		if(histCount != 1)
		{	
			if(point[1]=='!')
			{
				// If !! clears line[] through point and
				// copies in the most recent statement.
				memset(point,0,strlen(point));
				strcpy(point,hist[0]);

				// AddToHistory();
				for(int i=9;i>=0;i--)
				{
					strcpy(hist[i],hist[i-1]);
				}
				strcpy(hist[0],point);
				histCount++;
			}
			else if (isdigit(point[1]))
			{
				// Checks if 2nd char after ! is a number
				// If so, loop through until a char is not
				// a number, storing the numbers in numstr.
				while(isdigit(point[iter]))
				{
					numstr[iter-1] = point[iter];
					iter++;
				}
				// Converts numstr to an integer stored in iter
				iter = atoi(numstr);

				// Makes sure iter is within the bounds of existing
				// history.
				if((histCount <= 10 && iter >= 1 && iter < histCount) || 
					(histCount > 10 && iter < histCount && iter >= histCount - 10))
				{
					// Sets iter to the relavent position in hist
					// Then copies that to line[] via point.
					iter = (histCount - iter -1) % 10;
					memset(point,0,strlen(point));
					strcpy(point,hist[iter]);

					// AddToHistory();
					for(int i=9;i>=0;i--)
					{
						strcpy(hist[i],hist[i-1]);
					}
					strcpy(hist[0],point);
					histCount++;
				}
				else
					printf("The number %d, is out of the range of "
						"history.\n", iter);
			}
			else
				printf("Improper use of !! or !n. The Proper usage "
					"is '!!' or '!n' where n is a number listed with "
					"the 'history' command.\n");
		}
		else 
			printf("There is currently no command in the history.\n");
	}
	// Else there is no call to a previous command, so
	// store the current command into history, if the
	// current command is not "history"
	else if(point[0]!='\0' && strcmp(point,"history")!=0)
	{
		// Pop out the oldest command at hist[9], bump
		// everything right, then store newest command
		// at hist[0]. Increment histCount.
		for(int i=9;i>=0;i--)
		{
			strcpy(hist[i],hist[i-1]);
		}
		strcpy(hist[0],point);
		histCount++;
	}
	return histCount;
}

// Parses line[] into seperate commands located by pointers
// in *args via **comms.
void parseInput(char *parse, char **comms)
{
	char *point;
	int i = 0;
	point = strtok(parse," ");

	for (i=0;point!=NULL;i++)
	{
		comms[i] = point;
		point = strtok(NULL," ");
	}
	comms[i]=NULL;
	if(comms[0]!='\0')
	{
		if(strcmp(comms[0],"exit")==0)
			exit(0);

		else if(strcmp(comms[i-1],"&")==0)
		{
			comms[i-1]=NULL;
			i-=1;
			ampFlag=true;
		}
	}
}

// Forks the program sending args to child and commanding the
// parent to wait or go.
void forkIt(char **argList, char hist[][MAX_LINE], int histCount)
{
	int pid = fork();
	if (pid < 0)
		printf("Falled to create a Child Process.\n");
	// Child Process
	else if (pid==0)
	{
		// If 'history' then print the history
		if(strcmp(argList[0],"history")==0 && argList[1]==NULL)
		{
			if(histCount >= 1)
			{
				int rev;
				rev=histCount;
				for(int j=0;j<10 && j<histCount;j++)
				{
					printf("%d) %s\n",rev,hist[j]);
					rev--;
				}
			}
			else
				printf("Nothing yet to display in history.\n");
		}
		// Else execute the Command List
		else
			execvp(argList[0],argList);
		exit(0);
	}
	// Parent Procress
	else /* pid > 0 */
	{
		if(!ampFlag)
			wait(NULL);
		ampFlag=false;
	}
}

int main()
{
	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	char line[MAX_LINE] = {0}; /* user input*/
	char history[10][MAX_LINE]; /* holds previous commands */
	int histCount=1; /* keeps track of number of previous commands */

	while(1)
	{
		histCount = getInput(line,history,histCount);
		parseInput(line,args);
		forkIt(args,history,histCount-1);
	}
}