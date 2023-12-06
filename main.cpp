/*
    Build program: c++ main.cpp -o rpc.exe
*/

#include <Windows.h>
#include <conio.h>

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <vector>

enum instructions {
    NotDefined,
    instructionClear,
    instructionDs,
    instructionFs,
    instructionSs,
    instructionExit,
    instructionConf,
    instructionHelp
};

std::filesystem::path currentPath = std::filesystem::current_path();
static std::map<std::string, instructions> s_mapIns;
static std::vector<std::pair<std::string, std::string>> cmdDscp;
static char uInput[_MAX_PATH];
const std::string userName = "Repi";
std::string title = "Repi console | v: 0.01";
bool isRunning = true;

static void Initialize();
void errorMessage(std::string message, int type);
void fileHandler(std::filesystem::path path, bool method, bool recursive);
std::string formatInput(std::string input);
void configHandler(std::string fullString);
void commands();

int main() {
    Initialize();
    SetConsoleTitleA(title.c_str());
    std::wcout.imbue(std::locale::classic());
    while (isRunning) {
        std::cout << "\033[32m" << userName << "\033[0m\033[33m ~ \033[0m" << currentPath.string() << "\n";
        std::cout << "-$: ";
        std::cin.getline(uInput, _MAX_PATH);
        std::string formattedInput = formatInput(uInput);
        switch (s_mapIns[formattedInput]) {
            case instructionClear:
                system("cls");
                break;
            case instructionDs:
                fileHandler(currentPath.string(), true, false);
                break;
            case instructionFs:
                fileHandler(currentPath.string(), false, false);
                break;
            case instructionSs:
                fileHandler(currentPath, false, true);
                break;
            case instructionExit:
                isRunning = false;
                break;
            case instructionConf:
                configHandler(uInput);
                break;
            case instructionHelp:
                commands();
                break;
            default:
                if (uInput[0] != '\0') {
                    errorMessage(uInput, 0);
                }
                break;
        }
    }
    return 0;
}

void Initialize() {
    s_mapIns["clear"] = instructionClear;
    s_mapIns["c"] = instructionClear;
    s_mapIns["ds"] = instructionDs;
    s_mapIns["fs"] = instructionFs;
    s_mapIns["ss"] = instructionSs;
    s_mapIns["exit"] = instructionExit;
    s_mapIns["close"] = instructionExit;
    s_mapIns["config"] = instructionConf;
    s_mapIns["help"] = instructionHelp;
    s_mapIns["?"] = instructionHelp;
}

void errorMessage(std::string message, int type) {
    switch (type) {
        case 0:
            std::cout << "\n\033[31merror:\033[0m '" << message << "' is not recognizable.\n\n";
            break;
        default:
            break;
    }
}

void printerHandler(std::wstring fileName, bool method, long long int size) {
    if (method) {
        std::wcout << L"DIR \t??? byte\t" << fileName << L"\n";
    } else {
        std::wcout << L"FILE\t" << std::to_wstring(size) << L" byte \t" << fileName << L"\n";
    }

    return;
}

void fileHandler(std::filesystem::path path, bool method, bool recursive) {  // method: { true: dir; false: file }, recursive: { true: on; false: off }

    std::filesystem::path validpath;
    long long int bytes = 0;
    int fileNum = 0;
    int dirNum = 0;

    if (!path.empty()) {
        validpath = path;
    } else {
        validpath = currentPath;
    }

    if (recursive) {
        for (const auto &entry : std::filesystem::recursive_directory_iterator(validpath)) {
            std::wstring sFilename = entry.path().filename();
            if (entry.is_directory()) {
                dirNum++;
                printerHandler(sFilename, true, 0);
            } else if (entry.is_regular_file()) {
                fileNum++;
                int size = entry.file_size();
                bytes += size;
                printerHandler(sFilename, false, size);
            }
        }
    } else {
        for (const auto &entry : std::filesystem::directory_iterator(validpath)) {
            std::wstring sFilename = entry.path().filename();
            if (method && entry.is_directory()) {
                dirNum++;
                printerHandler(sFilename, true, 0);
            } else if (!method && entry.is_regular_file()) {
                fileNum++;
                int size = entry.file_size();
                bytes += size;
                printerHandler(sFilename, false, size);
            }
        }
    }

    if (!method && !recursive) {
        std::cout << "\t" << fileNum << " FILE(S) " << std::to_string(bytes) << " Byte\n";
    } else if (method && !recursive) {
        std::cout << "\t\t" << dirNum << " DIR(S)\n";
    }

    if (recursive) {
        std::cout << "\t" << fileNum << " FILE(S) " << std::to_string(bytes) << " Byte\n";
        std::cout << "\t\t" << dirNum << " DIR(S)\n";
    }

    return;
}

std::string formatInput(std::string input) {
    return input.substr(0, input.find_first_of(" "));
}

void configHandler(std::string fullString) {
    std::string args = fullString.substr(7, fullString.length());
    // not finished!
}

void commands() {
    cmdDscp.push_back({"clear, c", "Clears the screen."});
    cmdDscp.push_back({"exit, close", "Exits the program."});
    cmdDscp.push_back({"config", "You can configure: "});
    cmdDscp.push_back({"ds", "List all directories from the current path."});
    cmdDscp.push_back({"fs", "List all files from the current path."});
    cmdDscp.push_back({"ss", "List all files and directories from the current path."});
    for (const auto &pair : cmdDscp) {
        std::cout << pair.first << "\t\t- " << pair.second << "\n";
    }
}