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
#include <cstdio>
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
long int getSize(char filename[]);


// Driver method
int main(int argc, char *argv[]) {

    // store the command just received to history;
    // get the index of the last command in the history file
    // https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file
    
    // open the file.
    FILE * historyFile;
    historyFile = fopen("historyFile.txt", "a+");


    // count newline characters
    int ch;
    unsigned int numberOfLines = 0;
    while (EOF != (ch = getc(historyFile))) {
        if ('\n' == ch){
            ++numberOfLines;
        }
    }
    // increment by one to get the correct number.
    ++numberOfLines;

    if (numberOfLines > 1) {
        // append the integer index to the file
        fprintf(historyFile, "%u ./mbhistory\n", numberOfLines);
    } else {
        fprintf(historyFile, "%u ./mbhistory\n", numberOfLines);
    }

    // get the size of the file
    fseek(historyFile, 0L, SEEK_END);
    unsigned long len = (unsigned long)ftell(historyFile);

    // read the file "historyFile.txt."
    // write the entire contents of the file to the terminal
    // Read contents from file
    char c = fgetc(historyFile);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(historyFile);
    }

    // close the file now that we have written to it
    fclose(historyFile);

    return 0;

}

/*
URLs:
https://www.geeksforgeeks.org/basics-file-handling-c/
https://stackoverflow.com/questions/8236/how-do-you-determine-the-size-of-a-file-in-c
https://www.geeksforgeeks.org/c-program-print-contents-file/
https://www.geeksforgeeks.org/c-program-find-size-file/
https://www.tutorialspoint.com/c_standard_library/c_function_fseek.htm

*/

/*
// we don't seem to need this function anymore.
long int getSize(char filename[]) {
  // open file for reading
  FILE *fp = fopen(filename, "r");

  // check if the file exists...just in case!
  if (fp == NULL) {
    printf("History Library not found!\n");
    return -1;
  }

  fseek(fp, 0L, SEEK_END);

  // get the size of the file TODO: FInish:

}
*/

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
