#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.h"

#define INBUFLEN 1024

/* Modified version of forkPipeDemo provided by Dr. Freudenthal */
void forkPipe(char **vector, char **envp)
{
    
  char **temp1 = mytoc(vector[0], ' ');
  char **temp2 = mytoc(vector[1], ' ');
  int pid;
  int *pipeFds;
  pipeFds = (int *) calloc(2, sizeof(int));
  pipe(pipeFds);

  pid = fork();
  if (pid == 0) {		/* child */
    close(1);
    close(pipeFds[0]);
    dup(pipeFds[1]);
    run_exec(temp1, envp);
    exit(0);
  } else { 			/* parent */
    close(0);
    close(pipeFds[1]);
    dup(pipeFds[0]);
    close(pipeFds[0]); close(pipeFds[1]);
    run_exec(temp2, envp);
  }
}

int run_exec(char **vector, char **envp) {
    int retVal;
    int i = 0;
    
    if(vector[0][0] == '/') {
      retVal =  execve(vector[0], vector, envp);
      if(retVal == -1){
        return 1;
      }
      if(retVal > 0){
        return -1;
      }
      else 
        return 0;
    } else {
        i = path_index(envp);
        char **temp = mytoc(envp[i], '=');
        char **path = mytoc(envp[1],':');
        while(path[i] != '\0') {
            char *lastPath = concat(*path, vector[0]);
            path[0] = lastPath;
            retVal = execve(lastPath, vector, envp);
            if(retVal == -1){
                return 1;
            }
            if(retVal > 0){
                return -1;
            } else 
                return 0;
            
            i++;
        }
        if (retVal < 0)
            return 1;
    }
}


int main (int argc, char**argv, char**envp) {
    char **vector, **pipeVector, delimiter = ' ';
    int pid, retVal;
    char * PS1 = getenv("PS1");
    
    while(1) {
        if (getenv("PS1")) {
            char *PS1 = getenv("PS1");
            int length = sizeof(PS1);
            write(1, getenv("PS1"), length);
        } else {
            setenv("PS1", "$ ", 0);
        }
        char line[INBUFLEN];
        read(0, line, INBUFLEN);
        vector = mytoc(line, delimiter);     //Tokenize input with delimiter
        pipeVector = mytoc(line, '|');
    
        if (!compare_str(vector[0], "exit")) {
            /* check for change directory */
            if (compare_str(vector[0], "cd")) {
                chdir(vector[1]);
            }
            /* Check for piping */
            else if (find_length(pipeVector) == 2) {
                pid = fork();
                if (pid == 0) {
                    forkPipe(pipeVector, envp);
                    exit(0);
                } else {
                    wait(NULL);
                }
                continue;
            }
            else {
                pid = fork();
                if (pid == 0) {
                    retVal = run_exec(vector, envp);
                    if (retVal < 0) {
                        printf("Command finished with error.\n");
                        fflush(stdout);
                    } else if(retVal > 0) {
                        printf("Didn't find command.\n");
                        fflush(stdout);
                    } else {
                        exit(0);
                    }
                    
                } else if(pid < 0) {
                    printf("Fork failed.\n");
                }
            }
            for (int i = 0; i < sizeof(line); i++)
                line[i] = 0;
            free_vector(vector);
            free_vector(pipeVector);
        } else {
            return 0;
        }
    }
    
  
  return 0;
}
