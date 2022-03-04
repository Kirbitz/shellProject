#include "./ParseCmd.h"

/**
 * @brief parses the userLine input into a vector by deliminating spaces
 * 
 * @param userLine the user input to parse
 * @return a vector of parsed string data
 */
std::vector<std::string> ParseCmd::getTokenVector(std::string userLine)
{
    //Declares tmpVec to return parsed string
    std::vector<std::string> tmpVec;

    //Parses string into currentToken and then pushes token to vector
    std::stringstream streamObj = std::stringstream(userLine);
    std::string currentToken;
    while (streamObj >> currentToken)
        tmpVec.push_back(currentToken);
    return tmpVec;
}

/**
 * @brief parses the myStringTokens vector into a c string array
 * 
 * @param myStringTokens the vector to parse
 * @param userCmd the c string array to feed into
 * @param start the start position of the parsing
 * @param end  the end position of the parsing
 */
void ParseCmd::cStringCmd(const std::vector<std::string> &myStringTokens, char**userCmd, int start, int end){
    start++;
    //Checks if command needs to be parsed
    if(end - start != 0){
        //Parses the user command int required parts
        userCmd[end-start] = NULL;
        for (int pos = 0; pos < end-start; pos++){
            userCmd[pos] = (char*) myStringTokens[pos+start].c_str();
        }
    }
}