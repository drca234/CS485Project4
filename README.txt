Authors:
  Kendall Weihe
  Daniel Carroll
Date: November 10th, 2016
Assignment: CS485G Fall 2016 programming project #4
Purpose:
  - Teach the students how to develop a simple shell "sish"
    - Tokenizing input
    - Forking processes
    - Starting new processes (within child processes)
    - Waiting or not waiting for child processes to complete
    - Use C library calls such as chdir() and setenv()
    - Store and access shell variables

Requirements: be able to execute the following commands:
  - %
    - comment -- do nothing
  - set variable value
    - assign "value" to the "variable"
    - this can later be accessed using the "$" prefix metacharacter
  - defprompt prompt
    - change the prompt name to "prompt"
  - cd directory_name
    - change the current working directory to directory_name
  - listprocs
    - list all the processes the shell started
  - done
    - exit the shell program
  - run cmd param* [&]
    - run the Linux command "cmd" with the arguments "param*"
    - run in the background if the last token is "&"
  - assignto variable cmd param*
    - assign the output of the Linux command "cmd" (with arguments "param*") to "variable"
    - "variable" can later be accessed using the "$" prefix metacharacter

Files:
  - main.cpp
    - The source code of the project
  - Makefile
    - A makefile to compile and clean the program
  - README.txt
    - *this document

How to run the program:
  - Run this program in a Linux environment (we used both Ubuntu 14.10 and 16.04)
  - Run `make` (or `make clean`) in the project directory
  - Run `./main`
  - Begin using the simple shell

Important variables:
  - global variables (all vectors)
    - tokens --> this is filled for every command and then emptied after the command has completed running
    - process names
    - process IDs
      - process names and process ID's run in parallel
    - variable names
    - variable values
      - variable names and variable values run in parallel

General program flow of control:
  - initialize key variables
    - PATH and ShowTokens
  - loop until the user inputs "done"
    - read user input
    - tokenize the user input
      - use the tokenize() and parse() functions
    - call the function to execute the command based on the leading token
      - notable functions:
        - execute_set_command()
          - assigns a variable name and value to their global variables
        - execute_def_prompt()
          - returns a string that is the new prompt
        - execute_cd()
          - uses the C library chdir() function to change the active directory
        - execute_listprocs()
          - prints a list of processes and IDs --> stored in the global variables
        - execute_run()
          - parses the tokens looking for the & metacharacter -- to run in the background
          - forks a child process
            - uses execvp() to start the new process
          - waits or doesn't wait for child process
        - execute_assignto()
          - redirects stdout to a file
          - forks a child process
            - uses execvp() to start the new process
          - resets stdout
          - saves the result of the execvp() to the given variable, if the variable is valid
        - valid_variable()
          - checks the given string to see if it is alphanumeric
        - search()
          - looks in a given vector of strings for a given string
            - returns the index if found, or -1 if not found
