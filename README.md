Command Input:

The program prompts the user to enter a command.
It reads the input command line and tokenizes it into individual words.

Background Processing:
If a command ends with &, it is executed as a background process.
Background processes are handled by forking a new child process.

Built-in Commands:
cd <directory>: Changes the current working directory to the specified directory.
export <VAR>=<value>: Sets an environment variable.
echo <text>: Prints the specified text to the console. Supports environment variable expansion using $VAR.
exit: Exits the shell.

External Commands:
The shell can execute external commands like ls, mkdir, pwd, and others.
Commands are executed using the execvp function.

Command Logging:
A function termination() logs the termination of each child process to a file named logfile.txt.
The log file entry includes the process number of the terminated child process.

Error Handling:
If the user enters an unknown command, the shell prints "WRONG INPUT !!".
Directory change errors are handled and an appropriate message is displayed if the directory is not found.

Environment Variable Handling:
echo and external commands can expand environment variables prefixed with $.
The export command sets environment variables.

Implementation Details
Tokenization: The input command line is tokenized using strtok based on spaces.
Background Check: The presence of & at the end of the command is checked to determine if the command should run in the background.
Process Management: The fork function is used to create child processes for command execution.
Signal Handling: The signal function sets up a handler for SIGCHLD to log process terminations.
File Handling: The termination function opens a file in append mode to log terminated child processes.
