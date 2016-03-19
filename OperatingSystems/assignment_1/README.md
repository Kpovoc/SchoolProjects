# UNIX Shell and History Feature  
This project consists of designing a C program to serve as a shell interface
that accepts user commands and then executes each command in a separate
process. The shell interface presents a prompt, and accepts a command from the
user.  

    osh> cat makefile

Typically the parent waits on the child process to complete before attempting
to get a new command from the user. The child process and parent process can
be ran concurrently by appending an & to the end of a command.  

    osh> cat makefile &

The shell also contains a history feature that allows users to access the most
recently entered commands. The user will be able to access up to 10 commands
by using the feature. The commands are numbered starting at 1, and the
numbering continues past 10. For example:  

    osh> history
    6) ps
    5) ls -l
    4) top
    3) cal
    2) who
    1) date
The shell also supports two techniques for retrieving commands from the
history. When a user enters !!, the most recent command in the history is
executed. The user can also enter a single ! followed by the corresponding
number in the history to execute the desired command. In the previous example,
we could do

    osh> !!
to run the 'ps' command and,

    osh> !2
to run the 'who' command.

NOTE: this shell does not feature directory traversal.
