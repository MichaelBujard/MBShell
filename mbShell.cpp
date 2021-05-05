#include "mbShell.h"

#include <iterator>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <fstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]){
    mbShell ms;
    ms.execute();
}

void mbShell::execute(){
    while (true){
        cout << greeting;
        cin >> command;

        // Here, put the command entered into the historyFile.txt history file.

        // enter the index of the file
        int numLines = 0;
        ifstream in("historyFile.txt");
        std::string unused;
        while ( std::getline(in, unused) )
            ++numLines;
        ++numLines;
        cout << "numLines " << numLines << endl;

        // instantiate file
        std::ofstream ofs;

        // open file for writing
        ofs.open("historyFile.txt", std::ofstream::out | std::ofstream::app);

        // enter the command into the history file
        ofs << numLines << " " << command << endl;
        ofs.close();
        // TODO: try getline
        parse_and_execute(command);
    }
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*
* WaitFor(pid)                                                  *
* Waits for foreground processes...based on Rochkind's waitfor  *
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void mbShell::WaitFor(int pid){
  int gotpid, status;

  while (true){
      gotpid = waitpid(pid, &status, 0);
      if (gotpid == pid) break;
  }
  cout << "Process [" << gotpid << "] Finished..." << endl;
}

void mbShell::parse_and_execute(string c){  // fill the argv array...parse...where?

    // is this parsing?
    //if (c.compare("ls") == 0){
    //    cout << "run mbls executable" << endl;
    //} else if (c.compare("pwd") == 0){
    //    cout << "run mbpwd executable" << endl;
    //} else if (c.compare("history") == 0){
    //    cout << "run mbhistory executable" << endl;
    //} else if (c.compare("!") == 0){
    //    cout << "run mbbang executable" << endl;
    //} else 
    if (c.compare("exit") == 0){
        exit(0);
    }

    int pid = fork();
    if (pid == -1){
        perror("fork");
    }    
    if (pid == 0){ 

        // we are in the child process
        cout << "child" << pid << endl;     

        char* argv[10]; // program accepts no more than 10 command line arguments

        // does the child parse here, or lines 47-58?        
        argv[0] = (char*)c.c_str();
        argv[1] = NULL; // must have this at the end of the command. Returns better results
           
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
        cout << "parent" << pid << endl;
        //WaitFor(id);
    }
    cout << "You entered a command." << endl;
}