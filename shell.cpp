#include "./Finder.h"
#include "./ParseCmd.h"
#include "./ShellCmd.h"

using namespace std;

int main()
{
    // Opens history file to write/append to and initializes userCmd char pointer
    ofstream shellHistory;
    shellHistory.open("/home/kirbitz/Documents/CodingAssignments/CodingAssignment4/shellHistory.txt", ios::out | ios::app);
    char *userCmd[25];
    userCmd[0] = NULL;
    int originalSTDIN, originalSTDOUT;

    //Loops until user enters exit into terminal
    while (userCmd[0] == NULL)
    {
        //Storing original STDOUT and STDIN file descriptors
        originalSTDIN = dup(0);
        originalSTDOUT = dup(1);

        // Prints shell line and prompts user input
        string userInput;
        cout << "\033[1;35mmyshell@shell\033[0m:\033[1;31m";
        ShellCmd::curDir(1);
        cout << "\033[0m$ ";
        getline(cin, userInput);

        // Appends command to history
        if (userInput.length() > 0)
            shellHistory << userInput << endl;

        // Tokenized vector of user inputs
        vector<string> tokenizedInput = ParseCmd::getTokenVector(userInput);
        vector<int> pipeRedirctPos = Finder::findSpecialToken(tokenizedInput);

        // Runs loop to parse through commands based on pipe and redirect separation
        for (int i = 0; i < pipeRedirctPos.size() - 2; i += 2)
        {
            //Set cmd to NULL then initializes command array with next user command
            userCmd[0] = NULL;
            ParseCmd::cStringCmd(tokenizedInput, userCmd, pipeRedirctPos[i], pipeRedirctPos[i + 2]);
            
            //Breaks if user did not enter a command
            if (userCmd[0] == NULL)
                break;

            // Checks if pipeRedirect found a pipe variable
            if (pipeRedirctPos[i + 3] == 1)
            {
                //Spawns pipe for passing data through
                int fds[2];
                if (pipe(fds) < 0)
                {
                    perror("Failed to create pipe");
                }
                ShellCmd::runChild(userCmd, fds);
                dup2(fds[0], 0);
                close(fds[0]);
                dup2(originalSTDOUT, 1);
                close(fds[1]);
            }
            // Checks if pipeRedirect found a indirect variable
            else if (pipeRedirctPos[i + 3] == 2)
            {
                //Creates a file for reading data in from
                int inFile;
                if ((inFile = open(tokenizedInput[pipeRedirctPos[i + 2] + 1].c_str(), O_RDONLY, S_IRUSR)) < 0)
                {
                    perror("Failed to open file");
                }
                //Checks if input should be redirected to an output
                if (pipeRedirctPos[i + 5] == 3 && pipeRedirctPos[i + 4] != tokenizedInput.size())
                {
                    //Creates a file for writing data out to
                    int outFile;
                    if ((outFile = open(tokenizedInput[pipeRedirctPos[i + 4] + 1].c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0)
                    {
                        perror("Failed to open file");
                    }
                    //Runs child command with inFile and outFile
                    ShellCmd::runChild(userCmd, inFile, outFile);
                    i += 2;
                }
                else
                    //Runs child command with inFile and outFile data
                    ShellCmd::runChild(userCmd, inFile, false);
                i+=2;
            }
            // Checks if pipeRedirect found a outdirect variable
            else if (pipeRedirctPos[i + 3] == 3 && pipeRedirctPos[i + 2] != tokenizedInput.size())
            {
                //Creates a file for writing out data to
                int file;
                if ((file = open(tokenizedInput[pipeRedirctPos[i + 2] + 1].c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0)
                {
                    perror("Failed to open file");
                }
                //Runs the child command with the outfile and notifiying the command that it is receiving an outfile
                ShellCmd::runChild(userCmd, file, true);
                i += 2;
            }
            else
            {
                //Runs a standard command
                ShellCmd::runChild(userCmd);
            }
        }
        // Sets userCmd[0] to NULL so loop will not exit
        userCmd[0] = NULL;

        // Sets file descriptor of stdin and stdout back to default
        dup2(originalSTDIN, 0);
        dup2(originalSTDOUT, 1);
        close(originalSTDIN);
        close(originalSTDOUT);
        fflush(stdout);
    }
    // Closes history file
    shellHistory.close();
    return 0;
}
