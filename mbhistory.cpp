/*
 * 1. open historyFile.txt
 * 2. read from historyFile.txt
 * 3. print the contents of historyFile.txt to CLI
 * 4. close the file historyFile.txt.
 *
 * READ:
 * https://en.wikipedia.org/wiki/History_(command)
 *
 * https://www.digitalocean.com/community/tutorials/how-to-use-bash-history-commands-and-expansions-on-a-linux-vps
 * https://www.gnu.org/software/bash/manual/html_node/Bash-History-Builtins.html
 * manpages
 * The function does these three things:
 * 1. open
 * 2. read
 * 3. print
 * 4. close
 *
 * if user types "./mbhistory", print the line number before each command, and then close the file
 *
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

// Driver method
int main(int argc, char *argv[]) {

    // store the command just received to history;   
    // open the file.
    /*
    FILE * historyFile;
    historyFile = fopen("historyFile.txt", "a+");
    */


    // get the index of the last command in the history file
    // https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file
    // to do the above, count newline characters

    /*
    int ch;
    unsigned int numberOfLines = 0;
    while (EOF != (ch = getc(historyFile))) {
        if ('\n' == ch){
            ++numberOfLines;
        }
    }
    // increment by one to get the correct number.
    ++numberOfLines;
    */

    /*
    if (numberOfLines > 1) {
        // append the integer index to the file
        fprintf(historyFile, "%u ./mbhistory\n", numberOfLines);
    } else {
        fprintf(historyFile, "%u ./mbhistory\n", numberOfLines);
    }
    */

    /*
    // get the size of the file
    fseek(historyFile, 0L, SEEK_END);
    unsigned long len = (unsigned long)ftell(historyFile);
    */

    // read the file "historyFile.txt."
    // write the entire contents of the file to the terminal
    // TODO: get this function to work
    // Read contents from file
    /*
    char c = fgetc(historyFile);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(historyFile);
    }
    */

    // close the file now that we have written to it
    // and read from it
    /*
    fclose(historyFile);
    */

    return 0;

}
