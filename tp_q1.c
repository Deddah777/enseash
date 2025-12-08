#include <unistd.h> 
#include <string.h>

// Regrag Salma & CHEIKH SIDIYA Abdellahi

int main(){
    // Text to display on start
    const char *mess_prompt = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\nenseash%\n";

    // Send text to stdout
    write(STDOUT_FILENO, mess_prompt, strlen(mess_prompt));
}
