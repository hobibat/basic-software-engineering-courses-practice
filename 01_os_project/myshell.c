#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>// For open() and the flags: O_RDONLY, O_WRONLY, O_CREAT, etc.
#include<unistd.h>// For dup(), dup2(), close(), execvp(), STDIN_FILENO, STDOUT_FILENO
#include<sys/wait.h>

int main(){

    while(1){
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd))!=NULL){
            printf("%s$ habishell:> ", cwd);
        }
        else{
            //if getcwd failed for any probable reasons
            perror("habishell:>");
        }
        fflush(stdout);

        

        //readint input
        char command[1024];
        char* arg[64];//stores pointers to each argument or token
        char* clean_arg[64];


        if(fgets(command, sizeof(command), stdin)==NULL){
            printf("\n"); 
            break;
        }
        command [strcspn(command, "\n")]= '\0';

        char* portion= strtok(command, " ");//callig function once to place it on the string
        int i=0;
        while(portion !=NULL && i<63){
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


        //______________finding special redirecting characters______________

        char* redirections[64];//array to store places of the special characters
        int j=0;//index for that array
        int c=0;//index for clean args
        int syntax_error =0;
        for(int i=0; arg[i] !=NULL; i++){
            if(strcmp(arg[i], ">")==0 || strcmp(arg[i], ">>")==0 || strcmp(arg[i], "<")==0){
                if(arg[i+1]==NULL){
                    //user entered empty redirecting character
                    fprintf(stderr, "habishell:> syntax error near unexpected token 'newline'\n");
                    syntax_error=1;
                    break;//that's insane, i forgot i was already in a loop
                }
                if(j<62){
                    redirections[j]=arg[i];
                    redirections[j+1]=arg[i+1]; 
                    j+=2;
                }
                i++;//skip file name so not added to clean args
            }
            else{
                if(c<63){
                    clean_arg[c]=arg[i];
                    c++;
                }
            }
        }
        redirections[j]=NULL;//handling the last char
        clean_arg[c]=NULL;
        if (syntax_error || clean_arg[0] == NULL) {
            continue;
        }
        
        //ok now, i guess to start we have to prepare the files to read or write
        //loop incase we have more than one char


        //if the command is cd, then we do it and continue before fork
        if(strcmp(clean_arg[0], "cd")==0){
            if(clean_arg[1]==NULL){// no path passed means go to home directory
                //
                char* home=getenv("HOME");//form enviromen variables, return a pointer to the string holding path of home
                if(home!=NULL){
                    chdir(home);
                }
                
            }
            else{
                if(chdir(clean_arg[1])!=0){
                    perror("habishell:> ");
                }
            }
            continue;
        }

        //now, excute the command
        int id=fork();
        if(id==0) {
            j=0;//i want to use only one  global variable for indexes other than i 
            while(redirections[j]!=NULL ){
                if(strcmp(redirections[j], ">")==0){
                    //writing
                    int fd=open(redirections[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fd==-1){
                        perror("habishell:> ");
                        exit(1);
                    }
                    else{
                        int new_fd=dup2(fd, 1);
                        //this new fd points also to the same opened file, so we have now 3 pointing to opened file as well as the newfd
                        if(new_fd==-1){
                            perror("habishell:> ");//can check later what to write here
                            exit(1);
                        }
                    }
                    close(fd);
                }
                else if(strcmp(redirections[j], ">>")==0){
                    int fd=open(redirections[j+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if(fd==-1){
                        perror("habishell:> ");
                        exit(1);
                    }
                    else{
                        int new_fd=dup2(fd, 1);
                        //this new fd points also to the same opened file, so we have now 3 pointing to opened file as well as the newfd
                        if(new_fd==-1){
                            perror("habishell:> ");//can check later what to write here
                            exit(1);
                        }
                    }
                    close(fd);
                }
                else if(strcmp(redirections[j], "<")==0){
                    int fd=open(redirections[j+1], O_RDONLY);
                    if(fd==-1){
                        perror("habishell:> ");
                        exit(1);
                    }
                    else{
                        int new_fd=dup2(fd, 0);
                        //this new fd points also to the same opened file, so we have now 3 pointing to opened file as well as the newfd
                        if(new_fd==-1){
                            perror("habishell:> ");//can check later what to write here
                            exit(1);
                        }
                    }
                    close(fd);
                }
                j+=2;
            }
            execvp(clean_arg[0], clean_arg);
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