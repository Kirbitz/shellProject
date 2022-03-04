#include "./Finder.h"

/**
 * @brief finds special tokens of |, <, or > within command 
 * 
 * @param command the vector to check
 * @return a vector of positions of each special character
 */
std::vector<int> Finder::findSpecialToken(const std::vector<std::string> command){
  //The final result of the command
  std::vector<int> pipeRedirectPos;

  pipeRedirectPos.push_back(-1);
  pipeRedirectPos.push_back(-1);

  //loops through all of the string lines within the commands vector
  for(int pos = 0; pos < command.size(); pos++){
    //Checks if any of the tokens are equal to |, <, or >
    if (command.at(pos) == "|"){
      //pushes the position and value indicator of pipe to the pipeRedirectPos vector
      pipeRedirectPos.push_back(pos);
      pipeRedirectPos.push_back(1);
    }
    else if(command.at(pos) == "<"){
      //pushes the position and value indicator of input to the pipeRedirectPos vector
      pipeRedirectPos.push_back(pos);
      pipeRedirectPos.push_back(2);
    }
    else if(command.at(pos) == ">"){
      //pushes the position and value indicator of output to the pipeRedirectPos vector
      pipeRedirectPos.push_back(pos);
      pipeRedirectPos.push_back(3);
    }
  }

  pipeRedirectPos.push_back(command.size());
  pipeRedirectPos.push_back(-1);

  return pipeRedirectPos;
}

