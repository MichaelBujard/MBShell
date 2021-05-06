#include <iostream>
#include <string>       
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

class mbShell {
private:

public: 

    string greeting = "Hi there > ";
    string command;

    // The run loop
    void execute();

    // Helper Functions
    void parse_and_execute(string c);

    //void WaitFor(int pid);
};