The main objective of this program is to 
read 2 integers from input.txt, a file, with the scanf() function (to learn dup)

and print the sum

fds	file
0	STDIN
1	STDOUT (override by dup w/o 
2	STDERR


dup(file, value [lft column]),
dup1, dup2...

	
use to duplicate to file, "mbstdo.txt"



One example use would be I/O redirection. For this you fork a child process and close the stdin or stdout file descriptors (0 and 1) and then you do a dup() on another filedescriptor of your choice which will now be mapped to the lowest available file descriptor, which is in this case 0 or 1.

Using this you can now exec any child process which is possibly unaware of your application and whenever the child writes on the stdout (or reads from stdin, whatever you configured) the data gets written on the provided filedescriptor instead.

Shells use this to implement commands with pipes, e.g. /bin/ls | more by connecting the stdout of one process to the stdin of the other.