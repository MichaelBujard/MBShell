#include "mbShell.h"

#include <cstdio>
#include <iterator>
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
        getline(cin, command);  
        cout << "command is : " << command << "<" << endl;      

        // Here, put the command entered into the historyFile.txt history file.
        while(true){          // wait until the history file updates before executing 
            if (updateHistoryFile(command) == 0)
                break;
            else {
                perror("history");
                break;
            }
        }

        parse_and_execute(command);
    }
}

void mbShell::parse_and_execute(string c){  // fill the argv array...parse...where?

    char *cp = (char *)c.c_str();
    cout << "CP = " << cp << "<" << endl;

    // Do we fill the argv array here?
    char** argv = get_input(cp); // program accepts no more than 10 command line arguments
    cout << argv[0] << "---" << argv[1] << "---" << argv[2] << argv[3] << endl;
    // why is this not printing with command "ls"? What about
    cout << argv[0] << endl;


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
            
            cout << "exec failed here" << endl;
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
    char **command = (char **)malloc(30 * sizeof(char *));
    // what is 
    // this might be problematic, because we never free the memory allocation...?
    // or does it not matter, because this function's stack and data segment is discarded 
    // after the function returns a value?
    char *separator = (char *)" ";
    char *parsed;
    int index = 0;


    cout << "get_input function called" << endl;

    parsed = strtok(input, separator);
    cout << "outside while loop of get_input, parsed is : " << parsed << endl;
    while (parsed != NULL){
        cout << "command[index] = " << command[index] << endl;
        command[index] = parsed;
        cout << "command[index] after setting to parsed is : " << command[index] << endl;        
        index++;
        cout << "waiting for more input: " << endl;
        parsed = strtok(NULL, separator);
        cout << "parsed is : [" << parsed << "]" << endl;
        // why is the end being weird? seg faults, and printing output.
    }

    // debug char **command
    // check out what command is for all its length...
    #int i;
    #for (i = 0; i < 30; i++){
    #    if (i == 0){
    #        cout << "[" << command[0] << endl;
    #    } else if (i < 29 && i > 0){
    #       cout << ", " << command[i];
    #   } else if (i == 29){
    #        cout << command[i] << "]" << endl;
    #    }
    #}

    command[index] = NULL;
    cout << command[0] << "---" << command[1] << "---" << command[index] << "---" << endl;
    return command;

}
