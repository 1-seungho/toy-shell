#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>


#define MAX_LEN_LINE    100
#define LEN_HOSTNAME    30
#define MAX_ROUTE       350

int main(void)
{
    char command[MAX_LEN_LINE];
    char cwd[MAX_LEN_LINE];
    char *args[] = {command, NULL,NULL};
    int ret, status;
    pid_t pid, cpid;
    char hostname[LEN_HOSTNAME +1];
    memset(hostname, 0x00, sizeof(hostname));
    gethostname(hostname, LEN_HOSTNAME);
    
    while (true) {
        char *s;
        int len;
        getcwd(cwd,sizeof(cwd));
        
        printf("%c[36mMyShell@",27);
        printf("%c[36m%s@%s@", 27, getpwuid(getuid())->pw_name, hostname);
        printf("%c[37m%s $", 27, cwd);
        
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
       
        if(!strcmp(command, "exit")){
            printf("Program Exit \n");
            exit(0);
        }
        
        printf("Command: [%s]\n", command);
        printf("Len: %d\n", len);
      
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            if (!strcmp(command, "ls")){
                strcpy(command, "/bin/ls");
            }
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
