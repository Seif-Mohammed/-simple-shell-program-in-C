#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int counter = 1;
void termination()                      //Function to submit each process termination in text file
{
    FILE *loging;
    loging = fopen("logfile.txt","a");   //Opening the file stream = fopen(filename,mode)
    fprintf(loging, "child process %d was terminated\n",counter);
    fclose(loging);                      //Closing the file
    counter++;
}
void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
int main()
{
        printf("####################### Simple shell ########################");
    while(1)
    {
        char line[1000] ;               //Array of characters to save the command line
        char *commands[1000];                //Pointer to mark each word individually
        int background=0;                    //Variable to mark the existence of (&)
        printf("\n>> ");
        gets(line);                     //Gets the command line from user
        int i =0,l=0;
        l = strlen(line);
        line[l]='\0';                   //Make the last index NULL
        //*****************  Separate the line into words by each space  ******************
        commands[i] = strtok(line," ");
        while(commands[i]){
            i++;
            commands[i] = strtok(NULL," ");       //Make the pointer mark each word
        }
        //*********************  Check bachground process  **************

        char x = *commands[i- 1];
        if (x == '&'){
            background = 1;
            commands[i-1] = NULL;                   //If it exists we raise the flag and remove it
        }
        else background = 0;
        //*************************************************************************************************************
        if(!strcmp(commands[0] ,"exit"))break;
        else if(!strcmp(commands[0],"cd"))   { //Check if the command is change directory
            int status = chdir(commands[1]);         //Call change directory function
            if (status == -1)printf("directory not found");  //Check if there is error in directory
            else continue;
        }
        else if((strcmp(commands[0], "echo")) && (strcmp(commands[0] ,"exit")) && (strcmp(commands[0] ,"mkdir")) && (strcmp(commands[0] ,"pwd")) && (strcmp(commands[0] ,"firefox"))&& (strcmp(commands[0] ,"ls")) && (strcmp(commands[0], "export"))&& (strcmp(commands[0],"cd"))){
            printf("WRONG INPUT !!");
            continue;
        }
        else if (!strcmp(commands[0], "echo")) {
            i=1;
            while(commands[i]){
                removeChar(commands[i] , '\"');     //Make the pointer mark each word
                i++;
            }
            i=1;
            while(commands[i]) {
                char DOLLAR = commands[1][0];
                if (DOLLAR == '$') {
                    char *q = &commands[1][1];
                    commands[1] = NULL;
                    char *env_value = getenv(q);
                    commands[1] = env_value;
                    if (commands[1] == NULL) {
                        printf("Environment Variable not recognized\n");
                        break;
                    }
                }
                else{
                    i++;

                }
            }
            i=1;
            while(commands[i]){
                printf("%s ", commands[i]);
                i++;
            }
        }
        else if (!strcmp(commands[0], "export")){
            removeChar(commands[1] , '\"');
            putenv(commands[1]);
        }

        else{
            if (background == 1){
                pid_t pid = fork();
                if (pid == 0){
                    execvp(commands[0],commands);       //Executing the process without waiting it to terminate
                    signal(SIGCHLD, termination);       //Submitting the command
                }
                else if (pid == -1){
                    printf("Error in forking");                 //Error in forking
                    break;
                }
            }
            else{
                if(commands [1]!= NULL){
                    char DOLLAR = commands[1][0];
                    if( DOLLAR == '$'){
                        char *q = &commands[1][1];
                        commands[1] = NULL;
                        char * env_value = getenv(q);
                        commands[1] = env_value;
                        if(commands[1] == NULL) printf("Environment Variable not recognized\n");
                        pid_t pid = fork();         //Creating a child process
                        if (pid == 0){
                            int stat = execvp(commands[0],commands);           //Executing the process
                            if(stat == -1){
                                printf("WRONG INPUT !!");
                                continue;
                            }
                        }
                        else if (pid == -1){
                            printf("Error in forking");                 //Error in forking
                            break;
                        }
                        signal(SIGCHLD, termination);               //Submitting the command
                        waitpid(pid, NULL, 0);   //Waiting process to terminate
                    }
                    else{
                        pid_t pid = fork();         //Creating a child process
                        if (pid == 0){
                            int stat = execvp(commands[0],commands);           //Executing the process
                            if(stat == -1){
                                printf("WRONG INPUT !!");
                                continue;
                            }
                        }
                        else if (pid == -1){
                            printf("Error in forking");                 //Error in forking
                            break;
                        }
                        signal(SIGCHLD, termination);               //Submitting the command
                        waitpid(pid, NULL, 0);   //Waiting process to terminate
                    }
                }
                else{
                    pid_t pid = fork();         //Creating a child process
                    if (pid == 0){
                        int stat = execvp(commands[0],commands);           //Executing the process
                        if(stat == -1){
                            printf("WRONG INPUT !!");
                            continue;
                        }
                    }
                    else if (pid == -1){
                        printf("Error in forking");                 //Error in forking
                        break;
                    }
                    signal(SIGCHLD, termination);               //Submitting the command
                    waitpid(pid, NULL, 0);   //Waiting process to terminate
                }
            }
        }
    }
    return 0;
}