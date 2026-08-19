#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){

    while(1){
        printf("habishell:> "); 

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

        /*//for debugging:
        for(int i=0; arg[i] !=NULL; i++){
                printf("token no.%d : %s", i+1, arg[i]);
                printf("\n");
        }*/


        //now, excute the command
        int id=fork();
        if(id==0) {
            execvp(arg[0], arg);
            //error handling snnipet
            perror("habishell");
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