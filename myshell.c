#include <signal.h>
#include<sys/types.h>
#include<stdbool.h>
#include <sys/wait.h>
#include<unistd.h>
#include<wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pwd.h>
#define MAX_LEN 255
void *prompt(char buffer[])
{
   void *information;
   printf("#");
   information = fgets(buffer,MAX_LEN, stdin);
   if(buffer[strlen(buffer)-1] == '\n')
      buffer[strlen(buffer)-1] = 0;
   return information;
}
int main() // int argc, void *argv[]
{
   char buffer[MAX_LEN];
   char *arg;
   char *getdir;
   char *dir;
   char *to;
   pid_t pid;
   int status;
   int parent_pid;
   parent_pid = getpid();
   uid_t uid = getuid();
   struct passwd* pwd = getpwuid(uid);
   while(prompt(buffer))
   {
    if((pid = fork())<0){ // if fork is fail 
        perror("fork error");
        _exit(1);
      }
      if(strcmp(buffer,"exit") == 0 || strcmp(buffer,"quit") == 0)
      {
            if(getpid() != parent_pid) 
            {
               kill(-1*parent_pid,SIGINT);
               kill(parent_pid,SIGINT);
               // kill(getppid(),SIGINT);
               _exit(0);       
            }
            else
            {
               kill(-1*parent_pid,SIGINT);
               kill(parent_pid,SIGINT);
               _exit(0);                      
            }
      }
      else if(pid == 0)
      {
         strtok(buffer," ");
         arg = strtok(NULL," ");
         if(arg == NULL){ // 만약에 명령어가 옵션이 없다면 if there is no option in command 
             if(strcmp(buffer,"exit") == 0 || strcmp(buffer,"quit") == 0)
             {
                kill(0,SIGINT);
                exit(0);
             }
             else if(strcmp(buffer,"cd") == 0)
             {
                  chdir(pwd->pw_dir);
                  printf("current directory: %s\n",getcwd(buffer,sizeof(buffer)));
                  continue;
             }
             else if(execlp(buffer,buffer,(char*)0) == -1)
                  printf("No such a commands: %s\n", buffer);
             else 
                 execlp(buffer,buffer,(char*)0);
        }
         else // 명령어에 옵션이 존재 if option exist with command  
         {    
           if(strcmp(buffer,"cd") == 0)
           {   
              if(strcmp(arg,"..") == 0)
              {
                  chdir("..");
                  printf("current directory: %s\n",getcwd(buffer,sizeof(buffer)));
              }
              else
              {
                  to = malloc(strlen(arg)+1);
                  strcpy(to,arg);
                  getdir = getcwd(buffer,sizeof(buffer));
                  dir = strcat(getdir,"/");
                  to = strcat(dir,to);
                  chdir(to);
                  if(chdir(to) == -1)// if fail
                     printf("it's not right directory\n");
                  else
                     printf("current directory: %s\n",to);
              }
              continue;
           }
           if(execlp(buffer,buffer,arg,(char*)0) == -1)
           {
              printf("No such a commands: %s\n", buffer);
              exit(1);
           }
           execlp(buffer,buffer,arg,(char*)0);
           _exit(0);
      }
        exit(0);        
      }
      else if(pid > 0) // if parent process
      {
         waitpid(pid, &status,WUNTRACED);
         printf("(%d)return value: %d\n",parent_pid,status);
      }
   }
   _exit(0);
}

