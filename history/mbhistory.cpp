/*
 * the history list is an array of history entries. A history entry is declared as follows,
 * according to the manual "man history":
 * We don't need the data structure as provided in the manpage, because our implementation of the 
 * history list is more simple.
 * What the program does is, 
 * when the user types "./mbhistory" or whatever the executable file ends up being called,
 * the file executes the "history()" function,
 * which is defined in the mbhistory.cpp file.
 * This history() function has the following description:
 *
 * 1. open historyFile.txt
 * 2. Append the most recent instruction,
 *    the instruction that just executed, to "historyFile.txt"
 * 3. read from historyFile.txt
 * 4. print the contents of historyFile.txt out onto the "terminal window" (what is this called,
 * a console?)
 * 
 * 4. close the file.
 * 
 * READ:
 * https://en.wikipedia.org/wiki/History_(command)
 * 
 * https://www.digitalocean.com/community/tutorials/how-to-use-bash-history-commands-and-expansions-on-a-linux-vps
 * https://www.gnu.org/software/bash/manual/html_node/Bash-History-Builtins.html
 * manpages
 * The function does these three things:
 * 1. open
 * 2. append
 * 3. close
 *
 * if type history, print w/#s in front ofthe commands, and then close the 
 *
 * Note that no fork is needed for this particular function.
 */


/*
 * What include statements do we need?
 */
#include <ios>
#include<iostream>      // for input/output capability
#include <iterator>
#include<string>        // for working with strings from user input and text file read input
#include<fstream>       // for working with files
using namespace std;    // for simpler code

/*
 * forward declaration of functions
 */
void history(void);  // this is the function that the main() function calls.


// Driver method
int main(int argc, char *argv[]) {

    // store the command just received to history;

    // get the index of the last command in the history file
    // https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file
    // open the file.
    unsigned int numberOfLines = 0;
    FILE * historyFile;
    historyFile = fopen("historyFile.txt", "a+");
    int ch;
    // count newline characters
    while (EOF != (ch = getc(historyFile))) {
        if ('\n' == ch){
            ++numberOfLines;
        }
    }
    // increment by one to get the correct number.
    ++numberOfLines;

    // get the information associated with the most recent user input.
    // to do the above first get the line number and append it to the end.
    // we just did a ./mbhistory command, so increment plus one again.
    ++numberOfLines;

    // append the integer index to the file
    fprintf(historyFile, "\n%u ", numberOfLines);

    // get the most recent instruction, that is, most recent user input.
    // That would be the name of the executable just called, ./mbhistory

    // close the file now that we have written to it
    
    // read the file "historyFile.txt."

    // write the entire contents of the file to the terminal
    
    return 0;

}

// these seem to be the only two variables we will need.


int history_base;
/*
The logical offset of the first entry in the history list.
*/

int history_length;
/*
The number of entries currently stored in the history list.
*/

/*
 * The main method that this program uses, 
 * void history()
 */