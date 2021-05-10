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
* char **tokenize(char *input)                                 *
* User-defined function that takes the command as an argument,  *
* and uses strtok() to split the string w.r.t. whitespace, ' '  *
* and return an array of strings instead (char **)              *
* and terminate the array with NULL                             *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
char **tokenize(char *input);
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
 * bool has_redirect(string str)     *
 * returns true if the command   *
 * has a redirect, else false.   *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool has_redirect(string str);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int history(void) 
 * handles the history function.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int history(void);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * string check_for_bang(string s);             *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
string check_for_bang(string s);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * bool has_bang(string s);                   *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool has_bang(string s);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * bool is_bang(string s);                    *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool is_bang(string s);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * string find_bang_redirect_command(string s); *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
string find_bang_redirect_command(string s);

/*
 * global variables
 */
bool redirect_flag; // so that we know when to do a special case with fork() on redirect.


int main(int argc, char* argv[]){
    mbShell ms;
    ms.execute();
}

void mbShell::execute(){
    while (true){
        // prompt
        cout << greeting;

        // get command from user

        getline(cin, command);
        //command = check_for_bang(command);
        
        parse_and_execute(command);
    }
}

void mbShell::parse_and_execute(string c){

    string c_command = c;

    int history_flag = 0;
    redirect_flag = has_redirect(c_command);  // check the command to see if it had a redirect
    while(history_flag != 1) {

        c_command = handle_history_c_command(c_command);

        history_flag = handle_history_update(c_command);
        if (history_flag == -1){
            cerr << "error updating history" << endl;
            break;
        }

    }

    // because the history command in mbshell is located in a separate file,
    // when we enter "history" we must change the command 
    // to the name of the executable file, "./mbhistory"
    string c_as_executable;  // the executable file/name of the c command...
    // because some of the commands are files (only history, actually.)
    c_as_executable = c_command;
    cout << "Just outside updating history, c_as_executable = " << c_as_executable << endl;


    // note that if there was a redirect, we need to address this first so that the command, 
    // ![index] or history or other, may be parsed below
    string mbstdout_filename; // if we reditect to a file, we will want its name.
    if (has_redirect(c_command)) {
        // execute the command and redirect output to file.
        
        vector<string> segvctr_processed = split_redirect(c_command);

        c_command = segvctr_processed.front();
        mbstdout_filename = segvctr_processed.back();
        
    }

    if (c_command.compare("exit") == 0) {
        exit(0);
    }
    
    // hold on for now.
    // if we have just a simple history command, execute it as normal.
    if (c_command.compare("history") == 0)
    {
        //history();
        c_as_executable = "./mbhistory";
    }
    
    if (c_command.compare("!") == 0) {
        cout << "!" << endl;
    }

    cout << "variable c is " << c << endl;
    cout << "variable c_command is " << c_command << endl;
    cout << "c_as_executable is " << c_as_executable << endl;

    char *cp = (char *)c_command.c_str();

    
    if (c_command.compare("history") == 0) {
        cp = (char *)c_as_executable.c_str();  // the same thing
        cout << "CP : " << cp[0] << endl;        
    }
    


    // in the special case of '>', the program forks and the child writes to the file.
    // after fork, it's done, so below these lines reclaim stdout to the terminal.
    if (c_command.compare("!") != 0)
    {
        cout << "entered condition" << endl;
        char** argv = tokenize(cp);  // This is true for any command except '!' by itself.
        cout << "argv[0] : " << argv[0] << endl;
        // what if we have a redirect?
        if (redirect_flag) // treat history case separately
        {
            /*
            if (c_command.compare("history") == 0)  // command evaluates to history > file.txt
            {
                cout << "history command entered" << endl;
                
                ofstream file;
                file.open(mbstdout_filename, ios::out);
                string line;

                // Backup streambuffers of cout
                streambuf *stream_buffer_cout = cout.rdbuf();
                streambuf *stream_buffer_cin = cin.rdbuf();

                // Get the streambuffer of the file
                streambuf *stream_buffer_file = file.rdbuf();

                // Redirect cout to file
                cout.rdbuf(stream_buffer_file);

                // now we can actually start using 'cout << "stuff" to write "stuff" to file
                cout << "stuff to file" << endl;
                std::ifstream f("historyFile.txt");
                if (f.is_open())
                    std::cout << f.rdbuf();

                // redirect cout back to screen 
                cout.rdbuf(stream_buffer_cout);
                cout <<  "stuff to terminal" << endl;

                file.close();
            } else  // command does not evaluate to 'history > file.txt' for some file.
            {  */

                
            int pipefd[2];
            pipe(pipefd);

            int pid = fork();
            if (pid == 0) // Child
            {
                
                cout << "In the child: argv[0] = " << argv[0] << endl;
                close(1);
                dup(pipefd[1]);
                close(pipefd[0]);
                execvp(argv[0], argv);
                
            } else // Parent
            {
                ofstream file;            
                close(pipefd[1]);
                FILE *virtual_file = fdopen(pipefd[0], "r");
                file.open(mbstdout_filename); // here replace with the filename given by command
                char c[1000];  // fix so that it will input an arbitrary number of lines.

                while (fgets(c, 999, virtual_file) != NULL)  // Only 1000 lines now
                {
                    file << c; 
                }
                cout << "parent process: OK here" << endl;

                /* Close files */
                fclose(virtual_file);
                file.close();

            }

        } else 
        {
            fork_a_process(fork(), argv);
        }   

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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int updateHistoryFile(string c)              *
 *                                              *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int updateHistoryFile(string c) 
{
    int numLines = 1;
    ifstream in("historyFile.txt");
    std::string unused;
    while ( std::getline(in, unused) )
    {
        ++numLines;
    }
    in.close();

    // instantiate file if needed
    std::ofstream ofs;

    // open file for writing. Append if file exists.
    ofs.open("historyFile.txt", std::ofstream::out | std::ofstream::app);

    // append the command to the history file
    ofs << numLines << " " << c << endl;
    ofs.close();
    return 0;
}

/*
 * See:
 * https://indradhanush.github.io/blog/writing-a-unix-shell-part-2/
 */
char **tokenize(char *input)
{

    char **cmd = (char **)malloc(10 * sizeof(char *));  // allocate for 10 command line args

    char *separator = (char *)" ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);

    while (parsed != NULL)
    {

        cmd[index] = (char*)malloc(80 * sizeof(char));
        strcpy(cmd[index], parsed);  // make bunch of char pointer variables to do strcpy      
        cout << "token[" << index << "] : " << cmd[index] << endl;
        index++;

        parsed = strtok(NULL, separator);

    }
    
    cmd[index] = NULL;
    
    return cmd;

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
    return histCommand;
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
        if (cmd.compare("!!") == 0)
        {
            cmd = "!-1";
        }
        if (has_redirect(cmd)) {
            // we have a command like !-1 > foo.txt (or !! > foo.txt)
            // first, get the command that ![offset] calls,\

            // then store it as c_command
            // then append the rest of the command to it,
            // which is something like '> foo.txt'

            /*
             * 1. split the command into '![offset]' (or '!!') and ' > foo.txt'
             * spaces are a bit of a problem, so we will just remove them. See
             * https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
             * about removal of spaces.
             */
            stringstream sscommand(cmd);  // could be !! > ...
            string segment;
            
            vector<string> segvctr_unprocessed;
            vector<string> segvctr_processed;
            while (getline(sscommand, segment, '>')) {
                segvctr_unprocessed.push_back(segment);
                int beginningOfSegmentIndex = findFirstIndex(segment, ' ');
                int endOfSegmentIndex = findLastIndex(segment, ' ') + 1;
                segment = segment.substr(beginningOfSegmentIndex, endOfSegmentIndex);
                segvctr_processed.push_back(segment);

            }

            /*
                * now we have something like:
                * vector<string> segvctr{"![index]", "file.txt"};
                * change '!!' to '!-1' before continuing.
            */

            if (segvctr_processed.front().compare("!!") == 0)
            {
                cmd = "!-1 >" + segvctr_unprocessed.back();
            }

            // if the command before redirection is just '!', then 
            // we don't want to deal with any offset, so just return cmd.
            // if not, we need to deal with the offset.
            if (segvctr_processed.front().compare("!") != 0)
            {
                // 2. find the command that ! calls from its offset
                string offset_str = cmd.erase(0, 1);  // remove "./mbbang" substring to get just "[index]"
                int offset = stoi(offset_str);  // convert string index to integer
                cmd = get_bangcmd(offset);
                cout << "while trying to update history file " << endl;
                cout << " inside special case in handleccmd() '>', c_command is " << cmd << endl;

                // 3. store the command in c_command. Done above.

                // 4. append the rest of the command to c_command
                cmd = cmd + " >" + segvctr_unprocessed.back();
                // the above is a temporary fix. We want to be able to have any number 
                // of file redirects. iterate through the vector and append '>' to the string.
                //return cmd;
            }

        } else // if no redirection, just '![offset] alone
        {
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
int handle_history_update(string cmd) 
{
    if (updateHistoryFile(cmd) == 0) 
    {
        return 1;
    } else 
    {
        perror("history");
        return -1;
    }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * bool has_redirect(string str)     *
 * returns true if the command   *
 * has a redirect, else false.   *
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool has_redirect(string str) 
{
    return (str.find('>') != std::string::npos);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * 
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
vector<string> split_redirect(string s)
{
    stringstream sscmd(s);
        string sgmnt;
        vector<string> segvctr;
        while (getline(sscmd, sgmnt, '>')) {
            int beginIndex = findFirstIndex(sgmnt, ' ');
            int endIndex = findLastIndex(sgmnt, ' ') + 1;
            sgmnt = sgmnt.substr(beginIndex, endIndex);
            segvctr.push_back(sgmnt);
        }
        return segvctr;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
 * int history(void) 
 * handles the history function.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int history(void)
{
    std::ifstream f("historyFile.txt");

    if (f.is_open())
        std::cout << f.rdbuf();
        
    return 0;
}

string check_for_bang(string s)
{
    string cmd = s;

    if (is_bang(s))
    {
        cmd = s;
    }

    if (has_bang(s))
    {
        if (has_redirect(s))
        {
            find_bang_redirect_command(s);
            stringstream sscommand(cmd);  // could be !! > ...
            string segment;
            vector<string> segvctr_processed;
            getline(sscommand, segment, '>');
            cmd = segment.substr(1, findLastIndex(segment, ' '));
        } else {

        }
    }
    return cmd;
}

bool has_bang(string s)
{
    return (s.substr(0, 1).compare("!") == 0);
}

bool is_bang(string s)
{
    return (s.compare("!") == 0);
}

string find_bang_redirect_command(string s)
{
    return s;
}
