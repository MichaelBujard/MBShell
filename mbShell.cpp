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
* and return an array of strings instead (char **)              *
* and terminate the array with NULL                             *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
char **get_input(char *input);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* void fork_a_process(int pid, char **argv)                     *
*                                                               *
*                                                               *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void fork_a_process(int pid, char **argv);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* string bangCommand(string c);                                 *
*                                                               *
*                                                               *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
string get_bangcmd(int offset);

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

    if (c.compare("history") == 0){
        // make c = ./mbhistory
        c = "./mbhistory";
    }
    if (c.compare("!") == 0){
        c = "./mbbang";
        cout << "hit ! c = " << c << endl;
    }
    if (c.substr(0, 1).compare("!") == 0 && c.compare("!") != 0){
        c.erase(0, 1);
        c = "./mbbang" + c;
    }

    // wait for history file to update before continuing
    while(true){ 
        // unless it's bang, in which case we need to check if the updated 
        // history file is going to do the
        if (c.substr(0, 8).compare("./mbbang") == 0 && c.compare("./mbbang") != 0){

            string offset_str = c;
            string bangCommand;
        offset_str.erase(0, 8);
            int offset = stoi(offset_str);
            bangCommand = get_bangcmd(offset);
            bangCommand.erase(0, 8); // remove ./mbbang substring
            cout << "BC: " << bangCommand << endl;
            if (updateHistoryFile(bangCommand) == 0){
                cout << "updated the history file on bang" << endl;
                break;
            } else {
                cout << "uh oh on bang" << endl;
                perror("history");
                break;
            }
        } else {
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
    }

    char *cp = (char *)c.c_str();
    cout << "CP = " << cp << "<" << endl;

    // on exit command
    if (c.compare("exit") == 0){
        exit(0);
    }

    // what about history?
    /*
    if (c.compare("./mbhistory") == 0){
        //do we need to do anything?
    }
    */
    
    // what about bang?
    // does the command begin with a "bang"?
    // check if it's a bang substring.
    if (c.substr(0, 8).compare("./mbbang") == 0){
        c.insert(8, " ");
        cout << "./mbbang-1 should be split. It is: " << c << endl;
    }

    char** argv = get_input(cp);

    fork_a_process(fork(), argv);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * helper methods                                  *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void fork_a_process(int pid, char **argv){
    //Fork section
    
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * string get_bangcmd(char *argv[])            *
 *                                             *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
string get_bangcmd(int offset){

    // This offset value should cause the 
    // function to make the shell execute the 
    // second-to-last command in the history file. 

    //get the size of the file
    int numLines = 1;
    ifstream in("historyFile.txt");
    std::string unused;
    while ( std::getline(in, unused) )
        ++numLines;
    cout << "numLines " << numLines << endl;
    in.close();
    
    if (offset < 0) {
        // relative address. Make absolute by subtracting from length of history file, minus 1.
        offset += --numLines;
    }

    // access the history command at the line number specified by offset

    // open the history file for reading
    ifstream inFile("historyFile.txt");

    // line-based parsing, using streams from sstring and string libraries
    string line;
    int histCommIdx;  // the line number of the history file
    string histCommand;
    while (getline(inFile, line)){
        if (numLines < offset || offset == 0){
            histCommand = "";
            break;
        }
        histCommand = line;  // everything after lineNumber and the following space
        istringstream iss(line);
        iss >> histCommIdx;

        // get the command, which is everything after the index taken from above.

        string::size_type n = 0;

        n = histCommand.find_first_not_of( " \t", n );
        n = histCommand.find_first_of( " \t", n );

        histCommand.erase( 0,  histCommand.find_first_not_of( " \t", n ) );

        // if the current line index is the offset
        if (offset == histCommIdx){
            // we found the command. Exit the loop.
            break;
        }
    }
    inFile.close();
    return histCommand;
}