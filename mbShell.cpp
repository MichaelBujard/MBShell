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
#include <vector>

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
* string get_bangcmd(string c);                                 *
*                                                               *
*                                                               *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
string get_bangcmd(int offset);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int findLastIndex(string& str, char x)    *
 * From the right of the string to the left, *
 * find the first index from the end where   *
 * the command begins. This is the first     *
 * non-whitespace character if char x = ' '. *
 * Return the index.                         *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 int findLastIndex(string& str, char x);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int findFirstIndex(string& str, char x) *
 * find the index of the first character   *
 * in str that does not match x.           *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int findFirstIndex(string& str, char x);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * string handle_history_c_command(string c);   *
 *                                              *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
string handle_history_c_command(string str);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int handle_history_update(string strc)  *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int handle_history_update(string strc);

 /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
  * vector<string> split_redirect(string s)*
  *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
vector<string> split_redirect(string s);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * bool redirect(string str)     *
 * returns true if the command   *
 * has a redirect, else false.   *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool redirect(string str);

/*
 * global variables
 */
int saved_stdout;  // for use with redirect.
bool redirect_flag; // so that we know when to do a special case with fork() on redirect.


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
    int flag = 0;
    redirect_flag = redirect(c_command);  // check the command to see if it had a redirect
    cout << "redirect flag has value of : " << redirect_flag << endl;
    while(flag != 1) {

        c_command = handle_history_c_command(c_command);

        flag = handle_history_update(c_command);
        if (flag == -1)
            break;

    }

    string c_as_executable;  // the executable file/name of the c command...
    // because some of the commands are files (only history, actually.)
    c_as_executable = c_command;
    cout << "Just outside updating history, c_as_executable = " << c_as_executable << endl;


    // note that if there was a redirect, we need to address this first so that the command, 
    // ![index] or history or other, may be parsed below

    if (redirect(c_command)) {
        // execute the command and redirect output to file.
        
        vector<string> segvctr_processed = split_redirect(c_command);

        c_command = segvctr_processed.front();

        //close(fd);

        // get the file descriptor, which is the last segment of the vector
        //char *file_name = (char *)segvctr_processed.back().c_str();


        // before redirecting output to the file specified, 
        // save the current stdout for use after we have called the process.
        //saved_stdout = dup(STDOUT_FILENO);

        // now redirect output from stdout to the file
        //int redirect_fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY);
        
        //dup2(redirect_fd, STDOUT_FILENO);
        //cout << "redirect test" << endl;
    }

    if (c_command.compare("exit") == 0) {
        exit(0);
    }
    if (c_command.compare("history") == 0) {
        c_as_executable = "./mbhistory";
        cout << "on !71, c_command is history, and c_as_exec... = ./mbhistory" << endl;
    }
    if (c_command.compare("!") == 0) {
        cout << "!" << endl;
    }

    cout << "variable c is " << c << endl;
    cout << "variable c_command is " << c_command << endl;

    // if we have done a ! that evaluates to history, this will become ./mbhistory.
    char *cp = (char *)c_command.c_str();

    if (c_command.compare("history") == 0) {
        cp = (char *)c_as_executable.c_str();  // the only useful case for c_as_executable
    } else if (c_command.compare("!") == 0){
        // do nothing yet.
    }


    // in the special case of '>', the program forks and the child writes to the file.
    // after fork, it's done, so below these lines reclaim stdout to the terminal.
    if (c_command.compare("!") != 0){
        char** argv = get_input(cp);  // This is true for any command except '!' by itself.
        cout << "argv[0] : " << argv[0] << endl;

        // what if we have a redirect?
        if (redirect_flag) {
            // what happens on redirect?

            /*
             * pipe,
             * fork:
             *      child:
             *          1. close 
             *
             *      parent: 
             */ 
            int status;
            int pipefd[2]; // instantiate pipe


            // argv[0] = "ls";
            // argv[1] = "-l";
            // argv[2] = 0;
            // argv is our input now.
            
            pipe(pipefd);

            int pid = fork();

            if (pid == 0) // Child
            {
                close(pipefd[0]);
                close(1);
                dup(pipefd[1]);
                execvp(argv[0], argv);
                cout << "this was fun" << endl;
            } else // Parent
            {
                char ch;

                /* 
                 * Instantiate file pointers
                 * and open for reading / writing.
                 */
                FILE *f = fdopen(pipefd[0], "r");
                FILE *fp = fopen("file.txt", "w");

                /* Ensure f opened successfully*/
                if (f == NULL)
                {
                    puts("Error opening input file");
                }

                /* Ensure fp opened successfully*/
                if (fp == NULL)
                {
                puts("Error opening output file");
                }

                /* Read, write */
                while(1)
                {
                    ch = fgetc(f);
                    if (ch==EOF)
                        break;
                    else
                        fputc(ch, fp);
                }

                /* Close files */
                fclose(f);
                fclose(fp);

            }


            ofstream someFile;

        }
        fork_a_process(fork(), argv);
    }

    // here, reclaim stdout to the terminal...right?
    /*
     * restore stdout
     */
    //dup2(saved_stdout, STDOUT_FILENO);
    //close(saved_stdout);


    
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 *void redirect_from_stdout(char *argv[], char * filename)         *
 *                                                                 *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void redirect_from_stdout(char *argv[], string filename) {
    char *command[] = { (char *)"grep", (char *)"-E", (char *)"c$", (char *)"-", 0 };
    char *bin_file = command[0];

    int redirect_fd = open(".txt", O_CREAT | O_TRUNC | O_WRONLY);
    dup2(redirect_fd, STDOUT_FILENO);
    close(redirect_fd);

    if (execvp(bin_file, command) == -1) {
        cerr << "Error executing " << bin_file << endl;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int findLastIndex(string& str, char x)    *
 * From the right of the string to the left, *
 * find the first index from the end where   *
 * the command begins. This is the first     *
 * non-whitespace character if char x = ' '. *
 * Return the index.                         *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int findLastIndex(string& str, char x) {
    // Traverse from right
    for (int i = str.length() - 1; i >= 0; i--)
        if (str[i] != x)
            return i;
 
    return -1;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int findFirstIndex(string& str, char x) *
 * find the index of the first character   *
 * in str that does not match x.           *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int findFirstIndex(string& str, char x) {
    int index = -1;
    for (int i = 0; i < str.length(); i++)
        if (str[i] != x) {
            index = i;
            break;
        }
    return index;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * string handle_history_c_command(string c);   *
 *                                              *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
string handle_history_c_command(string cmd) {

    if (cmd.substr(0, 1).compare("!") == 0 && cmd.compare("!") != 0) {
        cout << "c_command : " << cmd << endl;
        if (redirect(cmd)) {
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
            stringstream sscommand(cmd);
            string segment;
            
            vector<string> segvctr_unprocessed;
            vector<string> segvctr_processed;
            while (getline(sscommand, segment, '>')) {
                segvctr_unprocessed.push_back(segment);
                cout << "unprocessed segment : '" << segment << "'" << endl;
                int beginningOfSegmentIndex = findFirstIndex(segment, ' ');
                int endOfSegmentIndex = findLastIndex(segment, ' ') + 1;
                segment = segment.substr(beginningOfSegmentIndex, endOfSegmentIndex);
                segvctr_processed.push_back(segment);
                cout << "processed segment : '" << segment << "'" << endl;
            }

            /*
                * now we have something like:
                * vector<string> segvctr{"![index]", "file.txt"};
            */

            if (segvctr_processed.front().compare("!") == 0) {
                cmd = "! > " + segvctr_unprocessed.back();
            } else {
                // 2. find the command that ! calls from its offset
                string offset_str = cmd.erase(0, 1);  // remove "./mbbang" substring to get just "[index]"
                int offset = stoi(offset_str);  // convert string index to integer
                cmd = get_bangcmd(offset);
                cout << "while trying to update history file \
                inside special case in handleccmd() '>', c_command is " << cmd << endl;

                // 3. store the command in c_command. Done above.

                // 4. append the rest of the command to c_command
                cmd = cmd + " >" + segvctr_unprocessed.back();
                // the above is a temporary fix. We want to be able to have any number 
                // of file redirects. iterate through the vector and append '>' to the string.
                //return cmd;
            }


        } else {
            /*
            * suppose we are not redirecting with bang. i.e.
            * suppose we are doing any normal, valid command with 
            * ![offset] alone. Then in history we just print out the command corresponding to the 
            * bang, that is, the command in the history file at offset = index of history file
            */

            string offset_str = cmd.erase(0, 1);  // remove "./mbbang" substring to get just "[index]"
            int offset = stoi(offset_str);  // convert string index to integer
            cmd = get_bangcmd(offset);
            cout << "while trying to update history file, c_command is " << cmd << endl;
            //return cmd;
        }
    }

    return cmd;        

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int handle_history_update(string strc)  *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int handle_history_update(string cmd) {
    if (updateHistoryFile(cmd) == 0) {
        cout << "updated history file" << endl;
        return 1;
    } else {
        perror("history");
        return -1;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * bool redirect(string str)     *
 * returns true if the command   *
 * has a redirect, else false.   *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool redirect(string str) 
{
    return (str.find('>') != std::string::npos);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * 
 */
vector<string> split_redirect(string s)
{
    stringstream sscmd(s);
        string sgmnt;
        vector<string> segvctr;
        while (getline(sscmd, sgmnt, '>')) {
            cout << "unprocessed segment : '" << sgmnt << "'" << endl;
            int beginIndex = findFirstIndex(sgmnt, ' ');
            int endIndex = findLastIndex(sgmnt, ' ') + 1;
            sgmnt = sgmnt.substr(beginIndex, endIndex);
            segvctr.push_back(sgmnt);
            cout << "processed segment : '" << sgmnt << "'" << endl;
        }
        return segvctr;
}