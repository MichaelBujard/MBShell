/*
 * if the index is negative, then it's relative to the end of the history file
 * if the index is positive, then it's relative to the beginning of the history file
 * The .cpp file here needs to be able to read the number following the ./mb!
 * That is, if ./mb!-1, then it behaves as !-1
 */


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
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Driver method
int main(int argc, char *argv[]) {

    // open the history file for reading
    ifstream inFile("historyFile.txt");

    // line-based parsing, using streams from sstring and string libraries
    string line;
    while (getline(inFile, line)){

        istringstream iss(line);
        int histCommIdx;  // the line number of the history file
        string histCommand = line;  // everything after lineNumber and the following space
        iss >> histCommIdx;

        string::size_type n = 0;
        n = histCommand.find_first_not_of( " \t", n );
        n = histCommand.find_first_of( " \t", n );
        histCommand.erase( 0,  histCommand.find_first_not_of( " \t", n ) );

        cout << "The command at " << histCommIdx << " is : " << histCommand << "\n";
    }
    
    return 0;
}

