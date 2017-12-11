#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytoc.h"

#define INBUFLEN 1024

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
        char **path = mytoc(envp[i],':');
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


int main (int argc, char**argv, char**envp) {
    char **vector, delimiter = ' ';
    int pid, retVal;

    while(1) {
        char line[INBUFLEN];
        write(1, "$ ", 2);
        read(0, line, INBUFLEN);
        vector = mytoc(line, delimiter);     //Tokenize input with delimiter
    
        if (compare_str(vector[0], "exit")) {
            return 0;
        }
        /* check for change directory */
        else if (compare_str(vector[0], "cd\0")) {
            retVal = chdir(vector[1]);
            if (retVal = 0) {
                printf("Failed to change directory.");
                fflush(stdout);
            }
        } else {
            pid = fork();
            if (pid == 0) {
                retVal = execve(vector[0], vector, envp);
                if (retVal < 0) {
                    printf("Command finished with error.\n");
                    fflush(stdout);
                } else if (retVal > 0) {
                    printf("Didn't find command.\n");
                    fflush(stdout);
                } else if (pid < 0) {
                    printf("Fork failure.\n");
                    fflush(stdout);
                }
            }
        }
        
        for (int i = 0; i < sizeof(line); i++)
            line[i] = 0;
        
        free_vector(vector);
    }
  return 0;
}
