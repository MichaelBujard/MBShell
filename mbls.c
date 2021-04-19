#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>    
#include <sys/stat.h>
#include <dirent.h>

static void find_dir_entry (struct stat *dot_sb, char *filename){
  DIR *dirp;
  struct stat fileStat;
  struct dirent *dp;

  dirp = opendir (filename);
  printf("cwd inode is %ld\n", dot_sb->st_ino);

  printf("Current directory files\n");
  while ((dp = readdir(dirp)) != NULL){
    stat(dp->d_name, &fileStat);
    printf("%ld, %lu: %s\n", fileStat.st_ino, dp->d_ino, dp->d_name);
  }

  closedir(dirp);

}


int main(int argc, char *argv[]){
  struct stat dot_sb;

  stat (argv[1], &dot_sb);
  find_dir_entry (&dot_sb, argv[1]);
  return 0;
}
