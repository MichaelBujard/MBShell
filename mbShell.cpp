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

        parse_and_execute(command);
    }
}

void mbShell::parse_and_execute(string c){  // fill the argv array...parse...where?
    cout << "parse_and_execute called" << endl;

    char *cp = (char *)c.c_str();

    cout << "CP = " << cp << "<" << endl;


    // wait for history file to update before calling parse_and_execute()
    while(true){ 
        if (updateHistoryFile(c) == 0){
            cout << "updated the history file" << endl;
            break;
        }
        else {
            cout << "uh oh" << endl;
            perror("history");
            break;
        }
    }
    
    // on exit command
    if (c.compare("exit") == 0){
        exit(0);
    }

    // what about history?
    if (c.compare("./mbhistory") == 0){
        //do we need to do anything?
    }
    
    // what about bang?
    // does the command begin with a "bang"?
    // check if it's a bang substring.
    if (command.substr(0, 8).compare("./mbbang") == 0){
        command.insert(8, " ");
        cout << "./mbbang-1 should be split. It is: " << command << endl;
    }

    char **argv = get_input(cp); // program accepts no more than 10 command line arguments
    
    cout << argv[0] << "---" << argv[1] << "---" << argv[2] << argv[3] << endl;
    cout << argv[0] << endl;

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
        
    }    
}


int updateHistoryFile(string c) {
    cout << "updateHistoryFile called in parse_and_execute" << endl;
    int numLines = 1;
    ifstream in("historyFile.txt");
    std::string unused;
    while ( std::getline(in, unused) ){
        ++numLines;
    }
    in.close();

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
    cout << "get_input function called in parse_and_execute" << endl;

    char **command = (char **)malloc(10 * sizeof(char *));  // allocate for 10 command line args

    char *separator = (char *)" ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);

    while (parsed != NULL){

        command[index] = (char*)malloc(80 * sizeof(char));
        strcpy(command[index], parsed);  // make bunch of char pointer variables to do strcpy      
        cout << "command[" << index << "] : " << command[index] << endl;
        index++;

        parsed = strtok(NULL, separator);

    }
    
    command[index] = NULL;
    
    return command;

}
