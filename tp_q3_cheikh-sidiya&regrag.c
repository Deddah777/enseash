#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

// Abdellahi CHEIKH SIDIYA & Salma REGRAG

#define BUFSIZE 1000    // max command size

void Commande(char *cmd, ssize_t n) {
    int pid, status;
    cmd[n - 1] = '\0';          // remove '\n'
    
    // exit shell if user types "exit" or presses Ctrl+D
    if (strcmp("exit", cmd) == 0 || n == 0) {
        write(STDOUT_FILENO, "Bye bye...\n", strlen("Bye bye...\n")); // print goodbye message
        exit(EXIT_SUCCESS);                                        
    }

    pid = fork();

    if (pid == -1) {            // fork error
        perror("fork");
    }
    else if (pid == 0) {        // child process
        execlp(cmd, cmd, NULL); // run command
        perror("execlp");       // on failure
        exit(EXIT_FAILURE);
    }
    else {                      // parent process
        wait(&status);          // wait for child
    }
}

int main() {
    const char *mess ="Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash % ";
    char cmd[BUFSIZE];
    ssize_t n;

    write(STDOUT_FILENO, mess, strlen(mess));   // welcome message

    while (1) {                     // shell loop
        n = read(STDIN_FILENO, cmd, BUFSIZE); // read command
        Commande(cmd, n);          // execute command
        write(STDOUT_FILENO, "% ", strlen("% "));         // prompt
    }
}
