#include "./ShellCmd.h"

/**
 * @brief Findes the currend working directory usering the getcwd command
 * 
 * @param part determines the if the read was from pwd or if it is for the shell location
 */
void ShellCmd::curDir(int part)
{
    // prints the current working directory
    char cwd[255];
    if (getcwd(cwd, sizeof(cwd)))
    {
        // Determins if full directory should be printed or if only part should be printed
        if (part == 0)
            std::cout << cwd << std::endl;
        //prints part of the working directory show current working path for the user
        else if (part == 1)
        {
            if(strcmp(cwd, "/home") == 0 || strcmp(cwd, "/") == 0){
                std::cout << cwd;
                return;
            }
            //Prints working directory in format of ~/foldername/foldername
            int i = 0, slashCount = 0;
            std::cout << "~";
            while (cwd[i] != '\0')
            {
                if (cwd[i] == '/')
                    slashCount++;
                if (slashCount > 2)
                    std::cout << cwd[i];
                i++;
            }
        }
    }
    // Runs if getcwd failed
    else
    {
        perror("Failed to get directory");
        exit(1);
    }
}

/**
 * Changes the current directory based on user command
 * 
 * @param userCmd the directory the user wants to change to
 */
void ShellCmd::chngDir(char **userCmd)
{
    if (userCmd[1] != NULL)
    {
        //Changes directory to the user entered path
        if (chdir(userCmd[1]) < 0)
            perror("Failed to change directory");
    }
    else
    {
        //Changes directory to user folder if no path is specified
        if (chdir("/home/kirbitz") < 0)
            perror("Failed to change directory");
    }
}

/**
 * @brief run commands of user input that is specified from a list of commands
 * 
 * @param userCmd the user command to run
 */
void ShellCmd::runCmd(char **userCmd)
{
    // Runs the exit command for the shell
    if (strcmp(userCmd[0], "exit") == 0)
    {
        exit(1);
    }
    // Run the pwd command
    if (strcmp(userCmd[0], "pwd") == 0)
    {
        ShellCmd::curDir(0);
    }
    // Runs a clear command
    else if (strcmp(userCmd[0], "clear") == 0)
    {
        std::cout << "\033[2J\033[1;1H";
    }
    else if (strcmp(userCmd[0], "cd") == 0)
    {
        exit(3);
    }
    // Runs a history command
    else if (strcmp(userCmd[0], "history") == 0)
    {
        // Truncates the history file if the user includes -c
        if (userCmd[1] != NULL && strcmp(userCmd[1], "-c") == 0)
        {
            std::ofstream clearFile;
            clearFile.open("/home/kirbitz/Documents/CodingAssignments/CodingAssignment4/shellHistory.txt", std::ios::out | std::ios::trunc);
            clearFile.close();
        }
        // Reads shellHistory.txt and prints commands entered from it
        std::ifstream readShellHistory;
        readShellHistory.open("/home/kirbitz/Documents/CodingAssignments/CodingAssignment4/shellHistory.txt");
        std::string historyCmd;
        int line = 1;
        while (std::getline(readShellHistory, historyCmd))
        {
            // Buffers spaces to mimic the actual history cmd
            std::string spaceBuffer = "     ";
            std::cout << spaceBuffer.substr(std::to_string(line).length()) << line << " " << historyCmd << std::endl;
            line++;
        }
        readShellHistory.close();
    }
    // Runs commands if it does not match any of the others listed
    else if (userCmd[0] != NULL)
    {
        if (execvp(userCmd[0], userCmd) < 0)
        { // Changes execution image of child
            perror("Failed to run user command");
            exit(2);
        }
    }
    exit(0);
}

/**
 * @brief Runs a basic child with only user command input
 * 
 * @param userCmd the command to run within the child
 */
