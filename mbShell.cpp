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
#include <fcntl.h>


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
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * bool is_number(const string &s)                              *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool is_number(const string &s);

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

    string c_command = c;
    while(true) {
        if (c_command.substr(0, 1).compare("!") == 0 
            && c_command.compare("!") != 0) {
            string offset_str = c_command.erase(0, 1);  // remove "./mbbang" substring to get just "[index]"
            int offset = stoi(offset_str);  // convert string index to integer
            c_command = get_bangcmd(offset);
            cout << "while trying to update history file, c_command is " << c_command << endl;
        }        
        if (updateHistoryFile(c_command) == 0) {
            cout << "updated history file" << endl;
            break;
        } else {
            perror("history");
            break;
        }
    }

    string c_as_executable;  // the executable file of the c command...because the commands are files
    c_as_executable = c_command;
    cout << "Just outside updating history, c_as_executable = " << c_as_executable << endl;

    if (c_command.compare("exit") == 0){
        exit(0);
    }
    if (c_command.compare("history") == 0){
        c_as_executable = "./mbhistory";
    }
    if (c_command.compare("!") == 0){
        cout << "!" << endl;
    }
    
    /*
    if (c.substr(0, 1).compare("!") == 0 && c.compare("!") != 0){ // then ![offset]
        c_as_executable = c_as_executable.erase(0, 1);
        c_as_executable = "./mbbang " + c_as_executable;

        string offset_str = c_as_executable; // "./mbbang[index]", index can be some number
        
        string bangCommand;  // use later
        offset_str.erase(0, 8);  // remove "./mbbang" substring to get just "[index]"
        int offset = stoi(offset_str);  // convert string index to integer
        bangCommand = get_bangcmd(offset);
        c = bangCommand;
        cout << "C in BANG : " << c << endl;
    }
    */


    /*
    // wait for history file to update
    while(true){ 

        string offset_str = c_as_executable;  // "./mbbang[index]", index is some number
        string bangCommand;  // use later
        offset_str.erase(0, 8);  // remove "./mbbang" substring to get just "[index]"

        if (c_as_executable.substr(0, 8).compare("./mbbang") == 0 
            && c_as_executable.compare("./mbbang") != 0){
            cout << "this is true" << endl;

            int offset = stoi(offset_str);  // convert string index to integer

            bangCommand = get_bangcmd(offset);   
            cout << "bangCommand : " << bangCommand << endl;
            if (updateHistoryFile(bangCommand) == 0){
                cout << "updated the history file on bang, bangCommand = " << bangCommand << endl;
                break;
            } else {
                cout << "uh oh on bang" << endl;
                perror("history");
                break;
            }
        } else {

            cout << "in else" << endl;
            if (updateHistoryFile(c) == 0){
                cout << "updated the history file" << endl;
                break;
            } else {
                cout << "uh oh" << endl;
                perror("history");
                break;
            }
        }
    }
    */

    cout << "variable c is " << c << endl;
    cout << "variable c_command is " << c_command << endl;

    if (c_command.find(" > ") != std::string::npos){
        // starting with simplest use of file redirect from stdout to,
        // I guess, "non-standard output"
        //execute the 
    }

    char *cp = (char *)c_command.c_str();

    if (c_command.compare("history") == 0) {
        cp = (char *)c_as_executable.c_str();
    } else if (c_command.compare("!") == 0){
        // do nothing
        // cp = (char *)c_as_executable.c_str();
    } /*else if (c_command.substr(0, 1).compare("!") == 0 && c.compare("!") != 0){
        c.insert(1, " ");
        cp = (char *)c.c_str();
    }
    */

    char** argv = get_input(cp);

    fork_a_process(fork(), argv);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * helper methods                                  *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void fork_a_process(int pid, char **argv){
    cout << "fork a process called here" << pid << "and argv is " << endl;
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
        offset += numLines;  // numLines is the number of _filled_ lines.
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
        // getting command is done in the lines below.
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
    cout << "in get_bangcmd, the command is " << histCommand << endl;
    return histCommand;
}

void redirect_from_stdout(char *argv[]) {
    char *command[] = { (char *)"grep", (char *)"-E", (char *)"c$", (char *)"-", 0 };
    char *bin_file = command[0];

    int redirect_fd = open(".txt", O_CREAT | O_TRUNC | O_WRONLY);
    dup2(redirect_fd, STDOUT_FILENO);
    close(redirect_fd);

    if (execvp(bin_file, command) == -1) {
        cerr << "Error executing " << bin_file << endl;
    }
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
