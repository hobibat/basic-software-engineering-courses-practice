#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){

    while(1){
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd))!=NULL){
            printf("%s$ habishell:> ", cwd);
        }else{
            //if getcwd failed for any probable reasons
            perror("habishell:>");
        }
        fflush(stdout);

        

        //readint input
        char command[1024];
        char* arg[64];//stores pointers to each argument or token


        if(fgets(command, sizeof(command), stdin)==NULL){
            printf("\n"); 
            break;
        }
        command [strcspn(command, "\n")]= '\0';

        char* portion= strtok(command, " ");//callig function once to place it on the string
        int i=0;
        while(portion !=NULL){
            arg[i]= portion;
            portion=strtok(NULL, " ");
            i++;
        }arg[i]=NULL;//end of arguments indicator

        //empty command handling
        if(arg[0]==NULL) continue;

        //exit command handling
        if(strcmp(arg[0], "exit")==0) break;

        /*//for debugging:
        for(int i=0; arg[i] !=NULL; i++){
                printf("token no.%d : %s", i+1, arg[i]);
                printf("\n");
        }*/


        //if the command is cd, then we do it and continue before fork
        if(strcmp(arg[0], "cd")==0){
            if(arg[1]==NULL){// no path passed means go to home directory
                //
                char* home=getenv("HOME");//form enviromen variables, return a pointer to the string holding path of home
                if(home!=NULL){
                    chdir(home);
                }
                
            }
            else{
                if(chdir(arg[1])!=0){
                    perror("habishell:> ");
                }
            }
            continue;
        }

        //now, excute the command
        int id=fork();
        if(id==0) {
            execvp(arg[0], arg);
            //error handling snnipet
            perror("habishell:> ");
            exit(1);
        }
        else if(id==-1){
            perror("fork failed!");
            exit(1);
        }
        else{
            wait(NULL);
            continue;
        }



    }

    return 0;
}