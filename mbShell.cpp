#include "mbShell.h"

#include <string.h>
#include <string>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]){
    mbShell ms;
    ms.execute();
}

void mbShell::execute(){
    while (true){
        cout << greeting;
        cin >> command;

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

void mbShell::parse_and_execute(string c){
    int id = fork();
    if (id == 0){ // we are in the child process
        char* argv[10];
        argv[1] = NULL; // better results in the execvp
        argv[0] = (char*)c.c_str();
        execvp(argv[0], argv);
    }
    else {
        // Here we are in the parent
        WaitFor(id);
    }

    if (c.compare("exit") == 0){
        exit(0);
    } else if (c.compare("ls") == 0){
        cout << "run mbls executable" << endl;
    } else if (c.compare("pwd") == 0){
        cout << "run mbpwd executable" << endl;
    } else if (c.compare("history") == 0){
        cout << "run mbhistory executable" << endl;
    } else if (c.compare("!") == 0){
        cout << "run mbbang executable" << endl;
    }
    cout << "That sounds like a good idea." << endl;
}