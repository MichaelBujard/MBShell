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
 * 2. read from historyFile.txt
 * 3. print the contents of historyFile.txt out onto the "terminal window" (what is this called,
 * a console?)
 * 
 * 4. close the file.
 * 
 * Note that no fork is needed for this particular function.
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