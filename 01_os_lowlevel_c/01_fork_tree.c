#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // A variable in the parent process
    int shared_val = 100;

    printf("--- Before Fork --- (Master Process PID: %d)\n", getpid());

    // Create a child process using fork()
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    } 
    else if (pid == 0) {
        // --- THIS IS THE CHILD PROCESS ---
        printf("[CHILD] I am the child process! PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        // Modify the variable in the child process memory space
        shared_val += 50;
        printf("[CHILD] Modified shared_val to: %d (Memory address: %p)\n", shared_val, (void*)&shared_val);
        
        // Exit child process
        _exit(0);
    } 
    else {
        // --- THIS IS THE PARENT PROCESS ---
        // Wait for the child process to finish execution so output doesn't mix
        wait(NULL);

        printf("[PARENT] I am the parent process! PID: %d, Child PID: %d\n", getpid(), pid);
        
        // Check if the parent's variable was affected by the child's modification
        printf("[PARENT] Original shared_val is still: %d (Memory address: %p)\n", shared_val, (void*)&shared_val);
        printf("--- Proof: Child and Parent have completely isolated memory spaces! ---\n");
    }

    return 0;
}