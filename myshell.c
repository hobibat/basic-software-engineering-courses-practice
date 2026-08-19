#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){

    while(1){
        printf("myshell>"); 

        //readint input
        char command[1024];
        char* arg[64];//stores pointers to each argument or token
        char d=" ";//this is the delimiter

        char arr[64]; 
        //this i guess i will need to derefrence and store arguments not their pointers


        fgets(command, sizeof(command), stdin);
        command [strcspn(command, "\n")]= '\0';
        
        arg[0]= strtok(command, d);//callig function once to place it on the string
        int i=0;
        while(strtok(NULL, d) !=NULL){
            arg[i+1]= strtok(NULL, d);
            arr[i]=* arg[i+1];
            i++;
        }
        //now, excute the command

        int id=fork();
        if(!id) execvp(* arg[0], arg);
        else{
            wait();
            continue;
        }

    }

    return 0;
}