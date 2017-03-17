/*
Name of file-du.c
Author- Jeton Koka
Date finished- February 27 2017
Class- UNIX SYSTEM PROGRAMMING
Language- C
Platform developed on- Ubuntu 16.10
Compiler used- gcc

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//function to go through directory and list all files
//no return value is necessary
//argument is a c string that will be used to open directory, if possible 
long listAllFilesInDir(char* pathToCheck);


int main(int argc, char *argv[])
{
	//if no path is provided, use the current directory!
	if(argc <= 1)
		listAllFilesInDir(".");
	else
		listAllFilesInDir(argv[1]);

	return 0;
}


long listAllFilesInDir(char * pathToCheck)
{
	DIR *openedDir= NULL;
    struct dirent *directoryPointer = NULL;
    struct stat statBuffer = {0};
    //open directory
    if(chdir(pathToCheck) == -1)
    	perror("Could not change working directory");
    openedDir = opendir(pathToCheck);
    int totalSize = 0;
    printf("%s\n", pathToCheck );
    if(openedDir != NULL)
    {
    	//while we read something within our directory
    	while((directoryPointer = readdir(openedDir)) != NULL)
    	{
    		if(lstat(directoryPointer->d_name, &statBuffer) == 0)
    		{
    			//ignore parent directory or current directory. Not doing so results in infinite recursion/seg fault
    			if(strcmp(directoryPointer->d_name, "..") == 0 || strcmp(directoryPointer->d_name, ".") == 0)
    				continue;
    			else if(S_ISDIR(statBuffer.st_mode))
    			{
    				//malloc new string. We add three becuase of the two null terminators and the / character
    				char *newDir = (char*) malloc((strlen(directoryPointer->d_name)+strlen(pathToCheck)+3)*sizeof(char));
    				if(newDir != NULL)
    				{
    					//copy everything but null terminating character
    					strncpy(newDir, pathToCheck, strlen(pathToCheck));
    					strncat(newDir, "/", 1);
    					//copy including null temrianting character
    					strcat(newDir, directoryPointer->d_name);
    					long getDirSize = listAllFilesInDir(newDir) + statBuffer.st_size;
    					printf("%ld%s%s\n",getDirSize, "\t", directoryPointer->d_name );
    					totalSize += statBuffer.st_size;
    					//removes our allocated string
    					free(newDir);
    					chdir("..");
    				}
    				else
    				{
    					printf("%s\n","malloc failed!" );
    				}
    				
    			}
    			else
    				printf("%ld%s%s\n",statBuffer.st_size, "\t", directoryPointer->d_name );

    		}
    		else
    		{
    			printf("%s\n", directoryPointer->d_name);
    			perror("Could not get information about file!");
    		}
    	}
    	
    }
    else
    {
    	printf("%s\n",pathToCheck );
    	perror("The directory you inputted is not valid. Please check your path name.");
    }
    //close file
	closedir(openedDir);
	return totalSize;


}