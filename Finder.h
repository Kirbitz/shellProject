#ifndef FINDER_H
#define FINDER_H

#include <string>
#include <vector>

class Finder{
public:
  //Check if tokens with vector contain |, <, or >
  static std::vector<int> findSpecialToken(const std::vector<std::string>);
};

#endif
