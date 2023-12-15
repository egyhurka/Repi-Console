/*
    Build program and run:
        1. c++ main.cpp fileHandler.cpp utilities.cpp variables.cpp -o Resc.exe
        2. ./Resc.exe
*/

#include <Windows.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include "includes/fileHandler.h"
#include "includes/utilities.h"
#include "includes/variables.h"

bool isRunning = true;
static char uInput[_MAX_PATH];

std::string formatInput(std::string&& input) {
    return input.substr(0, input.find_first_of(' '));
}

int main() {
    MainConstructor();
    std::wcout.imbue(std::locale::classic());
    system("cls");
    while (isRunning) {
        std::string username = "Repi";
        std::cout << "\033[32m" << username << "\033[0m\033[33m ~ \033[0m" << globalVariables.currentPath.string() << "\n";
        std::cout << "-$: ";
        std::cin.getline(uInput, _MAX_PATH);
        std::string fullCommand = uInput;
        std::string formattedInput = formatInput(uInput);
        switch (s_mapIns[formattedInput]) {
            case instructionClear:
                if (formattedInput == "clear" || formattedInput == "c") {
                    system("cls");
                }
                break;
            case instructionExit:
                isRunning = false;
                break;
            case instructionEmbeddedArgs:
                EmbeddedCommands(fullCommand);
                break;
            case instructionHelp:
                CommandsHelp();
                break;
            default:
                if (uInput[0] != '\0')
                    ErrorMessage(uInput, 0);
                break;
        }
    }
    return 0;
}