void ShellCmd::runChild(char **userCmd)
{
    // Creates child to run the execvp command
    pid_t childpid;
    int status;

    if ((childpid = fork()) < 0)
        perror("Failed to create child");
    else if (childpid == 0)
    {
        runCmd(userCmd);
    }
    else
    {
        // Attemps to wait for child to finish process and grabs the exit code
        if (waitpid(childpid, &status, 0) == -1)
        {
            perror("Failed to wait!");
            exit(3);
        }
        // Runs if exit code was detected and if that code is one then parent process exits
        if (WIFEXITED(status))
        {
            // Exits the program if exit code was 1
            if (WEXITSTATUS(status) == 1)
                exit(1);
            else if (WEXITSTATUS(status) == 3) //Runs the change directory command in parent if exit is 3
                chngDir(userCmd);
        }
    }
}

/**
 * @brief runs child command with input and output redirection
 * 
 * @param userCmd the command to run within the child
 * @param file the input file to redirect information from
 * @param outFile the output file to redirect information to
 */
void ShellCmd::runChild(char **userCmd, int &file, bool outFile)
{
    // Creates child to run the execvp command
    pid_t childpid;
    int status;

    if ((childpid = fork()) < 0)
        perror("Failed to create child");
    else if (childpid == 0)
    {
        if (outFile)
            dup2(file, STDOUT_FILENO);
        else
            dup2(file, STDIN_FILENO);
        close(file);
        runCmd(userCmd);
    }
    else
    {
        // Attemps to wait for child to finish process and grabs the exit code
        if (waitpid(childpid, &status, 0) == -1)
        {
            perror("Failed to wait!");
            exit(3);
        }
        // Runs if exit code was detected and if that code is one then parent process exits
        if (WIFEXITED(status))
        {
            // Exits the program if exit code was 1
            if (WEXITSTATUS(status) == 1)
                exit(1);
            else if (WEXITSTATUS(status) == 3) //Runs the change directory command in parent if exit is 3
                chngDir(userCmd);
        }
    }
}

/**
 * @brief runs child command with input or output redirection
 * 
 * @param userCmd the command to run within the child
 * @param file the input file to redirect information from or to
 * @param outFile flag to determine either input or output
 */
void ShellCmd::runChild(char **userCmd, int &inFile, int &outFile)
{
    // Creates child to run the execvp command
    pid_t childpid;
    int status;

    if ((childpid = fork()) < 0)
        perror("Failed to create child");
    else if (childpid == 0)
    {
        dup2(outFile, STDOUT_FILENO);
        close(outFile);
        dup2(inFile, STDIN_FILENO);
        close(inFile);
        runCmd(userCmd);
    }
    else
    {
        // Attemps to wait for child to finish process and grabs the exit code
        if (waitpid(childpid, &status, 0) == -1)
        {
            perror("Failed to wait!");
            exit(3);
        }
        // Runs if exit code was detected and if that code is one then parent process exits
        if (WIFEXITED(status))
        {
            // Exits the program if exit code was 1
            if (WEXITSTATUS(status) == 1)
                exit(1);
            else if (WEXITSTATUS(status) == 3) //Runs the change directory command in parent if exit is 3
                chngDir(userCmd);
        }
    }
}

/**
 * @brief runs child command with pipe
 * 
 * @param userCmd the command to run within the child
 * @param fds the pipe direct output to
 */
void ShellCmd::runChild(char **userCmd, int *fds)
{
    // Creates child to run the execvp command
    pid_t childpid;
    int status;

    if ((childpid = fork()) < 0)
        perror("Failed to create child");
    else if (childpid == 0)
    {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        runCmd(userCmd);
    }
    else
    {
        // Attemps to wait for child to finish process and grabs the exit code
        if (waitpid(childpid, &status, 0) == -1)
        {
            perror("Failed to wait!");
            exit(3);
        }
        // Runs if exit code was detected and if that code is one then parent process exits
        if (WIFEXITED(status))
        {
            // Exits the program if exit code was 1
            if (WEXITSTATUS(status) == 1)
                exit(1);
            else if (WEXITSTATUS(status) == 3) //Runs the change directory command in parent if exit is 3
                chngDir(userCmd);
        }
    }
}