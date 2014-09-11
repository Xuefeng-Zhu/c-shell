/**
 * @file shell.c 
 * Author: Xuefeng Zhu
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "log.h"


log_t Log;

void extCmd(char *cmd)
{
	log_push(&Log, cmd);

	char * pch = strtok(cmd, " ");
	if (strcmp(pch, "cd") == 0)
	{
	    pid_t pid = getpid();
		printf("Command executed by pid=%d\n", pid);
		pch = strtok(NULL, " ");
		if (pch != NULL && chdir(pch) != 0)
		{
			printf("%s: No such file or directory\n", cmd + 3);
		}
		return;
	}


	int child_status;
	if (fork())
	{
		wait(&child_status);
	}
	else
	{
		int i = 0;
		char *array[100];
		/*char * pch = strtok(cmd, " ");*/
		while (pch != NULL)
		{
			array[i] = pch;
			pch = strtok(NULL, " ");
			i++;
		}
		array[i] = NULL;
		char *temp = array[0];

	    pid_t pid = getpid();
		printf("Command executed by pid=%d\n", pid);

		if (execvp(temp, array) == -1)
		{
			printf("%s: not found\n",temp);	
		}

		exit(0);
	}
}

/**
 * Starting point for shell.
 */
int main() {
	char *cwd = NULL;
	char *cwd_buffer = NULL;
	size_t cwd_buffer_size = 0;
	ssize_t bytes_read;

	log_init(&Log);

	while (1)
	{
		char *cmd_buffer = NULL;
		size_t cmd_buffer_size = 0;

		pid_t pid = getpid();
		cwd = getcwd(cwd_buffer, cwd_buffer_size);
		printf("(pid=%d)%s$ ", pid, cwd);
		bytes_read = getline(&cmd_buffer, &cmd_buffer_size, stdin);	
		cmd_buffer[bytes_read-1] = '\0';
		free(cwd);
		cwd = NULL;

		if(strlen(cmd_buffer) == 0)
		{
			free(cmd_buffer);
			continue;
		}

		if (strcmp(cmd_buffer, "!#") == 0)
		{
			pid_t pid = getpid();
			printf("Command executed by pid=%d\n", pid);
			log_t *temp = &Log;

			if (temp->item == NULL)
			{
				free(cmd_buffer);
				continue;	
			}

			if (temp->next == NULL)
			{
				printf("%s\n", temp->item);	
				free(cmd_buffer);
				continue;
			}

			do
			{
				printf("%s\n", temp->item);	
				temp = temp->next;
			}
			while(temp != &Log);

			free(cmd_buffer);
			continue;
		}


		if (strstr(cmd_buffer, "!") == cmd_buffer)
		{
			pid_t pid = getpid();
			printf("Command executed by pid=%d\n", pid);
			char * cmd = log_search(&Log, cmd_buffer+1);
			if (cmd == NULL)
			{
				printf("No Match\n");	
			}
			else
			{	
				printf("%s matches %s\n", cmd_buffer+1, cmd);
				extCmd(cmd);
			}
			free(cmd_buffer);
			continue;
		}

		if (strcmp(cmd_buffer, "exit") == 0)
		{
			pid_t pid = getpid();
			printf("Command executed by pid=%d\n", pid);
			log_destroy(&Log);
			free(cmd_buffer);
			exit(0);
		}

		extCmd(cmd_buffer);
		free(cmd_buffer);
	}
}
