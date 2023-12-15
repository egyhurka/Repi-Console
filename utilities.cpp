#include <windows.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "includes/utilities.h"

void CommandsHelp() {
    std::vector<std::pair<std::string, std::string>> cmdDscp;
    cmdDscp.emplace_back("clear, c", "Clears the screen.");
    cmdDscp.emplace_back("exit, ex, close", "Exits the program.");
    cmdDscp.emplace_back("config", "You can configure: ");
    cmdDscp.emplace_back("ds", "List all directories from the current path.");
    cmdDscp.emplace_back("fs", "List all files from the current path.");
    cmdDscp.emplace_back("ss", "List everything (recursively) from the current path.");
    cmdDscp.emplace_back("ssc", "List all files and directories from the current path.");
    cmdDscp.emplace_back("cd", "Changes the current directory. \n  Args:\t'..' - current path parent.\n\t'./' - go to 'C:\\'.\n\t'\"full path\"' - you can write exact path.");
    for (const auto& pair : cmdDscp) {
        std::cout << pair.first << "\t\t- " << pair.second << "\n";
    }
}

void ErrorMessage(const std::string& errorDescription, int type) {
    switch (type) {
        case 0:
            std::cout << "\n\033[31merror:\033[0m '" << errorDescription  << "' is not recognizable.\n\n";
            break;
        case 1:
            std::cout << "\n\033[31merror:\033[0m '" << errorDescription  << "' is not a valid path.\n\n";
            break;
        case 2:
            std::cout << "\n\033[31merror:\033[0m Recursive listing of all files is restricted for security reasons.\n\n";
        default:
            break;
    }
}

std::string title = "Repi's Easy Console | v: 0.03";

std::map<std::string, instructions> s_mapIns;

void MainConstructor() {
    SetConsoleTitleA(title.c_str());
    s_mapIns["clear"] = instructionClear;
    s_mapIns["c"] = instructionClear;
    s_mapIns["ds"] = instructionEmbeddedArgs;
    s_mapIns["fs"] = instructionEmbeddedArgs;
    s_mapIns["ss"] = instructionEmbeddedArgs;
    s_mapIns["ssc"] = instructionEmbeddedArgs;
    s_mapIns["config"] = instructionEmbeddedArgs;
    s_mapIns["cd"] = instructionEmbeddedArgs;
    s_mapIns["mkd"] = instructionEmbeddedArgs;
    s_mapIns["mkf"] = instructionEmbeddedArgs,
    s_mapIns["exit"] = instructionExit;
    s_mapIns["close"] = instructionExit;
    s_mapIns["ex"] = instructionExit;
    s_mapIns["help"] = instructionHelp;
    s_mapIns["?"] = instructionHelp;
}