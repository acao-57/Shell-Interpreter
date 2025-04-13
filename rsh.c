#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12
#define maxArgv 21 //max of 20 arguments +1 for terminator NULL pointer

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	for (int i = 0; i < N; i++)
	{
		if(strcmp(cmd, allowed[i]) == 0)
		{
			return 1; //allowed command
		}
	}
	return 0; //not allowed
}

int main() {
    char line[256];
	pid_t pid;
	int status;
	posix_spawnattr_t attr;

    while (1) {

	//original code, do not touch (gets the input, skips empty lines and new lines)
	fprintf(stderr,"rsh>");
	if (fgets(line,256,stdin)==NULL) continue;
	if (strcmp(line,"\n")==0) continue;
	line[strlen(line)-1]='\0';

	//initialization
	char *argv[maxArgv] = {NULL};
	int argCount = 0;

	//copies line (from original code) for tokenization, separates by spaces
	char *token = strtok(line, " ");
	while (token != NULL) //&& argCount < maxArgv - 1) //counts up to 20 arguments (which would includes NULL terminator)
	{
		argv[argCount] = token;
		argCount++;
		token = strtok(NULL, " "); //continues, gets the next token
	}

    //checks for cd command
    if (strcmp(argv[0], "cd") == 0) 
    {
        if (argCount > 2) //has more than cd + one argument
        {
            printf("-rsh: cd: too many arguments\n");
        } 
        else
        {
            if(chdir(argv[1]) != 0) //checks if unsuccessful cd path change
            {
                perror("chdir error");
                exit(EXIT_FAILURE);
            }
        }
        continue; //otherwise continue
    }

    //exits
    if (strcmp(argv[0], "exit") == 0) 
    {
        return 0;
    }

    //prints out the allowed commands
    if (strcmp(argv[0], "help") == 0) 
    {
        printf("The allowed commands are:\n");
        for (int i = 0; i < N; i++) 
        {
            printf("%d: %s\n", i+1, allowed[i]); //prints the ordered allowed list of commands
        }
        continue;
    }

    //check if command is allowed
    if (!isAllowed(argv[0])) 
    {
        printf("NOT ALLOWED!\n");
        continue;
    }


	char *commandArgv[maxArgv];
	//copies all of the arguments into the array
	for (int i = 0; i < argCount; i++)
	{
		commandArgv[i] = argv[i];
	}
	commandArgv[argCount] = NULL; //last element set to null

    //posix spawning, taken from myspawn.c
	posix_spawnattr_init(&attr);

	if(posix_spawnp(&pid, argv[0], NULL, &attr, commandArgv, environ) != 0) //creates the process to execute the command
	{
		perror("spawn failed");
		exit(EXIT_FAILURE);
	}

	if(waitpid(pid, &status, 0) == -1) //waits for the  spawned process to terminate
	{
		perror("waitpid failed");
		exit(EXIT_FAILURE);
	}

	posix_spawnattr_destroy(&attr);
		
	}
    return 0;
} 
