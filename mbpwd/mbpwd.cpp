#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>

using namespace std;




int main(int argc, char *argv[]) {

    string s = "";
    
    while (1) {  // while true
        
        struct stat current;
        struct stat parent;
        
        stat (".", &current);
        int current_inode =  current.st_ino;

        stat ("..", &parent);
        int parent_inode = parent.st_ino;
        
        DIR *dirp;
        struct dirent *dp;

        dirp = opendir ("..");

        while ((dp = readdir(dirp)) != NULL) {

            if (dp->d_ino == current_inode) {

                if (strcmp(dp->d_name, ".") == 0) {
                    s = "/root/" + s;
                }
                else {
                    s = "/" + s;
                    s = dp->d_name + s;
                }

            }
            
        }

        if (current_inode == parent_inode) break;
        
        chdir("..");
    }
    printf("%s\n", s.c_str());
    
    return 0;
}
