/*
Name of file-env.c
Author- Jeton Koka
Date finished- February 10, 2017
Class- UNIX SYSTEM PROGRAMMING
Language- C
Platform developed on- Ubuntu 16.10
Compiler used- gcc

*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

extern char** environ;



//function to print out the environment when no arguments are passed
void printEnvContents();
//function to handle the -i option
void replaceEntireEnv(int argc, char* argv[]);
//function to handle the case with no -i option
void modifyEnv(int argc, char* argv[]);
//cleans up environment variable malloc allocation
void cleanup();
//counts the number of variables
int countNumOfVariables();
//checks if name of environemnt variable exists within environ array
int doesVariableExist(char *name);




int main(int argc, char *argv[])
{
	if(argc <= 1)
	{
		printEnvContents();
		printf("%d",countNumOfVariables());
	}
	else
	{
		if(strcmp(argv[1], "-i") == 0) {
			replaceEntireEnv(argc, argv);
			cleanup();
		}
		else
		{
			modifyEnv(argc,argv);
		}

	}
 	return 0;

}


void printEnvContents()
{
	for(int index = 0; environ[index] != NULL; ++index)
	{
		puts(environ[index]);
	}

}
void cleanup()
{
	for(int index =0; environ[index] != NULL; ++index)
	{
		free(environ[index]);
	}
	free(environ);
}
int countNumOfVariables()
{
	int numOfVars = 0;
	for(int index =0; environ[index] != NULL; ++index)
	{
		numOfVars++;
	}
	return numOfVars;
}
//replaces the environment variable completely with the arguements given in the call
void replaceEntireEnv(int argc, char* argv[])
{
	
	environ = malloc(sizeof(char*)*argc);
	char **listOfCommands = malloc(sizeof(char*)*argc);
	int findCommand = 0;

	for(int i=2; i < argc; ++i)
	{
		if(strchr(argv[i],'=') != NULL)
		{
			char *firstPartOfPair = strtok(argv[i],"=");
			char *followingPairPart = strtok(NULL, "=");
			environ[i-2] = malloc(sizeof(char)*(strlen(firstPartOfPair)+strlen(followingPairPart)+1));
			strcpy(environ[i-2], firstPartOfPair);
			strcat(environ[i-2], "=");
			strcat(environ[i-2],followingPairPart);

		}
		else
		{
			listOfCommands[findCommand] = malloc(sizeof(char)*strlen(argv[i]));
			strcpy(listOfCommands[findCommand++], argv[i]);
		}
	}
	if(findCommand == 0)
	{
		printEnvContents();
	}
	else
	{
		if(execvp(listOfCommands[0], listOfCommands) <0 )
			printf("%s\n", "exec failed :(");
		free(listOfCommands);
	}
	cleanup();
}
int doesVariableExist(char *name)
{
	for(int index = 0; environ[index] != NULL; ++index)
	{
		char *currentVariable= malloc(sizeof(char)*strlen(environ[index]));
		memcpy(currentVariable, environ[index], sizeof(char)*strlen(environ[index]) );
		if(strcmp(strtok(currentVariable,"="), name) == 0)
		{
			free(currentVariable);
			return index;
		}
		free(currentVariable);
		
	}
	return -1;
}

void modifyEnv(int argc, char* argv[])
{
	int findCommand= 0;
	char **listOfCommands = malloc(sizeof(char*)*argc);
	//environ = realloc(environ,2*sizeof(char *)*countNumOfVariables());
	for(int i=1; i < argc; ++i)
	{
		if(strchr(argv[i],'=') != NULL)
		{
			//printf("%s\n", "IT HAS AN EQUAL SIGN!!!!!");
			char *firstPartOfPair= malloc(sizeof(char)*strlen(environ[i]));
			memcpy(firstPartOfPair, argv[i], sizeof(char)*strlen(environ[i]));
			firstPartOfPair = strtok(firstPartOfPair,"=");
			int posToReplace = doesVariableExist(firstPartOfPair);
			if(posToReplace != -1)
			{
				environ[posToReplace] = malloc(sizeof(char)*strlen(argv[i])+1);
				memcpy(environ[posToReplace], argv[i], sizeof(char)*(strlen(argv[i])+1));
				

			}
			else
			{

				//environ[countNumOfVariables()] = malloc(sizeof(char)*strlen(argv[i])+1);
				//strcpy(environ[countNumOfVariables()], argv[i]);
				//environ[countNumOfVariables()] = argv[i];
				//environ[countNumOfVariables()+1]= NULL;
			}
			free(firstPartOfPair);

		}
		else
		{
			//findCommand++;
			listOfCommands[findCommand] = malloc(sizeof(char)*strlen(argv[i]));
			strcpy(listOfCommands[findCommand++], argv[i]);
		}

	}
	if(findCommand == 0)
	{
		printEnvContents();
	}
	else
	{
		if(execvp(listOfCommands[0], listOfCommands) <0 )
			printf("%s\n", "exec failed :(");
	}
	free(listOfCommands);

}
