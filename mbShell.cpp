#include "mbShell.h"

#include <cstdio>
#include <iterator>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <cstring>
#include <bits/stdc++.h>
#include <stdlib.h>

using namespace std;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* void updateHistoryFile(string c)                              *
* User-defined function that takes the command as an argument,  *
* and prints it to the historyFile.txt history file             *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int updateHistoryFile(string c);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* char **get_input(char *input)                                 *
* User-defined function that takes the command as an argument,  *
* and uses strtok() to split the string w.r.t. whitespace, ' '  *
* and return an array of strings instead                        *
* and terminate the array with NULL                             *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
char **get_input(char *input);

int main(int argc, char* argv[]){
    mbShell ms;
    ms.execute();
}

void mbShell::execute(){
    while (true){
        cout << greeting;
        cin >> command;

        // Here, put the command entered into the historyFile.txt history file.

        // enter the index of the file

        // this while loop allows them to wait until the 
        // history file updates before executing
        while(true){
            if (updateHistoryFile(command) == 0)
                break;
            else {
                perror("history");
                break;
            }
        }

        // TODO: try getline
        parse_and_execute(command);
    }
}

void mbShell::parse_and_execute(string c){  // fill the argv array...parse...where?

    // Do we fill the argv array here?
    char* argv[10]; // program accepts no more than 10 command line arguments

    // does the child parse here, or lines 47-58?        
    argv[0] = (char*)c.c_str();
    argv[1] = NULL; // must have this at the end of the command. Returns better results



    if (c.compare("exit") == 0){
        cout << "in exit case" << endl;
        exit(0);
    }

    /*
    Fork section
    */
    int pid = fork();
    if (pid == -1){
        perror("fork");
    }    
    if (pid == 0){ 

        // we are in the child process
        cout << "child: " << pid << endl;     

       //strtok()    
        // child executes here
        if (execvp(argv[0], argv) == -1){
            perror("exec");
        }
    }
    else {
        // Here we are in the parent
        if (wait(0) == -1){
            perror("wait");
        }
        cout << "parent: " << pid << endl;
        //WaitFor(id);
    }
    cout << "You entered a command." << endl;
}


int updateHistoryFile(string c) {
    int numLines = 1;
    ifstream in("historyFile.txt");
    std::string unused;
    while ( std::getline(in, unused) )
        ++numLines;
    cout << "numLines " << numLines << endl;

    // instantiate file
    std::ofstream ofs;

    // open file for writing
    ofs.open("historyFile.txt", std::ofstream::out | std::ofstream::app);

    // enter the command into the history file
    ofs << numLines << " " << c << endl;
    ofs.close();
    return 0;
}

/*
 * See:
 * https://indradhanush.github.io/blog/writing-a-unix-shell-part-2/
 */
char **get_input(char *input){
    char **command = (char **)malloc(8 * sizeof(char *));
    char *separator = (char *)" ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL){
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;

}
