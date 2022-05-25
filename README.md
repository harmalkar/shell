## Overview
This repository contains code for a simple Unix shell in C which can parse Unix commands, including those with single and double quotes. The shell also performs basic error handling. 

## Usage
To compile the code, run
```
make
```

To run the code, run
```
./shell
```

Example usage:
```
$ cd /
$ pwd
/
$ ls  
Applications	Volumes		etc		sbin
Library		bin		home		tmp
System		cores		opt		usr
Users		dev		private		var
$ echo "Hello"
Hello
$ printf "This is a shell that can handle %s\n" 'single and double quotes'
This is a shell that can handle single and double quotes
$ exit
```

## How does it work? 
When the user inputs a command, the shell calls fork(), which creates a child process that executes the user's desired instruction in a separate context. The environment variables are passed to the new context before running the command. The child process calls execvp(), which gives control to the command requested by the user and automatically returns any errors. The parent process then waits for the child process to complete, captures the return value of the child process and handles any errors if needed. 

A detailed introduction to these concepts can be found here: https://www.softprayog.in/programming/creating-processes-with-fork-and-exec-in-linux#:~:text=The%20shell%20is%20the%20parent,prompts%20for%20the%20next%20command.
