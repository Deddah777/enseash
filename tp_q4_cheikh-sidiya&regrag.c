#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1000    // Maximum command size

// Function to write exit or signal code in the shell prompt
void writeRet(char *str, int code) {
    char code_buf[BUFSIZE];

    sprintf(code_buf, "%d", code);   // Convert code to string
    strcat(str, code_buf);           // Append code to message
    strcat(str, "] % ");             // Add prompt format
    write(STDOUT_FILENO, str, strlen(str)); // Print to terminal
}

// Function to execute a command
void Commande(char *cmd, ssize_t n) {
    int pid, status;
    char msg[BUFSIZE];
    cmd[n - 1] = '\0';          // Remove newline from input
    
    // Exit the shell if user types "exit" or enters nothing
    if (strcmp("exit", cmd) == 0 || n == 0) {
        write(STDOUT_FILENO, "Bye bye...\n", strlen("Bye bye...\n"));
        exit(EXIT_SUCCESS);
    }

    pid = fork();  // Create child process
    
    if (pid == -1) {            // Fork error
        perror("fork");
    }
    else if (pid == 0) {        // Child process
        execlp(cmd, cmd, NULL); // Execute command
        perror("execlp");       // Print error if exec fails
        exit(EXIT_FAILURE);     
    }
    else {                      // Parent process
        wait(&status);          // Wait for child to finish
        if (WIFEXITED(status)) {        // Child exited normally
            strcpy(msg, "enseash [exit:");
            writeRet(msg, WEXITSTATUS(status)); // Print exit code
        }
        else if (WIFSIGNALED(status)) {       // Child terminated by signal
            strcpy(msg, "enseash [sign:");
            writeRet(msg, WTERMSIG(status)); // Print signal number
        }
    }
}

int main() {
    const char *mess ="Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash % ";
    char cmd[BUFSIZE];
    ssize_t n;

    write(STDOUT_FILENO, mess, strlen(mess));   // Print welcome message

    while (1) {                     // Main shell loop
        n = read(STDIN_FILENO, cmd, BUFSIZE); // Read user input
        Commande(cmd, n);          // Execute the command
        write(STDOUT_FILENO, "% ", strlen("% ")); // Print prompt
    }
}
