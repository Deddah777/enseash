#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define BUFSIZE 1000    // Maximum command length

// Function to print exit code or signal along with execution time
void writeRet(char *str, int code, int time_ms) {
    char buf[BUFSIZE];
    sprintf(buf, "%d|%dms] ", code, time_ms); 
    strcat(str, buf);
    write(STDOUT_FILENO, str, strlen(str));   // print to terminal
}

// Function to split command line into arguments
char **splitCommand(char *cmd) {
    char **argv = malloc(BUFSIZE * sizeof(char *)); // array of string pointers
    int i = 0;

    char *token = strtok(cmd, " "); // split by spaces
    while (token != NULL) {
        argv[i] = malloc(strlen(token) + 1); // allocate memory for each argument
        strcpy(argv[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    argv[i] = NULL; // last element must be NULL for execvp
    return argv;
}

// Function to execute a command and measure execution time
void Commande(char *cmd, ssize_t n) {
    int pid, status;
    char msg[BUFSIZE];
    struct timespec start, end;
    int time_exe_ms;

    cmd[n - 1] = '\0';  // remove \n

    // Exit shell if command is "exit" or sends ctrl D
    if (strcmp(cmd, "exit") == 0 || n == 0) {
        write(STDOUT_FILENO, "Bye bye...\n", strlen("Bye bye...\n"));
        exit(EXIT_SUCCESS);
    }

    char **argv = splitCommand(cmd); // split input into arguments
    
    clock_gettime(CLOCK_MONOTONIC, &start);  // start timer

    pid = fork(); // create child process
    if (pid == 0) {  // child process
        execvp(argv[0], argv);  // execute command with arguments
        perror("execvp");        // if execvp fails
        exit(EXIT_FAILURE);
    }

    wait(&status); // parent waits for child to finish
    
    clock_gettime(CLOCK_MONOTONIC, &end);  // end timer
    // compute execution time in milliseconds
    time_exe_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

    // print exit code or signal along with execution time
    if (WIFEXITED(status)) {
        strcpy(msg, "enseash [exit:");
        writeRet(msg, WEXITSTATUS(status), time_exe_ms);
    } 
    else if (WIFSIGNALED(status)) {
        strcpy(msg, "enseash [sign:");
        writeRet(msg, WTERMSIG(status), time_exe_ms);
    }
}

int main() {
    const char *mess ="Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash % ";
    char cmd[BUFSIZE];
    ssize_t n;

    write(STDOUT_FILENO, mess, strlen(mess));   // print welcome message

    while (1) {                     // main shell loop
        n = read(STDIN_FILENO, cmd, BUFSIZE); // read user input
        Commande(cmd, n);                     // execute command
        write(STDOUT_FILENO, "% ", strlen("% ")); // print prompt
    }
}
