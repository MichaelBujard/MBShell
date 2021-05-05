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
#include <iostream>
#include <fstream>

using namespace std;  // for simpler code

// Driver method
int main()
{
    std::ifstream f("historyFile.txt");

    if (f.is_open())
        std::cout << f.rdbuf();
        
    return 0;
}
