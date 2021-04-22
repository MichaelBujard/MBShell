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
#include<iostream>      // for input/output capability
#include<string>        // for working with strings from user input and text file read input
#include<fstream>       // for working with files
using namespace std;    // for simpler code

/*
 * forward declaration of functions
 */
void history(void);  // this is the function that the main() function calls.


// Driver method
int main(int argc, char *argv[]) {

    // open the file.
    
    // get the most recent instruction, that is, most recent user input.

    // get the information associated with the most recent user input.

    // append the most recent user input to the end of the file with associated data.
    
    // read the file "historyFile.txt."

    // write the entire contents of the file to the terminal

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