#ifndef PARSECMD_H
#define PARSECMD_H

#include <string>
#include <vector>
#include <sstream>

class ParseCmd{
public:
  //parses user input into a vector
  static std::vector<std::string> getTokenVector(std::string);
  //parses vector info into c string array
  static void cStringCmd(const std::vector<std::string>&, char**, int, int);
};

#endif
