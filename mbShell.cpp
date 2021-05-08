#include "mbShell.h"

#include <cstdio>
#include <iterator>
#include <sstream>
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
        if (c_command.substr(0, 1).compare("!") == 0 && c_command.compare("!") != 0) {
            cout << "c_command : " << c_command << endl;
            if (c_command.find((string)">") != std::string::npos) {
                cout << "FOUND '>'" << endl;
                // we have a command like !-1 > foo.txt
                // first, get the command that ![offset] calls,
                // then store it as c_command
                // then append the rest of the command to it,
                // which is something like '> foo.txt'

                /*
                    * 1. split the command into '![offset]' and ' > foo.txt'
                    * spaces are a bit of a problem, so we will just remove them. See
                    * https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
                    * about removal of spaces.
                    */
                stringstream sscommand(c_command);
                string segment;
                vector<string> segvctr;
                while (getline(sscommand, segment, '>')) {
                    //segment.erase(remove_if(segment.begin(), segment.end(), isspace), segment.end());
                    segvctr.push_back(segment);
                    cout << "type of segment is " << typeid(segment).name() << endl;
                    cout << "SEGMENT : " << segment << endl;
                }
                // loop through variables, removing whitespace.
                for (std::size_t i = 0; i < segvctr.size(); i++) {
                    cout << segvctr.at(i) << endl;
                }
                /*
                    * now we have something like:
                    * vector<string> segvctr{"![index] "};

                // 2. find the command that ! calls from its offset

                // 3. store the command in c_command

                // 4. append the rest of the command to c_command


            } else {
                /*
                * suppose we are not redirecting with bang. i.e.
                * suppose we are doing any normal, valid command with 
                * ![offset] alone. Then in history we just print out the command corresponding to the 
                * bang, that is, the command in the history file at offset = index of history file
                */

                string offset_str = c_command.erase(0, 1);  // remove "./mbbang" substring to get just "[index]"
                int offset = stoi(offset_str);  // convert string index to integer
                c_command = get_bangcmd(offset);
                cout << "while trying to update history file, c_command is " << c_command << endl;
            }
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

    cout << "variable c is " << c << endl;
    cout << "variable c_command is " << c_command << endl;

    if (c_command.find(" > ") != std::string::npos){
        // starting with simplest use of file redirect from stdout to,
        // I guess, "non-standard output"
        cout << "found redirection!" << endl;
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

    if (c_command.compare("!") != 0){
        char** argv = get_input(cp);
        fork_a_process(fork(), argv);
    }
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
