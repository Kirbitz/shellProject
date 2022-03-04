#ifndef SHELLCMD_H
#define SHELLCMD_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h>

class ShellCmd{
private:
  //Runs user command from a list of commands
  static void runCmd(char**);
  //Changes the current working directory
  static void chngDir(char**);
public:
  //Finds the current working directory
  static void curDir(int);
  //Runs a child command based on whether input, output, or a pipe is needed for input output redirection
  static void runChild(char**);
  static void runChild(char**, int&, bool);
  static void runChild(char**, int&, int&);
  static void runChild(char**, int*);
};

#endif